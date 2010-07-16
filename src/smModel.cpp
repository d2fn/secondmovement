/*
 *  smModel.cpp
 *  ofSecondMovement
 *
 *  Created by Dietrich Featherston on Sun 2009-11-15.
 *  Copyright 2009 All rights reserved.
 *
 */

#include "smModel.h"

smModel::smModel(int w, int h, int gridw, int gridh) {
	
	this->captureWidth	= w;
	this->captureHeight	= h;
	this->hres			= gridw;
	this->vres			= gridh;
	
	// initialize the color image output from camera
	colorFrame.allocate(w,h);
	
	// initialize te grayscale image output from camera
	grayscaleFrame.allocate(w,h);
	
	// create buffer to store two frames
	valueGrid	= (unsigned char *)malloc(2*hres*vres*sizeof(char));
	valueGridPtr = valueGrid;
	
	// allocate memory for motion detection grid
	diffGrid	= (unsigned char *)malloc(gridw*gridh*sizeof(char));
	clampGrid	= (unsigned char *)malloc(gridw*gridh*sizeof(char));
	
	playedNotes = new dfNotes;
	
	audioDelegate = new smAudioDelegate(this);
	
	motionDetected = false;
}

void smModel::update() {
	
	// these next two lines only need to happen once but this
	// will happen once per redraw cycle but oh well...
	this->blockWidth	= (float)ofGetWidth()  / (float)hres;
	this->blockHeight	= (float)ofGetHeight() / (float)vres;
}

void smModel::submitFrame(unsigned char * frame) {
	
	colorFrame = frame;
	grayscaleFrame = colorFrame;
	
	// gen the low-res grid
	calculateValueGrid(valueGridPtr,frame);
	
	// ensure that we keep writing to the value grid within the correct memory space
	valueGridPtr += hres*vres;
	if( valueGridPtr == ( valueGrid + (hres*vres*2) ) ) {
		valueGridPtr = valueGrid;
	}
	
	calculateDiffGrid(
				diffGrid,
				valueGrid,
				valueGrid + this->hres * this->vres);
	
	clamp(clampGrid,diffGrid,10,10);
	
	// compute the mbr of any motion detected
	detectRegion(clampGrid,1);
}

int smModel::getNoteIndex(int y) {
	int noteIndex = vres - y - 1;
}

int smModel::getNumNotes() {
	return vres;
}

unsigned char smModel::getValueAt(int x, int y) {
	return valueGrid[y*hres+x];
}

unsigned char smModel::getDiffAt(int x, int y) {
	return diffGrid[y*hres+x];
}

unsigned char smModel::getClampAt(int x, int y) {
	return clampGrid[y*hres+x];
}

string smModel::getScaleName(int index) {
	return audioDelegate->getScaleName(index);
}

void smModel::nextScale() {
	scaleIndex++;
	scaleIndex %= numScales;
}

void smModel::prevScale() {
	if(scaleIndex == 0) {
		scaleIndex = numScales - 1;
	}
	else {
		scaleIndex--;
	}
}

void smModel::advanceHue() {
	// advance hue
	hue += hueSpeed;
	if(hue > 1.0) {
		hue = 0.0f;
	}
	int color = hsb2rgb(hue,1.0f,1.0f);
	r = (color >> 16) & 0xff;
	g = (color >>  8) & 0xff;
	b = (color >>  0) & 0xff;
}

