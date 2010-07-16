/*
 *  dfNotes.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 10/27/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef DFNOTES
#define DFNOTES

#include "ofMain.h"

struct node {
	float x,y,   // position
	      xv,yv, // velocity
	      xa,ya, // acceleration
	      w,h;   // size
	
	int r,g,b;
	float a,aa; // alpha ad degredation
	int updates;
	long createTime;
	node * link;
};

class dfNotes {
	
	public:	
		node * notes;
	
		dfNotes();
		void addNote(float x, float y, float xv, float yv, float xa, float ya, float w, float h, int r, int g, int b, float a, float aa);
};

#endif