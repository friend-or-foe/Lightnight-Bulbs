#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//dmx.connect("tty.usbserial-EN143965", NBULBS); // use the name
	dmx.connect(0, NBULBS); // or use a number
	dmx.activateMk2(); //Un comment this if using dmxUSBPro mk2

	////// LOAD PLAN IMAGE AND SET OFFSETS FOR COLOUR GRABBING ////////
	plan.load("SGH - Catacomb Plans - 1200.jpg");

	//return the width + height of image
	planWidth = plan.getWidth();
	planHeight = plan.getHeight();

	//printf("planWidth: %i\n", planWidth);

	//set colour grab offsets (Location to draw plan to)
	planOffsetX = 470; // plan.getWidth();
	planOffsetY = 50; // plan.getHeight();


	//load scene 2 images
	for (int i = 0; i < 8; i++) {
		sc2_grad[i].load("gradient PNGS/circGrad0" + ofToString(i+1) + "-01.png");
	}

	//create spin objects
	for (int i = 0; i < NSPINNERS; i++) {
		ofImage tempImage;
		if (i < 8) { tempImage = sc2_grad[i];} else { tempImage = sc2_grad[i-8]; }
		mySpinner[i].setup(i, tempImage);
	}

	//create centre circle objects
	//for (int i = 0; i<20; i++) {
	//	centCirc tempCirc;
	//	tempCirc.setup(i, sc3_xLoc, sc3_yLoc, ofRandom(100));
	//	myCirc.push_back(tempCirc);
	//}

	//create bulb objects
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].setup(i, planWidth, planHeight, planOffsetX, planOffsetY, bulbSize);
	}

	///--------- INIT MIDI --------------//
	initMIDI();

	///--------- INIT GUI --------------//
	initGUI();

	///--------- INIT AUDIO --------------//
	initAudio();

	ofSetFrameRate(200);
	ofBackground(40);
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

void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;

	// draw the last recieved message contents to the consol
	MIDItext << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
	MIDItext.str(""); // clear

	MIDItext << "channel: " << midiMessage.channel;
	MIDItext.str(""); // clear

	MIDItext << "pitch: " << midiMessage.pitch;
	MIDItext.str(""); // clear

	MIDItext << "velocity: " << midiMessage.velocity;
	MIDItext.str(""); // clear

	MIDItext << "control: " << midiMessage.control;
	MIDItext.str(""); // clear

	MIDItext << "delta: " << midiMessage.deltatime;
	MIDItext.str(""); // clear

	//midiControl = midiMessage.control;

	/*
	//Faders on microKontrol are control numbers 10-17
	if (midiMessage.status == MIDI_CONTROL_CHANGE) {
		midiMessage.value = midiMessage.value * 2;

		switch (midiControl) {
		case 12:
			cSlider1 = midiMessage.value;
			break;
		case 13:
			cSlider2 = midiMessage.value;
			break;
		case 14:
			cSlider3 = midiMessage.value;
			break;
		}
	}
	*/
}


//--------------------------------------------------------------

void ofApp::update() {

	fftChannelL.update();
	fftChannelR.update();


	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].update(bulbSize);
		
	}
	

}


//--------------------------------------------------------------
void ofApp::draw() {

	//ofBackground(0);

	ofFill(); //set fill back on
	ofSetColor(0);
	ofDrawRectangle(planOffsetX, planOffsetY, planWidth, planHeight); //draw rectanle to wipe previous frame

	
	//ofSetColor(255, ofGetMouseY() / 4); //set transperency of test circle
	//ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ofGetMouseX() / 5, ofGetMouseX() / 5);

	///**** PICK SCENE ****///
	switch (myScene)
	{
	case SCENE_1:
		mainScene_1();
			break;
	case SCENE_2:
		mainScene_2();
			break;
	case SCENE_3:
		mainScene_3();
			break;

	case SCENE_9:
		mainScene_9();
		break;
	case SCENE_0:
		mainScene_0();
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

void ofApp::sendDMXVals(int _bulbNum) {

	////*********send DMX Values***********////
	//When sending to Mk2 you have to set universe as well as channel and value
	//dmx.setLevel(channel, value, universe)

	int thisLightValue = myBulb[_bulbNum].dmxLightVal;
	int thisID = myBulb[_bulbNum].dmxID;
	dmx.setLevel(thisID, thisLightValue, 2);

	///uncomment to check outgoing DMX values
	///printf("dmxVal: %i\n", thisLightValue);

}
////************************************************************ ALL SCENE FUNCTIONS ********************/////

//-------------------------------------------------------------- SCENE 01
void ofApp::mainScene_1() {

	///----------- DRAW FFT SHAPES ---------------//
	drawFFT_scene1();

	//grab screenshot  before bulbs are drawn
	tmpImage.grabScreen(planOffsetX, planOffsetY, planWidth, planHeight);

	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	}
	else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(planOffsetX, planOffsetY, planWidth, planHeight);
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

		myBulb[i].draw_sc1(adjustedBrightness); //call draw sending colour value to object.

		sendDMXVals(i);
		
	}

}

