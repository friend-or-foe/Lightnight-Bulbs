#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//dmx.connect("tty.usbserial-EN143965", NBULBS); // use the name
	dmx.connect(0, NBULBS); // or use a number
	dmx.activateMk2(); //Un comment this if using dmxUSBPro mk2

	////// LOAD PLAN IMAGE AND SET OFFSETS FOR COLOUR GRABBING ////////
	plan.load("SGH - Catacomb Plans - 1400.jpg");

	//return the width + height of image
	planWidth = plan.getWidth();
	planHeight = plan.getHeight();

	//printf("planWidth: %i\n", planWidth);

	//set colour grab offsets (Location to draw plan to)
	planOffsetX = 470; // plan.getWidth();
	planOffsetY = 50; // plan.getHeight();

	//create bulb objects
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].setup(i, planWidth, planHeight, planOffsetX, planOffsetY, bulbSize);
	}



	///--------- GUI STUFF --------------//
	loadBulbLocations.addListener(this, &ofApp::loadButtonPressed);
	saveBulbLocations.addListener(this, &ofApp::saveButtonPressed);

	gui.setup();
	gui.add(masterBrightness.set("master brightness", masterBrightness, 0.0, 255));
	gui.add(bulbSize.set("bulb size", bulbSize, 0.1, 40.0));
	gui.add(drawPlan.set("draw plan", false));
	gui.add(loadBulbLocations.setup("load bulb locations"));
	gui.add(saveBulbLocations.setup("save bulb locations"));

	///--------- FFT STUFF --------------//
	initAudio();

	ofSetFrameRate(200);
}

void ofApp::initAudio() {
	
	int channelsOut = 0;        // number of requested output channels (i.e. 2 for stereo).
	int channelsIn = 2;         // number of requested input channels.
	int sampleRate = 44100;     // requested sample rate (44100 is typical).
	int bufferSize = 1024;       // requested buffer size (256 is typical).
	int numOfBuffers = 4;       // number of buffers to queue, less buffers will be more responsive, but less stable.

	soundStream.setup(this, channelsOut, channelsIn, sampleRate, bufferSize, numOfBuffers);

	samplesChannelL.assign(bufferSize, 0.0);
	samplesChannelR.assign(bufferSize, 0.0);

	fftSmooth = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		fftSmooth[i] = 0;
	}


}

void ofApp::drawSamples(vector<float> samples) {

	int sampleWidth = ofGetWidth() / samples.size();
	int sampleHeight = ofGetHeight() / 2;
	int numOfSamples = samples.size();

	for (int i = 0; i<numOfSamples; i+=10) {
		int x = ofMap(i, 0, numOfSamples - 1, 0, ofGetWidth() - sampleWidth);
		int y = 0;

		//smoothing for fft values
		float value = samples[i] * sampleScale;// *= 0.993f;
		fftSmooth[i] *= 0.98f;
		if (fftSmooth[i] < value) {
			fftSmooth[i] = value;
		}

		int w = 1 + (-fftSmooth[i]);
		//int h = -samples[i] * sampleHeight;

		//ofDrawEllipse(x, y, w, w);
		ofDrawEllipse(planOffsetX + 700, y, w, w);
	}

}

//--------------------------------------------------------------

void ofApp::audioIn(float * input, int bufferSize, int nChannels) {

	for (int i = 0; i<bufferSize; i++) {
		samplesChannelL[i] = input[i * 2 + 0];
		samplesChannelR[i] = input[i * 2 + 1];
	}

	float * dataL = &samplesChannelL[0];
	float * dataR = &samplesChannelL[0];

	fftChannelL.audioIn(dataL);
	fftChannelR.audioIn(dataR);
}

//--------------------------------------------------------------

void ofApp::update() {

	fftChannelL.update();
	fftChannelR.update();


	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].update(bulbSize);
		
	}
	

}

