#include "bulbClass.h"



Bulb::Bulb()
{
}


void Bulb::setup(int _ID, int _planWidth, int _planHeight, int _offX, int _offY, float _bulbSize) {

	dmxID = _ID + 1;

	//Uncomment line below to print out DMX ID
	//printf("ID: %i\n", dmxID);

	x = ofRandom(_offX, _offX + _planWidth);
	y = ofRandom(_offY, _offY + _planHeight);

	difx = 0;
	dify = 0;

	bOver = false;
	locked = false;

	dim = _bulbSize;

	color.set(255, 255, 255);

	txtOffsetX = dim + 5;
	txtOffsetY = 5;
}

void Bulb::setLoc(int _x, int _y) {

	x = _x;
	y = _y;

}


void Bulb::update(float _bulbSize) {

	if (dim != _bulbSize) {
		dim = _bulbSize;
		txtOffsetX = dim + 5;
	}
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

void Bulb::draw(int _tmpCol) {

	//************** GET PIXEL COLOUR UNDER OBJECT (NOT NEEDED IF USING SCREENSHOT APPROACH)**************//
	//glReadPixels(x, ofGetHeight() - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, colorGet);
	//printf("pixels[0]: %i\n", colorGet[0]);// does output to the console  

	
	//switch on fill and set colour to show brightness
	dmxLightVal = _tmpCol; // colorGet[0];
	ofFill();
	ofSetColor(dmxLightVal, 0, 0);
	ofDrawCircle(x, y, dim);

	//draw outline and text
	ofNoFill();
	ofSetColor(color);
	ofSetLineWidth(3);
	ofDrawCircle(x, y, dim);

	ofDrawBitmapString(dmxID, x + txtOffsetX, y + txtOffsetY);

	////*********send DMX Values***********////
	//dmx.setLevel(ID, chan1);

	//dmx.update();
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
