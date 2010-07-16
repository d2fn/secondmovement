/*
 *  dfMidiOut.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 9/7/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "dfMidiOut.h"

void printbits(unsigned int bits) {
	
	unsigned int mask = 1 << 31;
	
	for(int i = 0; i < 32; i++) {
		int test = mask & bits;
		if(test == mask) {
			cout << "1";
		}
		else {
			cout << "0";
		}
		mask >>= 1;
	}
}

dfMidiOut::dfMidiOut() {
	
	CreateAUGraph(graph, synthUnit);
	AUGraphInitialize(graph);
	
	MusicDeviceMIDIEvent(synthUnit, 
						 kMidiMessage_ControlChange << 4 | 1, 
						 kMidiMessage_BankMSBControl, 0,
						 0/*sample offset*/);
	
	MusicDeviceMIDIEvent(synthUnit, 
						 kMidiMessage_ProgramChange << 4 | 1, 
						 0/*prog change num*/, 0,
						 0/*sample offset*/);
	 
	AUGraphStart(graph);
	
	// support 128 possible notes, one bit per note
	playing = (unsigned int *)malloc(sizeof(unsigned int)*4);
	playing[0] = 0;
	playing[1] = 0;
	playing[2] = 0;
	playing[3] = 0;
	
	sustain = (time_t *)malloc(sizeof(time_t)*128);
}

void dfMidiOut::printNoteTable() {
	
	time_t t;
	time(&t);
	
	cout << "Note Age Table:" << endl;
	
	for(int note = 12; note <= 99; note++) {
		int r = note % 32;
		int i = note / 32;
		unsigned int mask = 1 << r;
		
		cout << "mask              : ";
			printbits(mask);
			cout << endl;
		
		cout << "playing[i]        : ";
			printbits(playing[i]);
			cout << endl;
		
		cout << "playing[i] & mask : ";
			printbits(playing[i] & mask);
			cout << endl;
		
		if((unsigned int)(playing[i] & mask) == mask) {
			cout << note << ", age : " << (t-sustain[note]) << endl;
		}
		else {
			cout << note << " off" << endl;
		}
	}
	
	cout << endl;
}

void dfMidiOut::cleanupNotes(unsigned char channel, unsigned char velocity, bool debug) {
	
	if(debug) {
		cout << "enter dfMidiOut::cleanupNotes()" << endl;
		printNoteTable();
	}
	
	// capture the current time
	time_t t;
	time(&t);
	
	// cleanup code to turn off notes that haven't been played in n seconds
	for(int note = 0; note < 128; note++) {
		int r		= note % 32;
		int i		= note / 32;
		unsigned int mask = 1 << r;
		if((unsigned int)(playing[i] & mask) == mask) {
			
			time_t notesus = t - sustain[note];
			
			if(notesus >= 5) {
				
				if(!debug) {
					MusicDeviceMIDIEvent(synthUnit, (kMidiMessage_NoteOff<<4) | channel, note, velocity, 0);
				}
				
				// turn note off
				playing[i] &= ~mask;
				
				if(debug) {
					cout << "note              : " << note << endl;
					cout << "mask              : ";
						printbits(mask);
						cout << endl;
					
					cout << "turning off note " << note << ", idle for " << notesus << " seconds" << endl;
					cout << "playing[i] before : ";
						printbits(playing[i] | mask);
						cout << endl;
					
					cout << "playing[i] after  : ";
						printbits(playing[i]);
						cout << endl;
				}
			}
		}
	}
	
	if(debug) {
		cout << "exit dfMidiOut::cleanupNotes()" << endl;
	}
}

