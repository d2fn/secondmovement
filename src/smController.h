/*
 *  smController.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef SMCONTROLLER
#define SMCONTROLLER

#include "smModel.h"
#include "ofxOpenCv.h"
#include <iostream>

class smController {
	
public:
	
	// constructor based on given model
	smController(smModel * m);
	
	// perform additional initialization
	void setup();
	
	// called when the application state should be updated based on new information
	void update();
	
	// next color please
	void advanceHue();
	
	// advance note potisions
	void advanceNotes();
	
	// input handlers
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
private:
	// application state
	smModel * model;
	
	// devices and inputs
	ofVideoGrabber videoGrabber;
	
	void emitNote(int noteIndex, unsigned char velocity, float x, float y);
};

#endif