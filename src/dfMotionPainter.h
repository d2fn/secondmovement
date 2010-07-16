/*
 *  dfMotionTracker.h
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 9/5/09.
 *  Copyright 2009 All rights reserved.
 *
 */


#ifndef DFMOTIONPAINTER
#define DFMOTIONPAINTER

#include "ofMain.h"

#define DF_MT_BYTES_PER_PIXEL 3

class dfMotionPainter {
		
public:
	dfMotionPainter(int _numFrames, int _frameWidth, int _frameHeight);
	~dfMotionPainter();

	/**
	 * submit a new frame to be tracked
	 * causes the oldest frame to be overwritten
	 **/
	void submitFrame(unsigned char *frame);
	
	/**
	 * update for motion detection
	 **/
	void drawMotion(int detectThreshold, int paintThreshold, bool paint, int r, int g, int b);
	
	
private:
	int numFrames;
	int frameWidth, frameHeight;
	int bytesPerFrame;
	int bytesInBuffer;
	
	unsigned char *	buffer;
	unsigned char *	bufferPtr;
	unsigned char * motion;
};

#endif