void dfMidiOut::sendNoteOn(unsigned char channel, unsigned char note, unsigned char velocity, bool debug) {
	
	if(debug) {
		cout << "enter dfMidiOut::sendNoteOn" << endl;
	}
	
	// capture the current time
	time_t t;
	time(&t);
	
	// this is bad-bad... need to have separate banks for channel and velocity combos i think???
	cleanupNotes(channel,velocity,debug);
	
	// check the on bit for this tone to see if we have an outstanding note_on message
	int r		= note % 32; // which bit to check
	int i		= note / 32; // which bucket to check
	unsigned int mask	= 1 << r;
	
	if(debug) {
		cout << "note : " << (int)note << endl;
		cout << "r    : " << r << endl;
		cout << "i    : " << i << endl;
		
		cout << "playing[i]        : ";
			printbits(playing[i]);
			cout << endl;
		
		cout << "mask              : ";
			printbits(mask);
			cout << endl;
		
		cout << "playing[i] & mask : ";
			printbits(playing[i] & mask);
			cout << endl;
	}
	
	if((unsigned int)(playing[i] & mask)  == mask) {
		
		time_t notesus = t - sustain[note];
		
		if(debug) {
			cout << "note previously played, see how long ago" << endl;
			cout << "sustain[note]   : " << sustain[note] << endl;
			cout << "notesus         : " << notesus << endl;
		}
		
		// has note been playing for more than 2 sec?
		if(notesus >= 1) {
		
			if(debug) {
				cout << "note played more than 2 sec ago, send note off msg followed by note on msg" << endl;
			}
			
			if(!debug) {
				// send a note_off message
				MusicDeviceMIDIEvent(synthUnit, (kMidiMessage_NoteOff<<4) | channel, note, velocity, 0);
			}
			
			// reset the on bit for this note
			playing[i] &= ~mask;
			
			if(debug) {
				cout << "playing[i] & ~mask : ";
				printbits(playing[i]);
				cout << endl;
			}
			
			if(!debug) {
				// play the note
				MusicDeviceMIDIEvent(synthUnit, (kMidiMessage_NoteOn<<4) | channel, note, velocity, 0);
			}
			
			// set the on bit for this note
			playing[i] |= mask;
			
			if(debug) {
				cout << "playing[i] | mask  : ";
				printbits(playing[i]);
				cout << endl;			
			}
			
			// record the time the note was played
			sustain[note] = t;
		}
		else {
			// ignore requests to play the same note so close together
			if(debug) {
				cout << "ignoring request to play note " << (int)note << endl;
			}
		}
	}
	else {
		
		if(debug) {
			cout << "note is off, play as normal and flip its on bit" << endl;
		}
		
		if(!debug) {
			// note is not playing, go ahead and play it
			MusicDeviceMIDIEvent(synthUnit, (kMidiMessage_NoteOn<<4) | channel, note, velocity, 0);
		}
		
		// set the on bit for this note
		playing[i] |= mask;
		// record the time the note was played
		sustain[note] = t;
		
		if(debug) {
			cout << "playing[i] | mask : ";
			printbits(playing[i]);
			cout << endl;
			cout << "sustain[note]     : " << sustain[note] << endl;
		}
	}
	
	if(debug) {
		cout << "exit dfMidiOut::sendNoteOn" << endl << endl;
	}
	
//	Byte data[3];
//	
//	//data[0] = (Byte)( midiType | ((channel - 1) << 4));
//	data[0] = (Byte)( midiType + channel);
//	data[1] = (Byte) note;
//	data[2] = (Byte) velocity;
//	
//	MIDIPacketList pktlist;
//	MIDIPacket* packet = MIDIPacketListInit(&pktlist);
//	packet = MIDIPacketListAdd( &pktlist, sizeof(MIDIPacketList), packet, AudioGetCurrentHostTime(), 3, data);
//	
//	MIDIReceived(_source, &pktlist);
}

OSStatus	CreateAUGraph (AUGraph &outGraph, AudioUnit &outSynth)
{
	OSStatus result;
	//create the nodes of the graph
	AUNode synthNode, limiterNode, outNode;
	
	AudioComponentDescription cd;
	cd.componentManufacturer = kAudioUnitManufacturer_Apple;
	cd.componentFlags = 0;
	cd.componentFlagsMask = 0;
	
	require_noerr (result = NewAUGraph (&outGraph), home);
	
	cd.componentType = kAudioUnitType_MusicDevice;
	cd.componentSubType = kAudioUnitSubType_DLSSynth;
	
	require_noerr (result = AUGraphAddNode (outGraph, &cd, &synthNode), home);
	
	cd.componentType = kAudioUnitType_Effect;
	cd.componentSubType = kAudioUnitSubType_PeakLimiter;  
	
	require_noerr (result = AUGraphAddNode (outGraph, &cd, &limiterNode), home);
	
	cd.componentType = kAudioUnitType_Output;
	cd.componentSubType = kAudioUnitSubType_DefaultOutput;  
	require_noerr (result = AUGraphAddNode (outGraph, &cd, &outNode), home);
	
	require_noerr (result = AUGraphOpen (outGraph), home);
	
	require_noerr (result = AUGraphConnectNodeInput (outGraph, synthNode, 0, limiterNode, 0), home);
	require_noerr (result = AUGraphConnectNodeInput (outGraph, limiterNode, 0, outNode, 0), home);
	
	// ok we're good to go - get the Synth Unit...
	require_noerr (result = AUGraphNodeInfo(outGraph, synthNode, 0, &outSynth), home);
	
home:
	return result;
}
