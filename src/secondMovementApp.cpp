#include "secondMovementApp.h"

secondMovementApp::secondMovementApp(smModel * m, smView * v, smController * c) {
	model		= m;
	view		= v;
	controller	= c;
}

secondMovementApp::~secondMovementApp() { }

// init
void secondMovementApp::setup() {
	ofSetFrameRate(model->frameRate);
}

// update model between renderings
void secondMovementApp::update() {
	controller->update();
}

// render model
void secondMovementApp::draw() {
	view->draw();
}

void secondMovementApp::keyPressed(int key) {
	controller->keyPressed(key);
}

void secondMovementApp::keyReleased(int key) {
	controller->keyReleased(key);
}

void secondMovementApp::mouseMoved(int x, int y ) {
	controller->mouseMoved(x,y);
}

void secondMovementApp::mouseDragged(int x, int y, int button) {
	controller->mouseDragged(x,y,button);
}

void secondMovementApp::mousePressed(int x, int y, int button) {
	controller->mousePressed(x,y,button);
}

void secondMovementApp::mouseReleased() {
	controller->mouseReleased();
}
