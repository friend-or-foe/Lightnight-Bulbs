#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	dmx.connect("tty.usbserial-EN143965", NBULBS); // use the name
	//dmx.connect(0); // or use a number
	dmx.activateMk2(); //Un comment this if using dmxUSBPro mk2

	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].setup(i);
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].update();

		////*********send DMX Values***********////
		//When sending to Mk2 you have to set universe as well as channel and value
		//dmx.setLevel(channel, value, universe)
		int thisLightValue = myBulb[i].dmxLightVal;
		int thisID = myBulb[i].dmxID;
		dmx.setLevel(thisID, thisLightValue, 1);

		//uncomment to check outgoing DMX values
		//printf("dmxVal: %i\n", thisLightValue);
	}
	
	//update dmx values
	dmx.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofBackground(0);

	ofFill();
	ofSetColor(255, ofGetMouseY() / 5);
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ofGetMouseX() / 5, ofGetMouseX() / 5);

	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].draw();
	}
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
