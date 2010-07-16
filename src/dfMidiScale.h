/*
 *  dfMidiScale.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 9/7/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef _DF_MIDI_SCALE
#define _DF_MIDI_SCALE

#include "dfMidiNotes.h"
#include "dfMidiOut.h"

#include <string.h>
using namespace std;

class dfMidiScale {
public:
	dfMidiScale(string _name, unsigned char * scale, int scaleSize, int n);
	dfMidiScale();
	string getName();
	unsigned char getNote(int noteIndex);
	int getNumNotes();
	void chord(int *chord, int noteIndex, int offset1, int offset2);
	void playChords(dfMidiOut *midiOut);
	
private:
	string name;
	unsigned char * notes;
	int numNotes;
};

#endif