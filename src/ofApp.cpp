#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//dmx.connect("tty.usbserial-EN143965", NBULBS); // use the name
	dmx.connect(0, NBULBS); // or use a number
	dmx.activateMk2(); //Un comment this if using dmxUSBPro mk2

	////// LOAD PLAN IMAGE AND SET OFFSETS FOR COLOUR GRABBING ////////
	plan.load("SGH - Catacomb Plans - 1400.jpg");

	//set colour grab offsets (Location to draw plan to)
	planOffsetX = 470; // plan.getWidth();
	planOffsetY = 50; // plan.getHeight();

	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].setup(i, planOffsetX, planOffsetY, bulbSize);
	}

	ofSetFrameRate(200);

	///--------- GUI STUFF --------------//
	gui.setup();
	gui.add(bulbSize.set("bulb size", bulbSize, 0.1, 40.0));
	gui.add(drawPlan.set("draw plan", false));


}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].update(bulbSize);
		
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
	tmpImage.grabScreen(planOffsetX, planOffsetY, 1400, 674);

	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	} else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(470, 50, 1400, 674);
	}

	//tmpImage.save("screenshot.jpg");
	for (int i = 0; i < NBULBS; i++) {
		if (myBulb[i].x - planOffsetX > 0 && myBulb[i].x - planOffsetX < 1400
			&& myBulb[i].y - planOffsetY > 0 && myBulb[i].y - planOffsetY < 674) {
		tmpCol = tmpImage.getColor(myBulb[i].x - planOffsetX, myBulb[i].y - planOffsetY); //get pixel colour for object
		///uncomment to check colour being sent to bulb object
		///printf("myBulb - colour: %i\n", tmpCol.r);
		} else {
			tmpCol = (0, 0, 0);
		}

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

	//DRAW GUI
	gui.draw();
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
