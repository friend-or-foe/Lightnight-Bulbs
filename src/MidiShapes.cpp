#include "MidiShapes.h"



MidiShapes::MidiShapes()
{
}


MidiShapes::~MidiShapes()
{
}

void MidiShapes::setup(int _ID, float _x, float _y) {

	ID = _ID;
	x = _x;
	y = _y;


}

void MidiShapes::update() {


}

void MidiShapes::draw(float _fadeSpeed) {

	ofSetColor(255, bright);

	if (ID < 10) {
		drawPlink(_fadeSpeed);
	}

	if ((ID >= 10) && (ID < 19)){
		drawBass(_fadeSpeed);
	}

	if (ID >= 19) {
		ofNoFill();
		ofSetLineWidth(30);
		drawKey(_fadeSpeed);
		ofSetLineWidth(1);
	}

}

void MidiShapes::drawPlink(float _fadeSpeed){
	
	ofDrawRectangle(x, y, width, height);
	
	//fade out
	if (bright > 0) {
		bright -= _fadeSpeed;
	}

}

void MidiShapes::drawBass(float _fadeSpeed) {

	ofPushMatrix();
	ofTranslate(x, y);
	ofRotate(angle);
	ofDrawRectangle(0, 0, width, height);
	ofPopMatrix();

	//fade out
	if (bright > 0) {
		bright -= _fadeSpeed;
	}

	angle += rotSpeed;

}

void MidiShapes::drawKey(float _fadeSpeed) {

	
	ofDrawEllipse(x, y, width, width);

	//fade out
	if (bright > 0) {
		bright -= _fadeSpeed;
	}
}

void MidiShapes::initPlink(int _ID, float _x, float _y, float _initBright) {

	ID = _ID;
	x = _x;
	y = _y;
	bright = _initBright;

	width = ofRandom(200, 300);
	height = ofRandom(200, 300);

}

void MidiShapes::initBass(int _ID, float _x, float _y, float _initBright) {
	
	ID = _ID;
	x = _x;
	y = _y;
	bright = _initBright;

	width = ofRandom(50, 100);
	height = 500;// ofRandom(400, 100);

	angle = ofRandom(360);
	rotSpeed = ofRandom(-10, 10);

}

void MidiShapes::initKey(int _ID, float _x, float _y, float _initBright) {

	ID = _ID;
	x = _x;
	y = _y;
	bright = _initBright;

	width = ID * 10;
}