void ofApp::drawFFT() {

	ofSetColor(255, 20);
	ofPushMatrix();
	ofTranslate(0, 362);
	drawSamples(fftChannelL.getFftNormData());
	ofPopMatrix();

	//uncomment to draw right channel
	/*
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() * 0.75);
	drawSamples(fftChannelR.getFftNormData());
	ofPopMatrix();
	*/
}
//--------------------------------------------------------------
void ofApp::draw() {

	//ofBackground(0);

	ofFill(); //set fill back on
	ofSetColor(0);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight()); //draw rectanle to wipe previous frame

	
	//ofSetColor(255, ofGetMouseY() / 4); //set transperency of test circle
	//ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ofGetMouseX() / 5, ofGetMouseX() / 5);

	///**** PICK SCENE ****///
	switch (myScene)
	{
	case SCENE_1:
		mainScene_1();
			break;
	case SCENE_2:
		
			break;
	case SCENE_3:
			break;
	}

	//***************UNCOMMENT TO SEND SIGNALS WHEN CONNECTED TO USBPRO*****************//
	//update dmx values
	//dmx.update();

	//draw framerate to window
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), 10, 20);

	//DRAW GUI
	gui.draw();
}

void ofApp::mainScene_1() {

	///----------- DRAW FFT SHAPES ---------------//
	drawFFT();

	//grab screenshot  before bulbs are drawn
	tmpImage.grabScreen(planOffsetX, planOffsetY, planWidth, planHeight);

	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	}
	else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(470, 50, 1400, 674);
	}

	for (int i = 0; i < NBULBS; i++) {
		if (myBulb[i].x - planOffsetX > 0 && myBulb[i].x - planOffsetX < planWidth
			&& myBulb[i].y - planOffsetY > 0 && myBulb[i].y - planOffsetY < planHeight) {
			tmpCol = tmpImage.getColor(myBulb[i].x - planOffsetX, myBulb[i].y - planOffsetY); //get pixel colour for object
																							  ///uncomment to check colour being sent to bulb object
																							  ///printf("myBulb - colour: %i\n", tmpCol.r);
		}
		else {
			tmpCol = (0, 0, 0);
		}

		/*** IT MAY BE MORE EFFICIENT TO LINK ALL GLOBAL GUI CONTROLS TO BULB OBJECTS AS PER VIDEO TUTORIAL
		THIS WOULD MEAN THE VALUES DO NOT NEED TO BE PASSED TO INDIVIDUAL OBJECTS THROUGH THE DRAW COMMAND*/

		float adjustedBrightness = ofMap(tmpCol.r, 0, 255, 0, masterBrightness, true); //map the brightness to the masterBrightness variable

		myBulb[i].draw(adjustedBrightness); //call draw sending colour value to object.

											////*********send DMX Values***********////
											//When sending to Mk2 you have to set universe as well as channel and value
											//dmx.setLevel(channel, value, universe)
		int thisLightValue = myBulb[i].dmxLightVal;
		int thisID = myBulb[i].dmxID;
		dmx.setLevel(thisID, thisLightValue, 2);

		///uncomment to check outgoing DMX values
		///printf("dmxVal: %i\n", thisLightValue);
	}

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key)
	{
	case '1':
		myScene = SCENE_1;;
		break;
	case '2':
		myScene = SCENE_2;
		break;
	case '3':
		myScene = SCENE_3;
		break;
	}

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

////******************* XML LOAD/SAVE BULB LOCATION FUNCTIONS ********************/////
//--------------------------------------------------------------
void ofApp::loadButtonPressed() {
	ofxXmlSettings settings;
	if (settings.loadFile("positions.xml")) {
		settings.pushTag("positions");
		int numberOfSavedPoints = settings.getNumTags("position");
		for (int i = 0; i < numberOfSavedPoints; i++) {
			settings.pushTag("position", i);

			int tempX = settings.getValue("X", 0);
			int tempY = settings.getValue("Y", 0);

			myBulb[i].setLoc(tempX, tempY);
			settings.popTag();
		}

		settings.popTag(); //pop position
	}
	else {
		ofLogError("Position file did not load!");
	}
}

//--------------------------------------------------------------
void ofApp::saveButtonPressed() {

	ofxXmlSettings positions;
	positions.addTag("positions");
	positions.pushTag("positions");
	//points is a vector<ofPoint> that we want to save to a file
	for (int i = 0; i < NBULBS; i++) {
		//each position tag represents one point
		positions.addTag("position");
		positions.pushTag("position", i);
		//so set the three values in the file
		positions.addValue("X", myBulb[i].x);
		positions.addValue("Y", myBulb[i].y);
		positions.popTag();//pop position
	}
	positions.popTag(); //pop position
	positions.saveFile("positions.xml");

}

//--------------------------------------------------------------
void ofApp::exit() {
	loadBulbLocations.removeListener(this, &ofApp::loadButtonPressed);
	saveBulbLocations.removeListener(this, &ofApp::saveButtonPressed);
}
