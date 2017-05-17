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
		if(bright> 0) { ofSetColor(255, bright + ofRandom(-bright, bright/4)); }
		drawBass(_fadeSpeed);
	}

	if (ID >= 19) {
		ofNoFill();
		ofSetLineWidth(10);
		drawKey(_fadeSpeed);
		ofSetLineWidth(1);
	}

}

void MidiShapes::drawPlink(float _fadeSpeed){

	ofSetColor(255, bright);

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

	for (int i = 0; i < 10; i++){
	ofDrawEllipse(x, y, width+(10*i), width + (10 * i));
	}

	//fade out
	if (bright > 0) {
		bright -= _fadeSpeed;
	}
}

void MidiShapes::drawRussPlink(float _fadeSpeed) {

	ofSetColor(255, bright);


	ofDrawRectangle(x, y, 10, 10);


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

	width = ofRandom(100, 200);
	height = ofRandom(100, 200);

}

void MidiShapes::initBass(int _ID, float _x, float _y, float _initBright) {
	
	ID = _ID;
	x = _x;
	y = _y;
	bright = _initBright;

	width = ofRandom(100, 200);
	height = 500;// ofRandom(400, 100);

	angle = ofRandom(360);
	rotSpeed = ofRandom(-10, 10);

}

void MidiShapes::initKey(int _ID, float _x, float _y, float _initBright) {

	ID = _ID;
	x = _x;
	y = _y;
	bright = _initBright;

	width = 30 + ((ID-19) * 100);
}

void MidiShapes::initRussPlink(int _ID, float _x, float _y, float _initBright) {

	ID = _ID;
	x = _x;
	y = _y;
	bright = _initBright;

}