//-------------------------------------------------------------- SCENE 02
void ofApp::mainScene_2() {

	///----------- DRAW FFT SHAPES ---------------//
	//analyseFFT_scene2(fftChannelL.getFftNormData());

	//ofSetColor(255, sc2_opac);
	vector<float> samples = fftChannelL.getFftNormData();

	ofPushMatrix();
	ofTranslate(sc2_xLoc, sc2_yLoc);
	ofScale(sc2_scale, sc2_scale);
	ofRotate(sc2_baseRot);

	for (int i = 0; i < NSPINNERS; i++) {
		//smoothing for fft values
		float value = samples[i];// *= 0.993f;
		fftSmooth[i*sc2_freqStep] *= sc2_smoothAmount;
		if (fftSmooth[i * sc2_freqStep] < value) {
			fftSmooth[i * sc2_freqStep] = value;
		}
		ofSetColor(255, abs(fftSmooth[i*sc2_freqStep] * sc2_opac));
		//ofImage tempIm = sc2_grad[i];
		mySpinner[i].update();
		mySpinner[i].draw();
	}
	
	ofPopMatrix();

	sc2_baseRot += sc2_rotSpeed;

	//grab screenshot  before bulbs are drawn
	tmpImage.grabScreen(planOffsetX, planOffsetY, planWidth, planHeight);

	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	}
	else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(planOffsetX, planOffsetY, planWidth, planHeight);
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

		myBulb[i].draw_sc1(adjustedBrightness); //call draw sending colour value to object.

		sendDMXVals(i);

	}


}

//-------------------------------------------------------------- SCENE 03
void ofApp::mainScene_3() {


	vector<float> samples = fftChannelL.getFftNormData();

	if (sc3_countDown == 0) {
		for (int i = 0; i < samples.size(); i += 8) {
			if (samples[i] > sc3_gate) {
				centCirc tempCirc;
				tempCirc.setup(i, sc3_xLoc, sc3_yLoc, 10, samples[i]*sc3_velocityMultiplier, sc3_lifeSpan);
				myCirc.push_back(tempCirc);
			}
		}
		sc3_countDown = sc3_count;
	}
	else {
		sc3_countDown--;
	}


	ofNoFill();
	ofSetColor(255);

	for (int i = 0; i<myCirc.size(); i++) {
		myCirc[i].update();
		myCirc[i].draw();
	}
	//if (myCirc.size() > 0){
	for (int i = myCirc.size() -1; i >= 0; i--) {
	//	if (myCirc[i].rad > 400) {
	//		myCirc.erase(myCirc.begin() + i);
	//	}
		if (myCirc[i].myAge == 0) {
			myCirc.erase(myCirc.begin() + i);
		}
	}
	

	//grab screenshot  before bulbs are drawn
	tmpImage.grabScreen(planOffsetX, planOffsetY, planWidth, planHeight);

	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	}
	else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(planOffsetX, planOffsetY, planWidth, planHeight);
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

		myBulb[i].draw_sc1(adjustedBrightness); //call draw sending colour value to object.

		sendDMXVals(i);

	}


}

