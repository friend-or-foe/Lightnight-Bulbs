#include "bulbClass.h"



Bulb::Bulb()
{
}


void Bulb::setup(int _ID) {

	dmxID = _ID + 1;

	//Uncomment line below to print out DMX ID
	//printf("ID: %i\n", dmxID);

	x = ofRandom(0, ofGetWidth());
	y = ofRandom(0, ofGetHeight());

	difx = 0;
	dify = 0;

	bOver = false;
	locked = false;

	dim = 20;

	color.set(255, 255, 255);

	txtOffsetX = dim + 5;
	txtOffsetY = 5;
}


void Bulb::update() {

	// Test if the cursor is over the box 

	if (ofGetMouseX() > x - dim && ofGetMouseX() < x + dim &&

		ofGetMouseY() > y - dim && ofGetMouseY() < y + dim) {

		bOver = true;

		if (!locked) {
			color.set(200, 100, 0);
		}

	}
	else {
		color.set(100);

		bOver = false;

	}



}

void Bulb::draw() {

	//************** GET PIXEL COLOUR UNDER OBJECT **************//
	glReadPixels(x, ofGetHeight() - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, colorGet);
	//printf("pixels[0]: %i\n", colorGet[0]);// does output to the console  

	//switch on fill and set colour to show brightness
	ofFill();
	ofSetColor(colorGet[0], 0, 0);
	ofDrawCircle(x, y, dim);

	//draw outline and text
	ofNoFill();
	ofSetColor(color);
	ofSetLineWidth(3);
	ofDrawCircle(x, y, dim);

	ofDrawBitmapString(dmxID, x + txtOffsetX, y + txtOffsetY);
}

void Bulb::mousePressed() {
	if (bOver) {

		locked = true;

		color.set(255, 255, 255);

	}
	else {

		locked = false;

	}

	difx = ofGetMouseX() - x;

	dify = ofGetMouseY() - y;

}

void Bulb::mouseDragged() {

	if (locked) {

		x = ofGetMouseX() - difx;
		y = ofGetMouseY() - dify;

	}



}

void Bulb::mouseReleased() {

	locked = false;

}
