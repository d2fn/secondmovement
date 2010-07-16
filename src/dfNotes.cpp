/*
 *  dfNotes.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 10/27/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "dfNotes.h"

dfNotes::dfNotes() {
	notes = NULL;
}

void dfNotes::addNote(float x, float y, float xv, float yv, float xa, float ya, float w, float h, int r, int g, int b, float a, float aa) {
	
	node *n = new node;
	
	n->x = x;
	n->y = y;
	n->xv = xv;
	n->yv = yv;
	n->xa = xa;
	n->ya = ya;
	n->w = w;
	n->h = h;
	n->r = r;
	n->g = g;
	n->b = b;
	n->a = a;
	n->aa = aa;
	n->link = NULL;
	n->updates = 0;
	
	// notes = n;
	
	// add note to end of list
	if(notes == NULL) {
		notes = n;
	}
	else {
		node* next = notes;
		while(next->link != NULL) {
			next = next->link;
		}
		next->link = n;
	}
}
