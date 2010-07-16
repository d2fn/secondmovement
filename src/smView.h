/*
 *  smView.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef SMVIEW
#define SMVIEW

#include "ofMain.h"
#include "dfMotionPainter.h"
#include "smModel.h"
#include "dfNotes.h"

class smView {
public:
	smView(smModel * m);
	void draw();
private:
	void drawGrid(unsigned char * grid, int threshold, int r, int g, int b, int a);
	void drawNotes();
	smModel * model;
	dfMotionPainter * motionPainter;
};

#endif