#include "secondMovementApp.h"
#include "smModel.h"

int main() {

	//ofSetupOpenGL(1024,768,OF_WINDOW);
	ofSetupOpenGL(1024,768,OF_FULLSCREEN);
	
	smModel		*m		= new smModel(320,200,50,52);
	
	m->colorDepth		= 3;
	m->motionFrames		= 4;
	m->motionPaintThreshold = 20;
	m->hue				= 0.0f;
	m->hueSpeed			= 0.0012f;
	m->frameRate		= 15;
	m->playChromatic	= false;
	m->debug			= false;
	m->hudVisible		= false;
	
	//m->notemode			= falling; // falling | scrolling
	m->notemode			= scrolling;
	
	smView       *v = new smView(m);
	smController *c = new smController(m);
	
	// run with our mvc context
	ofRunApp(new secondMovementApp(m,v,c));
}
