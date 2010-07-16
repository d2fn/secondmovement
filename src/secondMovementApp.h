#ifndef _SECOND_MOVEMENT_APP
#define _SECOND_MOVEMENT_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "dfMidiNotes.h"
#include "dfMidiOut.h"
#include "dfMidiScale.h"
#include "dfMotionPainter.h"
#include "dfNotes.h"
#include "smModel.h"
#include "smView.h"
#include "smController.h"

class secondMovementApp : public ofBaseApp {
	
	public:
		secondMovementApp(smModel * m, smView * v, smController * c);
		~secondMovementApp();
	 
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
	
	private:
		// MVC support
		smModel	     * model;
		smView	     * view;
		smController * controller;
};

#endif
	
