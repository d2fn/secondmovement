/*
 *  smModel.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#ifndef SMMODEL
#define SMMODEL

#include "ofxOpenCv.h"
#include "dfNotes.h"
#include "smAudioDelegate.h"
#include <string.h>
using namespace std;

class smAudioDelegate;

enum visualNoteMode {
	scrolling,
	falling
};

class smModel {
	
public:
	
	smModel(int w, int h, int gridw, int gridh);
	void update();
	
	/**
	 * submit a new frame to be tracked
	 * causes the oldest frame to be overwritten
	 **/
	void submitFrame(unsigned char *frame);
	
	// go to the next scale in the sequence
	void nextScale();
	// go to the previous scale in the sequence
	void prevScale();
	
	// get the name of the scale at a given index
	// index must be >= 0 and < numScales
	string getScaleName(int index);
	
	// return the number of note in scale
	int getNumNotes();
	
	// advance the hue
	void advanceHue();
	
	/********************
	 * input parameters 
	 * treated as immutable once set
	 ********************/
	
	// current frame rate
	int frameRate;

	/**
	 * modeling of video capture parameters
	 **/
	int captureWidth,captureHeight;
	int colorDepth;
	
	/**
	 * modeling of motion detection grid parameters
	 **/
	int hres,vres;
	float blockWidth,blockHeight;
	
	/********************
	 * transient parameters
	 ********************/
	
	// whether or not the heads up display should be drawn
	bool hudVisible;
	
	// current hue
	float hue;
	// current hue translated into rgb space
	int r, g, b;
	// amount to advance the hue in each frame
	float hueSpeed;
	
	/**
	 * motion painting parameters
	 * does not affect motion detection or sound output!
	 **/
	// number of frames to reference to track motion for painting
	// - more = longer ghost trails
	int motionFrames;
	// motion threshold to pass before being painted
	// - higher = less noise painted but harsher edges
	// - lower = more noise, nicer edges
	int motionPaintThreshold;
	
	/**
	 * transient recording of where motion is being detected
	 **/
	// camera input buffers
	ofxCvColorImage colorFrame;
	ofxCvGrayscaleImage grayscaleFrame;
	//raw grid of camera input of the dimension hres * vres * 2
	// each cell contains an average of the pixels underneath
	unsigned char * valueGrid;
	// point to the current place in which to capture raw
	// 'de-resolutioned' cam input (2 frames so motion can be detected)
	unsigned char * valueGridPtr;
	// raw differencing of the two frames in valueGrid
	unsigned char * diffGrid;
	// clamping of the value grid to values of 0 or 1 based on motion
	// detect threshold
	unsigned char * clampGrid;
	
	// was motion detected?
	bool motionDetected;
	// hold the bounding box for motion last detected
	int motionRectX1;
	int motionRectX2;
	int motionRectY1;
	int motionRectY2;
	
	/**
	 * musical scale options
	 **/
	// the current scale in which to play notes
	int scaleIndex;
	int numScales;
	
	// set to true to play chromatically vice highest note mode
	bool playChromatic;
	
	// capture each note
	dfNotes * playedNotes;
	
	// how notes should be represented
	visualNoteMode notemode;
	
	// audio delegate
	smAudioDelegate * audioDelegate;
	
	// debug mode flag - when in debug mode, midi output should be disabled
	bool debug;
	
	unsigned char getValueAt(int x, int y);
	unsigned char getDiffAt(int x, int y);
	unsigned char getClampAt(int x, int y);
	
	// return the note index given a y coordinate in (hres,vres) space
	int getNoteIndex(int y);
	
	unsigned int hsb2rgb(float hue, float saturation, float brightness);
	
private:
	
	void calculateValueGrid(unsigned char * grid, unsigned char * pixels);
	void calculateDiffGrid(unsigned char * diff, unsigned char * grid1, unsigned char * grid2);
	void clamp(unsigned char * clamp, unsigned char * grid, char lo, char hi);
	void detectRegion(unsigned char * grid, int threshold);
};

#endif