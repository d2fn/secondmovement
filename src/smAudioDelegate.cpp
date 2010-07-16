/*
 *  smAudioDelegate.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 ISS. All rights reserved.
 *
 */

#include "smAudioDelegate.h"

smAudioDelegate::smAudioDelegate(smModel * m) {
	model = m;
	midiOut = new dfMidiOut;
	setupScales();
}

int smAudioDelegate::getNumNotes() {
	return model->vres;
}

string smAudioDelegate::getScaleName(int index) {
	if(index < 0 || index >= model->numScales) {
		return "<empty>";
	}
	return scales[index]->getName();
}

void smAudioDelegate::playNote(int scaleIndex, int noteIndex, unsigned char velocity) {
	if(model->debug) {
		cout << "playNote-->" << endl;
		cout << "- scaleIndex : " << scaleIndex << endl;
		cout << "- noteIndex  : " << noteIndex << endl;
		cout << "- velocity   : " << (unsigned int)velocity << endl;
		cout << "- scales[scaleIndex]->getNote(noteIndex) " <<
			endl << (int)scales[scaleIndex]->getNote(noteIndex) <<
			endl << endl;
	}
	midiOut->sendNoteOn(0, scales[scaleIndex]->getNote(noteIndex), velocity, model->debug);
}

void smAudioDelegate::setupScales() {
	
	int numScales = 13;
	scales = new dfMidiScale*[numScales];
	
	// leave room enough for 8 notes max
	unsigned char scale[8];
	
	/**
	 * C Minor
	 **/
	scale[0] = MIDI_C+12;
	scale[1] = MIDI_D+12;
	scale[2] = MIDI_E_FLAT+12;
	scale[3] = MIDI_F+12;
	scale[4] = MIDI_G+12;
	scale[5] = MIDI_A_FLAT+12;
	scale[6] = MIDI_B_FLAT+12;
	scales[0] = new dfMidiScale("C Minor",scale,7,getNumNotes());
	
	/**
	 * Enigmatic (G)
	 **/
	scale[0] = MIDI_G+12;
	scale[1] = MIDI_A_FLAT+12;
	scale[2] = MIDI_B+12;
	scale[3] = MIDI_C_SHARP+12;
	scale[4] = MIDI_D_SHARP+12;
	scale[5] = MIDI_E_SHARP+12;
	scale[6] = MIDI_F_SHARP+12;
	scale[7] = MIDI_G+12;
	scales[1] = new dfMidiScale("Enigmatic (G)",scale,8,getNumNotes());
	
	/**
	 * Pentatonic (C)
	 * C G D A E
	 **/
	scale[0] = MIDI_C;
	scale[2] = MIDI_G;
	scale[1] = MIDI_D;
	scale[4] = MIDI_A;
	scale[3] = MIDI_E;
	scales[2] = new dfMidiScale("Pentatonic (C)",scale,5,getNumNotes());
	
	/**
	 * Hirojoshi (D)
	 **/
	scale[0] = MIDI_D;
	scale[1] = MIDI_E_FLAT;
	scale[2] = MIDI_G;
	scale[3] = MIDI_A;
	scale[4] = MIDI_B_FLAT;
	scales[3] = new dfMidiScale("Hirojoshi (D)",scale,5,getNumNotes());
	
	/**
	 * Ukranian Minor (C)
	 **/
	scale[0] = MIDI_C+12;
	scale[1] = MIDI_D+12;
	scale[2] = MIDI_E_FLAT+12;
	scale[3] = MIDI_F_SHARP+12;
	scale[4] = MIDI_G+12;
	scale[5] = MIDI_A+12;
	scale[6] = MIDI_B_FLAT+12;
	scales[4] = new dfMidiScale("Ukranian Minor (C)",scale,7,getNumNotes());
	
	/**
	 * Ukranian Minor (A)
	 **/
	scale[0] = MIDI_A+12;
	scale[1] = MIDI_B+12;
	scale[2] = MIDI_C+12;
	scale[3] = MIDI_D_SHARP+12;
	scale[4] = MIDI_E+12;
	scale[5] = MIDI_F_SHARP+12;
	scale[6] = MIDI_G+12;
	scales[5] = new dfMidiScale("Ukranian Minor (A)",scale,7,getNumNotes());
	
	/**
	 * Hungarian Minor (A)
	 **/
	scale[0] = MIDI_A+12;
	scale[1] = MIDI_B+12;
	scale[2] = MIDI_C+12;
	scale[3] = MIDI_D_SHARP+12;
	scale[4] = MIDI_E+12;
	scale[5] = MIDI_F+12;
	scale[6] = MIDI_G_SHARP+12;
	scales[6] = new dfMidiScale("Hungarian Minor (A)",scale,7,getNumNotes());
	
	/**
	 * Hungarian Minor (C)
	 **/
	scale[0] = MIDI_C+12;
	scale[1] = MIDI_D+12;
	scale[2] = MIDI_E_FLAT+12;
	scale[3] = MIDI_F_SHARP+12;
	scale[4] = MIDI_G+12;
	scale[5] = MIDI_A_FLAT+12;
	scale[6] = MIDI_B+12;
	scales[7] = new dfMidiScale("Hungarian Minor (C)",scale,7,getNumNotes());
	
	/**
	 * Diminished E Flat
	 **/
	scale[0] = MIDI_E_FLAT+12;
	scale[1] = MIDI_F+12;
	scale[2] = MIDI_F_SHARP+12;
	scale[3] = MIDI_G_SHARP+12;
	scale[4] = MIDI_A+12;
	scale[5] = MIDI_B+12;
	scale[6] = MIDI_C+12;
	scale[6] = MIDI_D+12;
	scales[8] = new dfMidiScale("Diminised (E flat)",scale,7,getNumNotes());
	
	/**
	 * Diminished D
	 **/
	scale[0] = MIDI_D+12;
	scale[1] = MIDI_E+12;
	scale[2] = MIDI_F+12;
	scale[3] = MIDI_G+12;
	scale[4] = MIDI_A_FLAT+12;
	scale[5] = MIDI_B_FLAT+12;
	scale[6] = MIDI_B+12;
	scale[7] = MIDI_C_SHARP+12;
	scales[9] = new dfMidiScale("Diminised (D)",scale,8,getNumNotes());
	
	/**
	 * Lydian Dominant (C)
	 **/
	scale[0] = MIDI_C+12;
	scale[1] = MIDI_D+12;
	scale[2] = MIDI_E+12;
	scale[3] = MIDI_F_SHARP+12;
	scale[4] = MIDI_G+12;
	scale[5] = MIDI_A+12;
	scale[6] = MIDI_B_FLAT+12;
	scales[10] = new dfMidiScale("Lydian Dominant (C)",scale,7,getNumNotes());
	
	/**
	 * Persian (C)
	 **/
	scale[0] = MIDI_C+12;
	scale[1] = MIDI_D_FLAT+12;
	scale[2] = MIDI_E+12;
	scale[3] = MIDI_F+12;
	scale[4] = MIDI_G_FLAT+12;
	scale[5] = MIDI_A_FLAT+12;
	scale[6] = MIDI_B+12;
	scales[11] = new dfMidiScale("Persian (C)",scale,7,getNumNotes());
	
	/**
	 * Half-diminished (C)
	 **/
	scale[0] = MIDI_C+12;
	scale[1] = MIDI_D+12;
	scale[2] = MIDI_E_FLAT+12;
	scale[3] = MIDI_F+12;
	scale[4] = MIDI_G_FLAT+12;
	scale[5] = MIDI_A_FLAT+12;
	scale[6] = MIDI_B_FLAT+12;
	scales[12] = new dfMidiScale("Half-diminished (C)",scale,7,getNumNotes());
	
	// select the first scale for starters
	model->scaleIndex = 0;
	model->numScales = numScales;
}
