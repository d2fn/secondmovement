/*
 *  smView.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "smView.h"

smView::smView(smModel * m) {
	model = m;
	motionPainter = new dfMotionPainter(
							model->motionFrames,
							model->captureWidth,
							model->captureHeight);
}

void smView::draw() {
	
	ofPushMatrix();
	
	// paint as a mirror image so camera input is more intuitive
	ofRotate(180, 0, 1, 0);
	ofTranslate(-ofGetWidth(), 0, 0);
	
	// show the raw camera output
	ofSetColor(255,255,255);
	model->grayscaleFrame.draw(0,0,ofGetWidth(),ofGetHeight());
	
	ofEnableAlphaBlending();
	ofFill();
	
	// push new pixels to the motion painter
	motionPainter->submitFrame(model->colorFrame.getPixels());
	// paint the ghost trails
	motionPainter->drawMotion(
						model->motionPaintThreshold,
						model->motionPaintThreshold,
						true,
						model->r,
						model->g,
						model->b);
	
	drawGrid(model->diffGrid,
			 999,
			 model->r,
			 model->g,
			 model->b,
			 255);
	
	ofDisableAlphaBlending();
	
	int x1,y1,x2,y2 = 0;
	ofNoFill();
	
	// when debugging, show the raw camera output as a
	// thumbnail in the lower right at 10% size
	if(model->debug) {
		ofSetColor(255,255,255);
		model->colorFrame.draw(
							0,
							ofGetHeight() - ofGetHeight() * 0.1f,
							ofGetWidth()  * 0.1f,
							ofGetHeight() * 0.1f
						);
	}
	
	// render notes
	drawNotes();
	
	// resume normal drawing in standard coordinates
	ofPopMatrix();
	
	// render the color spectrum along the bottom of the screen
	for(int i = 0; i < ofGetWidth(); i++) {
		float hue = (float)i/(float)ofGetWidth();
		int color = model->hsb2rgb(hue, 1.0f, 1.0f);
		ofSetColor(color);
		ofLine(i, ofGetHeight(), i, ofGetHeight()-2);
	}
	
	// indicate with an add'l tick mark where the current color is
	ofSetColor(model->r,model->g,model->b);
	
	int hue_x = model->hue * ofGetWidth();
	ofLine(hue_x,ofGetHeight()-2,hue_x,ofGetHeight()-5);
	
	// display the heads up menu
	if(model->hudVisible) {
		
		ofDisableAlphaBlending();
		
		for(int i = 0; i < model->numScales; i++) {
			if(i == model->scaleIndex) {
				ofSetColor(model->r,model->g,model->b);
			}
			else {
				ofSetColor(255,255,255);
			}
			ofDrawBitmapString(model->getScaleName(i), 50, 20+i*15);
		}
	}
}

void smView::drawGrid(unsigned char * grid, int threshold, int r, int g, int b, int a) {
	
	// save style from caller
	ofPushStyle();
	
	ofFill();
	
	for(int y = 0; y < model->vres; y++) {
		
		// draw a faint line at each note position, kinda like sheet music
		ofSetColor(255,255,255,10);
		ofLine(
			0, y * model->blockHeight + model->blockHeight/2,
			ofGetWidth() - 30, y * model->blockHeight + model->blockHeight/2);
		
		// draw a transparent white block from where each note may originate
		ofSetColor(255,255,255,50);
		ofRect(ofGetWidth()-30, y*model->blockHeight+1,30,model->blockHeight/2+4);
	}
	
	ofPopStyle();
}

void smView::drawNotes() {

	if(model->playedNotes->notes == NULL) {
		return;
	}
	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofFill();
	node * n;
	
	for(n = model->playedNotes->notes; n != NULL; n = n->link) {
		
		ofSetColor(n->r,n->g,n->b,(int)n->a);
		
		if(model->notemode == scrolling) {
			ofRect(n->x, n->y, n->w, n->h);		
		}
		else {
			ofEllipse(n->x, n->y, n->w, n->h);
		}
		
		/**
		float factor = 1.0f;		
		
		for(int i = 0; i < 7; i++) {
			ofSetColor(n->r,n->g,n->b,n->a*factor);
			ofCircle(n->x-10*i, n->y, 8);
			factor *= 0.66;
		}
		 **/
	}
	
	ofPopStyle();
}
