/*
 *  dfMidiScale.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 9/7/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "dfMidiScale.h"

#include <iostream.h>

dfMidiScale::dfMidiScale(string _name, unsigned char * scale, int scaleSize, int _numNotes) {
	
	name = _name;
	
	// set aside one octave for each repetition
	numNotes = _numNotes;
	notes = (unsigned char *)malloc(numNotes*sizeof(unsigned char));
	unsigned char * notePtr = notes;
	
	int noteCount = 0;
	int octave = 0;
	int maxNote = -1;
	while(noteCount < numNotes) {
		for(int i = 0; i < scaleSize && noteCount < numNotes; i++) {
			int note = scale[i] + octave*12;
			if(note >= 127 && maxNote == -1) {
				maxNote = notePtr[-1];
			}
			if(maxNote == -1) {
				*notePtr++ = scale[i] + octave*12;
			}
			else {
				*notePtr++ = maxNote;
			}
			noteCount++;
		}
		octave++;
	}
	
	// print notes
	for(int i = 0; i < numNotes; i++) {
		cout << "notes["<<i<<"]  : " << (int)notes[i] << endl;
	}
}

dfMidiScale::dfMidiScale() {
	free(notes);
}

string dfMidiScale::getName() {
	return name;
}

unsigned char dfMidiScale::getNote(int noteIndex) {
	return notes[noteIndex];
}

int dfMidiScale::getNumNotes() {
	return numNotes;
}

void dfMidiScale::chord(int *chord, int noteIndex, int offset1, int offset2) {
	chord[0] = notes[noteIndex];
	chord[1] = notes[noteIndex+offset1];
	chord[2] = notes[noteIndex+offset2];
}

void dfMidiScale::playChords(dfMidiOut *midiOut) {
	for(int i = 10; i < 50; i++) {
		//		midiProgression->chord(chord, i, 4, 7);
		printf("playing note %i\n",notes[i]);
		midiOut->sendNoteOn(0, notes[i], 100,false);
		usleep(1000000);
//		midiOut->sendNoteOn(0, notes[i+4], 100);
//		usleep(100000);
//		midiOut->sendNoteOn(0, notes[i+7], 100);
//		usleep(1000000);
	}
}