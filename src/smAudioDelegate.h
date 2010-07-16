/*
 *  smAudioDelegate.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef SMAUDIODELEGATE
#define SMAUDIODELEGATE

#include "dfMidiOut.h"
#include "dfMidiScale.h"
#include "smModel.h"
#include <iostream>

class smModel;

class smAudioDelegate {
	
public:
	smAudioDelegate(smModel * m);
	string getScaleName(int index);
	void playNote(int scaleIndex, int noteIndex, unsigned char velocity);
	int getNumNotes();
private:
	smModel * model;
	
	// midi interface
	dfMidiOut * midiOut;
	
	// the current scale in use
	dfMidiScale * scale;
	
	// all scales as options
	dfMidiScale ** scales;
	
	void setupScales();
};

#endif