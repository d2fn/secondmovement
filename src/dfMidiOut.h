/*
 *  dfMidiOut.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 9/7/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef _DF_MIDI_OUT
#define _DF_MIDI_OUT

#include <CoreMIDI/CoreMIDI.h>
#include <CoreAudio/CoreAudio.h>
#include <CoreServices/CoreServices.h> //for file stuff
#include <Carbon/Carbon.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioUnit/AudioUnitProperties.h>
#include <CoreAudio/CoreAudioTypes.h>
#include <AudioToolbox/AudioToolbox.h> //for AUGraph

#include <sys/types.h>
#include <time.h>
#include <iostream.h>

class dfMidiOut {

public:
	dfMidiOut();
	void sendNoteOn(unsigned char channel, unsigned char note, unsigned char velocity, bool debug);
	
private:
	AUGraph graph;
	AudioUnit synthUnit;

	// on bits for currently playing notes
	unsigned int * playing;
	
	// holds the systain of each note so we can tell how long it's been playing
	time_t * sustain;
	
	void cleanupNotes(unsigned char channel, unsigned char velocity, bool debug);
	void printNoteTable();
};

// some MIDI constants:
enum {
	kMidiMessage_ControlChange 		= 0xB,
	kMidiMessage_ProgramChange 		= 0xC,
	kMidiMessage_BankMSBControl 	= 0,
	kMidiMessage_BankLSBControl		= 32,
	kMidiMessage_NoteOn 			= 0x9,
	kMidiMessage_NoteOff            = 0x8
};

OSStatus CreateAUGraph(AUGraph &outGraph, AudioUnit &outSynth);

#endif
