/*
 *  dfMotionTracker.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on 9/5/09.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "dfMotionPainter.h"

dfMotionPainter::dfMotionPainter(int _numFrames, int _frameWidth, int _frameHeight) {
	
	numFrames = _numFrames;
	frameWidth = _frameWidth;
	frameHeight = _frameHeight;
	
	bytesPerFrame = frameWidth * frameHeight * DF_MT_BYTES_PER_PIXEL;
	bytesInBuffer = bytesPerFrame * numFrames;
	
	buffer = (unsigned char *)malloc(bytesInBuffer);
	bufferPtr = buffer;
}

dfMotionPainter::~dfMotionPainter() {
	free(buffer);
}

void dfMotionPainter::submitFrame(unsigned char *newFrame) {
	
	for(int i  = 0; i < bytesPerFrame; i += DF_MT_BYTES_PER_PIXEL) {
		
		char avg = 0;
		int c = i;
		
		avg += newFrame[c++]/3;
		avg += newFrame[c++]/3;
		avg += newFrame[c++]/3;
		
		*bufferPtr++ = avg;
		*bufferPtr++ = avg;
		*bufferPtr++ = avg;
	}
	
	if(bufferPtr == (buffer + bytesInBuffer)) {
		bufferPtr = buffer;
	}
}

void dfMotionPainter::drawMotion(int detectThreshold, int paintThreshold, bool paint, int r, int g, int b) {
	
	// save previous style
	ofPushStyle();
	
	ofEnableAlphaBlending();
	ofFill();
	
	float xscale = (float)ofGetWidth()/(float)frameWidth;
	float yscale = (float)ofGetHeight()/(float)frameHeight;
	
	int minx = frameWidth;
	int maxx = 0;
	int miny = frameHeight;
	int maxy = 0;
	
	bool motionDetected = false;
	
	// visit each pixel index for each frame
	// jump 3 bytes at a time so we always land on the next red component
	for(int y = 0; y < frameHeight; y++) {
		for(int x = 0; x < frameWidth; x++) {
		
			int p = (y*frameWidth+x)*DF_MT_BYTES_PER_PIXEL;
				
			int rdiff = 0;
			int gdiff = 0;
			int bdiff = 0;
			
			for(int i = 1; i < numFrames; i++) {
				
				// point to the bytes comprising the next pixel
				unsigned char *pixel1 = buffer+p+(i*bytesPerFrame);
				unsigned char *pixel2 = buffer+p+((i-1)*bytesPerFrame);
				
				rdiff += abs( *pixel1++ - *pixel2++ );
				gdiff += abs( *pixel1++ - *pixel2++ );
				bdiff += abs( *pixel1   - *pixel2   );
				
			}
			
			int a = ((float)rdiff+bdiff+gdiff)/(float)numFrames;
			
			if(paint && a > paintThreshold) {
				//int r = (color >> 16) & 0xff;
				//int g = (color >>  8) & 0xff;
				//int b = (color >>  0) & 0xff;
				
				ofSetColor(r,g,b,a/6);
				
				ofRect((x-1)*xscale,(y-1)*yscale,xscale,yscale);
				
				ofSetColor(r,g,b,a/3);
				
				ofRect(x*xscale,(y-1)*yscale,xscale,yscale*3);
				ofRect((x-1)*xscale,y*yscale,xscale*3,yscale);
				
				/*
				ofRect(x*xscale,y*yscale,xscale,yscale);//1,1);
				ofSetColor(r,g,b,a/2);
				ofRect((x-1)*xscale,(y)*yscale,xscale,yscale);
				ofRect((x+1)*xscale,(y)*yscale,xscale,yscale);
				ofRect((x)*xscale,(y-1)*yscale,xscale,yscale);
				ofRect((x)*xscale,(y+1)*yscale,xscale,yscale);
				ofSetColor(r,g,b,a/4);
				ofRect((x-1)*xscale,(y-1)*yscale,xscale,yscale);
				ofRect((x-1)*xscale,(y+1)*yscale,xscale,yscale);
				ofRect((x+1)*xscale,(y-1)*yscale,xscale,yscale);
				ofRect((x+1)*xscale,(y+1)*yscale,xscale,yscale);
				 */
			}
			
			if(a > detectThreshold) {
				if(x > maxx) {
					maxx = x;
				}
				if(x < minx) {
					minx = x;
				}
				if(y > maxy) {
					maxy = y;
				}
				if(y < miny) {
					miny = y;
				}
				motionDetected = true;
			}
		}
	}
	
	/**
	if(motionDetected) {
		if(paint) {
			ofNoFill();
			ofRect(minx,miny,maxx-minx,maxy-miny);
		}
	}
	else {
	}
	 **/
	
	// restore prevous style
	ofPopStyle();
}






