/*
 *  smController.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "smController.h"

float vmap(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

smController::smController(smModel * m) {
	
	model = m;
	
	// initialize the camera for input
	videoGrabber.setVerbose(true);
	videoGrabber.initGrabber(
			model->captureWidth,
			model->captureHeight);

}

void smController::setup() { }

void smController::update() {
	
	// grab a new frame from camera
	videoGrabber.grabFrame();
	
	if(videoGrabber.isFrameNew()) {		
		
		model->submitFrame(videoGrabber.getPixels());
		
		// check for motion detect
		if(model->motionDetected) {
		
			if(model->debug) {
				cout << "motion bbox = ("
				     << model->motionRectX1 << ","
				     << model->motionRectY1 << ","
				     << model->motionRectX2 << ","
				     << model->motionRectY2
				     << ")" << endl;
			}
			
			int velocity, noteIndex;
			
			// 'intelligent' music generation based on 
			// y-histogram of motion detect translated directly into musical chords
			if(model->playChromatic) {
				
				for(int y = model->motionRectY1; y <= model->motionRectY2; y += 3) {
					int yvel = 0;
					for(int x = model->motionRectX1; x <= model->motionRectX2; x++) {
						if(model->getClampAt(x,y) > 0) {
							yvel += model->getDiffAt(x,y);
						}
					}
					
					velocity = vmap(yvel,1,10000,30,127);
					if(velocity > 127) {
						velocity = 127;
					}
					noteIndex = model->getNoteIndex(y);
					
					emitNote(noteIndex, velocity, 0.f, (float)y*model->blockHeight);
				}
			}
			// otherwise, just fake it by playing at the top of the motion detect boundary
			else {
				
				// figure out which notes to trigger and fire add to model and trigger audio delegate
				int noteIndex = model->getNoteIndex(model->motionRectY1);
				// get the size of the area as an indicator of volume
				int detectSize =
						(model->motionRectX2 - model->motionRectX1) *
						(model->motionRectY2 - model->motionRectY1);
				
				// calculate note volume
				int velocity = vmap(detectSize,1,1000,45,127);
				if(velocity > 127) {
					velocity = 127;
				}
				
				emitNote(noteIndex, velocity, model->motionRectX1 * model->blockWidth, (float)model->motionRectY1 * model->blockHeight);
				
				if(noteIndex >= 5) {
					emitNote(noteIndex-5, velocity*0.5f, model->motionRectX1 * model->blockWidth, (float)(model->motionRectY1+5) * model->blockHeight);
				}
				
				//if(noteIndex >= 12) {
				//	emitNote(noteIndex-12,velocity*0.5, model->motionRectX1 * model->blockWidth, (float)(model->motionRectY1+12) * model->blockHeight);
				//}
			}
		}
	}
	
	model->update();
	model->advanceHue();
	advanceNotes();
}

void smController::advanceNotes() {
	
	if(model->playedNotes->notes == NULL) {
		return;
	}
	
	// clean up old notes
	while(model->playedNotes->notes->updates > 600 && model->playedNotes->notes->link != NULL) {
		node * nodeToDelete = model->playedNotes->notes;
		model->playedNotes->notes = model->playedNotes->notes->link;
		delete nodeToDelete;
	}
	
	// advance note positions
	node* n;
	for(n = model->playedNotes->notes; n != NULL; n = n->link) {
		
		n->x += n->xv;
		n->y += n->yv;
		n->xv += n->xa;
		n->yv += n->ya;
		
		if(model->notemode == falling) {
			if(n->x >= ofGetWidth()) {
				n->xv = -n->xv * 0.7f;
				n->x--;
			}
			else if(n->x <= 0) {
				n->xv = -n->xv * 0.7f;
				n->x++;			
			}
			
			if(n->y >= ofGetHeight()) {
				n->yv = -n->yv * 0.6f;
				n->y--;
			}
			else if(n->y <= 0) {
				n->yv = -n->yv * 0.6f;
				n->y++;			
			}
		}
		
		n->a *= n->aa;
		
		n->updates++;
	}
}

//#define SM_DORELEASE

void smController::emitNote(int noteIndex, unsigned char velocity, float x, float y) {
	
#ifdef SM_DORELEASE
	noteIndex = model->getNumNotes() - noteIndex - 1;
#endif
	
	// send note to audio delegate
	model->audioDelegate->playNote(
								   model->scaleIndex,
								   noteIndex,
								   velocity);
	
	if(model->notemode == scrolling) {
		
		// add notes to model
		float notex = ofGetWidth()-model->blockWidth;
		model->playedNotes->addNote(notex,y,   // p
									-3.7f,0.f, // v
									0.f,0.f,   // a
									ofGetWidth()/34, model->blockHeight-4, // size
									model->r,model->g,model->b, // color
									vmap(velocity,0,127,0,120),1.f); // alpha
			
	}
	else if(model->notemode == falling) {
		
		for(int x = 0; x < model->hres; x++) {	
			for(int y = 0; y < model->vres; y++) {
				unsigned char motionAmount = model->getDiffAt(x,y);
				if(motionAmount > 45) {
					float radius = vmap(motionAmount,0,255,5,20);
					float notex = x*model->blockWidth + ofRandom(0.f,model->blockWidth);
					float notey = y*model->blockHeight + ofRandom(0.f,model->blockHeight);
					model->playedNotes->addNote(
											notex,notey,
											0.f,0.f,
											0.f,3.f,
											radius,radius,
											model->r,model->g,model->b,
											vmap(velocity,0,127,0,120),0.98f);
				}
			}
		}
	}
}

void smController::keyPressed(int key) {
	
	// space key shows and hides the hud
	if(key == ' ') {
		model->hudVisible = !model->hudVisible;
	}
	
	// left and up keys go to the previous scale
	if(key == 356 || key == 357) {
		model->prevScale();
	}
	
	// right and down keys advance to the next scale	
	if(key == 359 || key == 358) {
		model->nextScale();
	}
	
	if(key == '1') {
		model->notemode = scrolling;
	}
	else if(key == '2') {
		model->notemode = falling;
	}
}

void smController::keyReleased(int key) { }
void smController::mouseMoved(int x, int y ) { }
void smController::mouseDragged(int x, int y, int button) { }
void smController::mousePressed(int x, int y, int button) { }
void smController::mouseReleased() { }
