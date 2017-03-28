#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//dmx.connect("tty.usbserial-EN143965", NBULBS); // use the name
	dmx.connect(0, NBULBS); // or use a number
	dmx.activateMk2(); //Un comment this if using dmxUSBPro mk2

	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].setup(i);
	}

	ofSetFrameRate(200);

}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].update();
		
	}
	

}

//--------------------------------------------------------------
void ofApp::draw() {

	//ofBackground(0);

	ofFill(); //set fill back on
	ofSetColor(0);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight()); //draw rectanle to wipe previous frame

	
	ofSetColor(255, ofGetMouseY() / 4); //set transperency of test circle
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ofGetMouseX() / 5, ofGetMouseX() / 5);

	//grab screenshot  before bulbs are drawn
	tmpImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

	//tmpImage.save("screenshot.jpg");
	for (int i = 0; i<NBULBS; i++) {
		ofColor tmpCol = tmpImage.getColor(myBulb[i].x, myBulb[i].y); //get pixel colour for object
		///uncomment to check colour being sent to bulb object
		///printf("myBulb - colour: %i\n", tmpCol.r);

		myBulb[i].draw(tmpCol.r); //call draw sending colour value to object.

		////*********send DMX Values***********////
		//When sending to Mk2 you have to set universe as well as channel and value
		//dmx.setLevel(channel, value, universe)
		int thisLightValue = myBulb[i].dmxLightVal;
		int thisID = myBulb[i].dmxID;
		dmx.setLevel(thisID, thisLightValue, 2);

		///uncomment to check outgoing DMX values
		///printf("dmxVal: %i\n", thisLightValue);
	}

	//***************UNCOMMENT TO SEND SIGNALS WHEN CONNECTED TO USBPRO*****************//
	//update dmx values
	//dmx.update();

	//draw framerate to window
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].mouseDragged();
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].mousePressed();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].mouseReleased();
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