//-------------------------------------------------------------- SCENE 9 - Glimmer / Noise
void ofApp::mainScene_9() {


	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	}
	else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(planOffsetX, planOffsetY, planWidth, planHeight);
	}

	//int myBright;

	float myBright;
	float adjustedBrightness = 0;
	
	float mainBright = ofMap(sc9_allBrightness, 0, 255, 0, masterBrightness, true);
	//float fadeSpeed = sc9_fadeSpeed;

	for (int i = 0; i < NBULBS; i++) {
		int randChance = ofRandom(1000);
		if (randChance < sc9_changeChance){
			
				myBright = sc9_allBrightness - (ofRandom(sc9_noiseScale*sc9_allBrightness));// (ofNoise(ofRandom(sc9_noiseVal))*sc9_noiseScale);
			
		//printf("myBulb - colour: %i\n", myBright);
		/*** IT MAY BE MORE EFFICIENT TO LINK ALL GLOBAL GUI CONTROLS TO BULB OBJECTS AS PER VIDEO TUTORIAL
		THIS WOULD MEAN THE VALUES DO NOT NEED TO BE PASSED TO INDIVIDUAL OBJECTS THROUGH THE DRAW COMMAND*/

				adjustedBrightness = ofMap(myBright, 0, 255, 0, masterBrightness, true); //map the brightness to the masterBrightness variable
				myBulb[i].myTrigger = true;
				myBulb[i].dmxLightVal = adjustedBrightness;
		}

			myBulb[i].draw_sc9(mainBright, sc9_fadeSpeed); //call draw sending colour value to object.

			sendDMXVals(i);
	
	}
	
}