void smModel::calculateValueGrid(unsigned char * grid, unsigned char * pixels) {
	
	// get the number of value bins
	int nvalues = hres*vres;
	
	// determine the dimensions of each block from the image that will be averaged
	float sampleWidth	= (float)captureWidth/(float)hres;
	float sampleHeight	= (float)captureHeight/(float)vres;
	int   nSamples		= sampleWidth * sampleHeight;
	
	long numPixels = captureWidth * captureHeight;
	long numBytes = numPixels * colorDepth;
	
	int startx = 0;
	int starty = 0;
	int stopx = sampleWidth;
	int stopy = sampleHeight;
	
	// loop to compute the average pixel value at each block
	for(int i = 0; i < nvalues; i++) {
		int blockx = i%hres;
		int blocky = i/hres;
		
		// determine dimensions and coordinates of the current block
		int startx = blockx * sampleWidth;
		int starty = blocky * sampleHeight;
		int stopx = startx + sampleWidth;
		int stopy = starty + sampleWidth;
		
		float blockValue = 0.f;
		
		for(int y = starty; y < stopy; y++) {
			for(int x = startx; x < stopx; x++) {
				unsigned char * p = pixels + ((captureWidth * y + x) * colorDepth);
				
				// take average of rgb channels
				float graytone = 0.f;
				for(int j = 0; j < colorDepth; j++) {
					graytone += (float)(*p++)/(float)colorDepth;
				}
				
				blockValue += graytone/(float)nSamples;
			}
		}
		grid[i] = (int)blockValue;
	}
}

void smModel::calculateDiffGrid(unsigned char * diff, unsigned char * grid1, unsigned char * grid2) {
	for(int i = 0; i < hres*vres; i++) {
		diff[i] = abs(grid1[i] - grid2[i]);
	}
}

void smModel::clamp(unsigned char * clamp, unsigned char * grid, char lo, char hi) {
	for(int i = 0; i < hres*vres; i++) {
		if(grid[i] <= lo) clamp[i] = 0;
		if(grid[i] >= hi) clamp[i] = 255;
	}
}

void smModel::detectRegion(unsigned char * grid, int threshold) {
	
	motionRectX1 = captureWidth;
	motionRectX2 = 0;
	motionRectY1 = captureHeight;
	motionRectY2 = 0;
	
	motionDetected = false;
	
	for(int y = 0; y < vres; y++) {
		for(int x = 0; x < hres; x++) {
			int i = y*hres+x;
			if(grid[i] >= threshold) {
				if(motionRectX1 > x) {
					motionRectX1 = x;
				}
				if(motionRectX2 < x) {
					motionRectX2 = x;
				}
				if(motionRectY1 > y) {
					motionRectY1 = y;
				}
				if(motionRectY2 < y) {
					motionRectY2 = y;
				}
			}
		}
	}
	
	if(motionRectX1 < motionRectX2 && motionRectY1 < motionRectY2) {
		motionDetected = true;
	}
}

unsigned int smModel::hsb2rgb(float hue, float saturation, float brightness) {
	
	int r = 0, g = 0, b = 0;
	
	if (saturation == 0) {
	    r = g = b = (int) (brightness * 255.0f + 0.5f);
	}
	else {
	    float h = (hue - (float)floor(hue)) * 6.0f;
	    float f = h - (float)floor(h);
	    float p = brightness * (1.0f - saturation);
	    float q = brightness * (1.0f - saturation * f);
	    float t = brightness * (1.0f - (saturation * (1.0f - f)));
	    switch ((int) h) {
			case 0:
				r = (int) (brightness * 255.0f + 0.5f);
				g = (int) (t * 255.0f + 0.5f);
				b = (int) (p * 255.0f + 0.5f);
				break;
			case 1:
				r = (int) (q * 255.0f + 0.5f);
				g = (int) (brightness * 255.0f + 0.5f);
				b = (int) (p * 255.0f + 0.5f);
				break;
			case 2:
				r = (int) (p * 255.0f + 0.5f);
				g = (int) (brightness * 255.0f + 0.5f);
				b = (int) (t * 255.0f + 0.5f);
				break;
			case 3:
				r = (int) (p * 255.0f + 0.5f);
				g = (int) (q * 255.0f + 0.5f);
				b = (int) (brightness * 255.0f + 0.5f);
				break;
			case 4:
				r = (int) (t * 255.0f + 0.5f);
				g = (int) (p * 255.0f + 0.5f);
				b = (int) (brightness * 255.0f + 0.5f);
				break;
			case 5:
				r = (int) (brightness * 255.0f + 0.5f);
				g = (int) (p * 255.0f + 0.5f);
				b = (int) (q * 255.0f + 0.5f);
				break;
	    }
	}
	return 0xff000000 | (r << 16) | (g << 8) | (b << 0);
}