//-------------------------------------------------------------- SCENE 0 - test/warmer scene
void ofApp::mainScene_0() {


	if (drawPlan) {
		ofSetColor(255);
		plan.draw(planOffsetX, planOffsetY);
	}
	else {
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(planOffsetX, planOffsetY, planWidth, planHeight);
	}

	//int myBright;

	for (int i = 0; i < NBULBS; i++) {
		
		/*** IT MAY BE MORE EFFICIENT TO LINK ALL GLOBAL GUI CONTROLS TO BULB OBJECTS AS PER VIDEO TUTORIAL
		THIS WOULD MEAN THE VALUES DO NOT NEED TO BE PASSED TO INDIVIDUAL OBJECTS THROUGH THE DRAW COMMAND*/

		float adjustedBrightness = ofMap(sc0_allBrightness, 0, 255, 0, masterBrightness, true); //map the brightness to the masterBrightness variable

		myBulb[i].draw_sc0(adjustedBrightness); //call draw sending colour value to object.

		sendDMXVals(i);

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

	case '9':
		myScene = SCENE_9;
		break;
	case '0':
		myScene = SCENE_0;
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
	for (int i = 0; i < NBULBS; i++) {
		myBulb[i].mouseReleased();
	}

	if (myScene = SCENE_3) {
	sc3_xLoc = ofGetMouseX();
	sc3_yLoc = ofGetMouseY();
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

////************************************************************ ALL INIT CALLS ********************/////

//-------------------------------------------------------------- GUI
void ofApp::initGUI() {

	//****** MASTER GUI CONTROLS ********//
	loadBulbLocations.addListener(this, &ofApp::loadButtonPressed);
	saveBulbLocations.addListener(this, &ofApp::saveButtonPressed);

	gui.setup();

	master.setName("Master");
	master.add(masterBrightness.set("master brightness", masterBrightness, 0.0, 255));
	master.add(bulbSize.set("bulb size", bulbSize, 0.1, 40.0));
	master.add(drawPlan.set("draw plan", false));
	gui.add(master);

	gui.add(loadBulbLocations.setup("load bulb locations"));
	gui.add(saveBulbLocations.setup("save bulb locations"));

	//****** SCENE 1 GUI CONTROLS ********//
	scene_01.setName("SCENE 01");
	scene_01.add(sc1_sampleScale.set("sample scale", sc1_sampleScale, 0.0, 1000));
	scene_01.add(sc1_xLoc.set("X location", sc1_xLoc, 0, ofGetWidth()));
	scene_01.add(sc1_yLoc.set("Y location", sc1_yLoc, 0, ofGetHeight()));
	scene_01.add(sc1_opac.set("circle opacity", sc1_opac, 0, 255));

	gui.add(scene_01);

	//****** SCENE 2 GUI CONTROLS ********//
	scene_02.setName("SCENE 02");
	//scene_01.add(sc1_sampleScale.set("sample scale", sc1_sampleScale, 0.0, 1000));
	scene_02.add(sc2_xLoc.set("X location", sc2_xLoc, 0, ofGetWidth()));
	scene_02.add(sc2_yLoc.set("Y location", sc2_yLoc, 0, ofGetHeight()));
	scene_02.add(sc2_rotSpeed.set("rotation speed", sc2_rotSpeed, 0, 10));
	scene_02.add(sc2_scale.set("scale", sc2_scale, 0, 2.0));
	scene_02.add(sc2_opac.set("opacity multiplier", sc2_opac, 0, 2000));
	scene_02.add(sc2_freqStep.set("frequency step", sc2_freqStep, 1, 100));
	scene_02.add(sc2_smoothAmount.set("smooth amount", sc2_smoothAmount, 0.5, 0.99));

	gui.add(scene_02);

	//****** SCENE 3 GUI CONTROLS ********//
	scene_03.setName("SCENE 03");
	//scene_01.add(sc1_sampleScale.set("sample scale", sc1_sampleScale, 0.0, 1000));
	scene_03.add(sc3_xLoc.set("X location", sc3_xLoc, 0, ofGetWidth()));
	scene_03.add(sc3_yLoc.set("Y location", sc3_yLoc, 0, ofGetHeight()));
	scene_03.add(sc3_gate.set("trigger gate", sc3_gate, 0.05, 1.0));
	scene_03.add(sc3_count.set("count down", sc3_count, 0, 20));
	scene_03.add(sc3_velocityMultiplier.set("velocity multiplier", sc3_velocityMultiplier, 0.0, 40.0));
	scene_03.add(sc3_lifeSpan.set("lifespan", sc3_lifeSpan, 0, 200));

	gui.add(scene_03);

	//****** SCENE 9 GUI CONTROLS ********//
	scene_09.setName("SCENE 09");
	scene_09.add(sc9_allBrightness.set("bulb brightness", sc9_allBrightness, 0, 255));
	scene_09.add(sc9_fadeSpeed.set("fade speed", sc9_fadeSpeed, 0.0, 1.0));
	scene_09.add(sc9_noiseScale.set("noise scale", sc9_noiseScale, 0.0, 1));
	scene_09.add(sc9_changeChance.set("chance of change", sc9_changeChance, 0.0, 1000));

	gui.add(scene_09);

	//****** SCENE 0 GUI CONTROLS ********//
	scene_00.setName("SCENE 00");
	scene_00.add(sc0_allBrightness.set("bulb brightness", sc0_allBrightness, 0, 255));

	gui.add(scene_00);

}

//-------------------------------------------------------------- MIDI

void ofApp::initMIDI() {

	ofSetLogLevel(OF_LOG_VERBOSE);

	// print input ports to console
	midiIn.listPorts(); // via instance
						//ofxMidiIn::listPorts(); // via static as well
						// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, true, true);

	// add ofApp as a listener
	midiIn.addListener(this);

	//midiIn.getPort();

	// print received messages to the console
	midiIn.setVerbose(true);

}
//-------------------------------------------------------------- AUDIO

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

////************************************************************ ALL FFT DRAWING CALLS ********************/////

//-------------------------------------------------------------- SCENE 1
void ofApp::drawFFT_scene1() {

	ofSetColor(255, sc1_opac);
	ofPushMatrix();
	ofTranslate(sc1_xLoc, sc1_yLoc);
	drawSamples_scene1(fftChannelL.getFftNormData());
	ofPopMatrix();

	//uncomment to draw right channel
	/*
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() * 0.75);
	drawSamples(fftChannelR.getFftNormData());
	ofPopMatrix();
	*/
}

void ofApp::drawSamples_scene1(vector<float> samples) {

	int sampleWidth = ofGetWidth() / samples.size();
	int sampleHeight = ofGetHeight() / 2;
	int numOfSamples = samples.size();

	for (int i = 0; i<numOfSamples; i += 10) {
		int x = 0; // ofMap(i, 0, numOfSamples - 1, 0, ofGetWidth() - sampleWidth);
		int y = 0;

		//smoothing for fft values
		float value = samples[i] * sc1_sampleScale;// *= 0.993f;
		fftSmooth[i] *= 0.98f;
		if (fftSmooth[i] < value) {
			fftSmooth[i] = value;
		}

		int w = 1 + (-fftSmooth[i]);
		//int h = -samples[i] * sampleHeight;

		//ofDrawEllipse(x, y, w, w);
		ofDrawEllipse(0, 0, w, w);
	}

}

//-------------------------------------------------------------- SCENE 2


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

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}
