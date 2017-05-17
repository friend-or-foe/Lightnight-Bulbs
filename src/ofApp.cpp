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

	//create bar objects - Scene 5
	for (int i = 0; i < NBARS; i++) {
		myBars[i].setup(i, sc5_xLoc, sc5_yLoc);
	}

	//create elliptical orbit objects - Scene 7
	for (int i = 0; i < NELLIPSES; i++) {
		myEllipses[i].setup(i, sc7_xStart, sc7_yStart, sc7_xSpread, sc7_ySpread);
	}

	//create bulb objects
	for (int i = 0; i<NBULBS; i++) {
		myBulb[i].setup(i, planWidth, planHeight, planOffsetX, planOffsetY, bulbSize);
	}

	//create midiShape objects
	for (int i = 0; i<NSHAPES; i++) {
		float tempX = ofRandom(planOffsetX, (planOffsetX + planWidth));
		float tempY = ofRandom(planOffsetY, (planOffsetY + planHeight));
		myShapes[i].setup(i, tempX, tempY);
	}

	//create spot objects **SCENE 8**
	for (int i = 0; i<NSPOTS; i++) {
		if (i < 5) {
			mySpots[i].pos.set(sc8_xLoc + (sc8_spacing*i), sc8_yLoc, 0);
		}
		else {
			mySpots[i].pos.set(sc8_xLoc + (sc8_spacing*(i - 5)), sc8_yLoc + sc8_spacing, 0);
		}
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

	if(midiMessage.pitch != 0){
	printf("channel:  %i\n", midiMessage.channel);
	printf("pitch:  %i\n", midiMessage.pitch);
	}

	//---------------------------------------------- Scene 5 MIDI
	if (myScene == SCENE_5) {
		if (midiMessage.velocity != 0) {
			//if (midiMessage.channel == 1) {
			int pitchBase = 36;
			int midiKey = midiMessage.pitch;
				if ((midiKey >= pitchBase) && (midiKey <= (pitchBase + sc5_numBars))) {
					float adjustedBrightness = ofMap(255, 0, 255, 0, masterBrightness, true); //map the brightness to the masterBrightness variable
					myBars[midiKey - pitchBase].bright = adjustedBrightness;
				}
			//}
			
		}	
	}

	if (myScene == SCENE_4) {
		if (midiMessage.velocity != 0) {
			if (midiMessage.channel == 1) {
						if (plinkCount < 10) {
							float tempX = ofRandom(planOffsetX, (planOffsetX + planWidth));
							float tempY = ofRandom(planOffsetY, (planOffsetY + planHeight));
							myShapes[plinkCount].initPlink(plinkCount, tempX, tempY, sc4_plinkBright);
							plinkCount++;
						}
						else {
							plinkCount = 0;
							float tempX = ofRandom(planOffsetX, (planOffsetX + planWidth));
							float tempY = ofRandom(planOffsetY, (planOffsetY + planHeight));
							myShapes[plinkCount].initPlink(plinkCount, tempX, tempY, sc4_plinkBright);
							plinkCount++;
						}
			}
			if (midiMessage.channel == 2) {
				if (bassCount < 19) {
					float tempX = ofRandom(planOffsetX + sc4_leftEdge, (planOffsetX + sc4_rightEdge));
					float tempY = planOffsetY + (planHeight / 2);
					myShapes[bassCount].initBass(bassCount, tempX, tempY, sc4_bassBright);
					bassCount++;
				}
				else {
					bassCount = 10;
					float tempX = ofRandom(planOffsetX + sc4_leftEdge, (planOffsetX + sc4_rightEdge));
					float tempY = planOffsetY + (planHeight / 2);
					myShapes[bassCount].initBass(bassCount, tempX, tempY, sc4_bassBright);
					bassCount++;
				}
			}
			if (midiMessage.channel == 3) {
				if (keyCount < 25) {
					float tempX = planOffsetX + (planWidth / 2);
					float tempY = planOffsetY + (planHeight / 2);
					myShapes[keyCount].initKey(keyCount, tempX, tempY, sc4_keyBright);
					keyCount++;
				}
				else {
					keyCount = 19;
					float tempX = planOffsetX + (planWidth / 2);
					float tempY = planOffsetY + (planHeight / 2);
					myShapes[keyCount].initKey(keyCount, tempX, tempY, sc4_keyBright);
					keyCount++;
				}
			}
		}
	}
	//midiControl = midiMessage.control;

	
	//Faders on microKontrol are control numbers 10-17
	

	if (midiMessage.status == MIDI_CONTROL_CHANGE) {
		if (midiMessage.control != 5) {
		printf("channel:  %i\n", midiMessage.channel);
		printf("controller num:  %i\n", midiMessage.control);
		printf("value:  %i\n", midiMessage.value);
	}
		/*
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
		*/
	}
	
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
	case SCENE_4:
		mainScene_4();
	case SCENE_5:
		mainScene_5();
		break;
	case SCENE_6:
		mainScene_6();
		break;
	case SCENE_7:
		mainScene_7();
		break;
	case SCENE_8:
		mainScene_8();
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

	for (int i = sc2_startFreq; i < NSPINNERS; i++) {
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
		
		float tempDist = 1000; //store diameter of circle
		float measure = ofDist(sc3_xLoc, sc3_yLoc, myBulb[i].x, myBulb[i].y); //get distance of bulb from centre to work out if line is intersecting
		float tempBright = 0; //set brightness to 0 initialy. only updated if it is within range

		for (int i = 0; i < myCirc.size(); i++) {
			tempDist = (myCirc[i].rad)*1.25; //not sure why this has to be multiplied but seems to be working
			float gap = abs(tempDist - measure);
			if (gap <= sc3_range) {
				tempBright+= 255 - (gap * (255.0/ sc3_range));
			}

		}

		
	
		/*** IT MAY BE MORE EFFICIENT TO LINK ALL GLOBAL GUI CONTROLS TO BULB OBJECTS AS PER VIDEO TUTORIAL
		THIS WOULD MEAN THE VALUES DO NOT NEED TO BE PASSED TO INDIVIDUAL OBJECTS THROUGH THE DRAW COMMAND*/

		float adjustedBrightness = ofMap(tempBright, 0, 255, 0, masterBrightness, true); //map the brightness to the masterBrightness variable

		myBulb[i].draw_sc3(adjustedBrightness); //call draw sending colour value to object.

		sendDMXVals(i);

	}


}

//-------------------------------------------------------------- SCENE 04
void ofApp::mainScene_4() {

	//ofSetColor(255);
	ofSetRectMode(OF_RECTMODE_CENTER);
	for (int i = 0; i < NSHAPES; i++) {
		
		myShapes[i].draw(sc4_fade);
		
		/*
		if (i < 10) {
			myShapes[i].drawPlink(sc4_fade);
		}

		if ((ID >= 10) && (ID < 19)) {
			if (bright> 0) { ofSetColor(255, bright + ofRandom(-bright, bright / 4)); }
			drawBass(_fadeSpeed);
		}

		if (ID >= 19) {
			ofNoFill();
			ofSetLineWidth(10);
			drawKey(_fadeSpeed);
			ofSetLineWidth(1);
		}
		*/
	}
	ofSetRectMode(OF_RECTMODE_CORNER);

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

//-------------------------------------------------------------- SCENE 05
void ofApp::mainScene_5() {

	if (sc5_drawFFT) {
	///----------- DRAW FFT SHAPES ---------------//
	drawFFT_scene5();
	}

	////------------------
	float barW = sc5_width / sc5_numBars;


	for (int i = 0; i < sc5_numBars; i++) {
		float myLoc = sc5_xLoc + (i*barW);
		myBars[i].update(myLoc, barW);
		myBars[i].draw(sc5_fade);
	}

	//grab screenshot  before bulbs are drawn
	//tmpImage.grabScreen(planOffsetX, planOffsetY, planWidth, planHeight);

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

//-------------------------------------------------------------- SCENE 06
void ofApp::mainScene_6() {

	///----------- DRAW FFT SHAPES ---------------//
	drawFFT_scene6();

	///----------- DRAW MIDI SHAPES ----------------
	//ofSetColor(255);
	//ofPushMatrix();
	//ofTranslate(planOffsetX + (planWidth / 2), planOffsetY + (planHeight / 2));
	ofSetRectMode(OF_RECTMODE_CENTER);
	for (int i = 0; i < NSHAPES; i++) {

		myShapes[i].drawRussPlink(sc6_russFade);

	}
	ofSetRectMode(OF_RECTMODE_CORNER);
	//ofPopMatrix();

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

//-------------------------------------------------------------- SCENE 07
void ofApp::mainScene_7() {

	vector<float> samples = fftChannelL.getFftNormData();

	ofPushMatrix();
	ofTranslate(sc7_xLoc, sc7_yLoc);
	for (int i = 0; i < NELLIPSES; i++) {
		myEllipses[i].update(sc7_xStart, sc7_yStart, sc7_xSpread, sc7_ySpread);
		myEllipses[i].draw();
	}
	ofPopMatrix();

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

//-------------------------------------------------------------- SCENE 08
void ofApp::mainScene_8() {


	drawFFT_scene8();

	//--
	//float barW = sc5_width / sc8_numSpots;

	for (int i = 0; i < NSPOTS; i++) {
		//sc8_xLoc -= 0.1;
		//mySpots[i].update(i, sc8_xLoc, sc8_yLoc, sc8_spacing, sc8_move);
		mySpots[i].update(sc8_move);
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
		int randChance = ofRandom(3000);
		if (randChance < sc9_changeChance) {

			myBright = sc9_allBrightness;// -(ofRandom(sc9_noiseScale*sc9_allBrightness));// (ofNoise(ofRandom(sc9_noiseVal))*sc9_noiseScale);

																						//printf("myBulb - colour: %i\n", myBright);
																						/*** IT MAY BE MORE EFFICIENT TO LINK ALL GLOBAL GUI CONTROLS TO BULB OBJECTS AS PER VIDEO TUTORIAL
																						THIS WOULD MEAN THE VALUES DO NOT NEED TO BE PASSED TO INDIVIDUAL OBJECTS THROUGH THE DRAW COMMAND*/

			adjustedBrightness = ofMap(myBright, 0, 255, 0, masterBrightness, true); //map the brightness to the masterBrightness variable
			myBulb[i].myTrigger = true;
			myBulb[i].dmxLightVal = adjustedBrightness;
		}

		myBulb[i].draw_sc9(mainBright, sc9_fineFadeSpeed, sc9_fadeSpeed); //call draw sending colour value to object.

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
		myScene = SCENE_1;
		for (int i = 0; i < 9; i++) {
			gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
			gui.getGroup("SCENE 01").maximize();
		break;
	case '2':
		myScene = SCENE_2;
		for (int i = 0; i < 9; i++) {
				gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
			gui.getGroup("SCENE 02").maximize();
		break;
	case '3':
		myScene = SCENE_3;
		for (int i = 0; i < 9; i++) {
				gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
			gui.getGroup("SCENE 03").maximize();
		break;
	case '4':
		myScene = SCENE_4;
		for (int i = 0; i < 9; i++) {
				gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
			gui.getGroup("SCENE 04").maximize();
		break;
	case '5':
		myScene = SCENE_5;
		for (int i = 0; i < 9; i++) {
				gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
		gui.getGroup("SCENE 05").maximize();
		break;
	case '6':
		myScene = SCENE_6;
		for (int i = 0; i < 9; i++) {
				gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
		gui.getGroup("SCENE 06").maximize();
		break;
	case '7':
		myScene = SCENE_7;
		for (int i = 0; i < 9; i++) {
				gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
		gui.getGroup("SCENE 07").maximize();
		break;
	case '8':
		myScene = SCENE_8;
		for (int i = 0; i < 9; i++) {
			gui.getGroup("SCENE 0" + ofToString(i)).minimize();
		}
		gui.getGroup("SCENE 08").maximize();
		break;
	case '9':
		myScene = SCENE_9;
		for (int i = 0; i < 9; i++) {
			gui.getGroup("SCENE 0" + ofToString(i)).minimize();	
		}
		gui.getGroup("SCENE 09").maximize();
		break;
	case '0':
		myScene = SCENE_0;
		break;
	}

	//----------------- TEST SCENE 4 SHAPES ------------------//
	if (myScene == SCENE_4) {
		if (key == 'q') {
			if (plinkCount < 10) {
				float tempX = ofRandom(planOffsetX + sc4_leftEdge, (planOffsetX + sc4_rightEdge));
				float tempY = ofRandom(planOffsetY + sc4_topEdge, (planOffsetY + sc4_botEdge));
				myShapes[plinkCount].initPlink(plinkCount, tempX, tempY, sc4_plinkBright);
				plinkCount++;
			}
			else {
				plinkCount = 0;
				float tempX = ofRandom(planOffsetX + sc4_leftEdge, (planOffsetX + sc4_rightEdge));
				float tempY = ofRandom(planOffsetY + sc4_topEdge, (planOffsetY + sc4_botEdge));
				myShapes[plinkCount].initPlink(plinkCount, tempX, tempY, sc4_plinkBright);
				plinkCount++;
			}
		}
		if (key == 'w') {
			if (bassCount < 19) {
				float tempX = ofRandom(planOffsetX + sc4_leftEdge, (planOffsetX + sc4_rightEdge));
				float tempY = planOffsetY + (planHeight / 2);
				myShapes[bassCount].initBass(bassCount, tempX, tempY, sc4_bassBright);
				bassCount++;
			}
			else {
				bassCount = 10;
				float tempX = ofRandom(planOffsetX + sc4_leftEdge, (planOffsetX + sc4_rightEdge));
				float tempY = planOffsetY + (planHeight / 2);
				myShapes[bassCount].initBass(bassCount, tempX, tempY, sc4_bassBright);
				bassCount++;
			}
		}
		if (key == 'e') {
			if (keyCount < 25) {
				float tempX = planOffsetX + (planWidth /2);
				float tempY = planOffsetY + (planHeight / 2);
				myShapes[keyCount].initKey(keyCount, tempX, tempY, sc4_keyBright);
				keyCount++;
			}
			else {
				keyCount = 19;
				float tempX = planOffsetX + (planWidth / 2);
				float tempY = planOffsetY + (planHeight / 2);
				myShapes[keyCount].initKey(keyCount, tempX, tempY, sc4_keyBright);
				keyCount++;
			}
		}
	}

	//printf("key: %i\n", key);
	//----------------- TEST SCENE 5 SHAPES ------------------//
	if (myScene == SCENE_5) {
		
		switch (key)
		{
		case 'q':
			myBars[0].bright = 255;
			break;
		case 'w':
			myBars[1].bright = 255;
			break;
		case 'e':
			myBars[2].bright = 255;
			break;
		case 'r':
			myBars[3].bright = 255;
			break;
		case 't':
			myBars[4].bright = 255;
			break;

		case 'y':
			myBars[5].bright = 255;
			break;
		case 'u':
			myBars[6].bright = 255;
			break;
		}
	}

	//----------------- TEST SCENE 4 SHAPES ------------------//
	if (myScene == SCENE_6) {
		int randBulb = int(ofRandom(30, NBULBS));
		switch (key)
		{
		case 'a':
			myShapes[0].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 's':
			myShapes[1].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 'd':
			myShapes[2].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 'f':
			myShapes[3].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 'g':
			myShapes[4].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 'h':
			myShapes[5].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 'j':
			myShapes[6].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		case 'k':
			myShapes[7].initRussPlink(plinkCount, myBulb[randBulb].x, myBulb[randBulb].y, sc6_russBright);
			break;
		}
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

	if (myScene == SCENE_3) {
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

	gui.setDefaultWidth(400);
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
	scene_01.add(sc1_startFreq.set("star fequency", sc1_startFreq, 0, 100));
	scene_01.add(sc1_freqStep.set("frequency step", sc1_freqStep, 1, 100));
	scene_01.add(sc1_smoothAmount.set("smooth amount", sc1_smoothAmount, 0.5, 0.99));

	gui.add(scene_01);

	//****** SCENE 2 GUI CONTROLS ********//
	scene_02.setName("SCENE 02");
	//scene_01.add(sc1_sampleScale.set("sample scale", sc1_sampleScale, 0.0, 1000));
	scene_02.add(sc2_xLoc.set("X location", sc2_xLoc, 0, ofGetWidth()));
	scene_02.add(sc2_yLoc.set("Y location", sc2_yLoc, 0, ofGetHeight()));
	scene_02.add(sc2_rotSpeed.set("rotation speed", sc2_rotSpeed, 0, 10));
	scene_02.add(sc2_scale.set("scale", sc2_scale, 0, 2.0));
	scene_02.add(sc2_opac.set("opacity multiplier", sc2_opac, 0, 2000));
	scene_02.add(sc2_startFreq.set("start frequency", sc2_startFreq, 0, 100));
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
	scene_03.add(sc3_range.set("range", sc3_range, 8, 240));

	gui.add(scene_03);

	//****** SCENE 4 GUI CONTROLS ********//
	scene_04.setName("SCENE 04");
	scene_04.add(sc4_fade.set("fade speed", sc4_fade, 1, 20));
	scene_04.add(sc4_plinkBright.set("plink brightness", sc4_plinkBright, 0, 255));
	scene_04.add(sc4_bassBright.set("bass brightness", sc4_bassBright, 0, 255));
	scene_04.add(sc4_keyBright.set("key brightness", sc4_keyBright, 0, 255));
	gui.add(scene_04);

	//****** SCENE 5 GUI CONTROLS ********//
	scene_05.setName("SCENE 05");
	scene_05.add(sc5_xLoc.set("X location", sc5_xLoc, 0, ofGetWidth()));
	scene_05.add(sc5_yLoc.set("Y location", sc5_yLoc, 0, ofGetHeight()));
	scene_05.add(sc5_width.set("span of bars", sc5_width, 500, 1400));
	scene_05.add(sc5_numBars.set("number of bars", sc5_numBars, 0, NBARS));
	scene_05.add(sc5_fade.set("fade speed", sc5_fade, 1, 20));

	scene_05.add(sc5_drawFFT.set("draw FFT", false));
	scene_05.add(sc5_FFTxLoc.set("FFT X location", sc5_FFTxLoc, 0, ofGetWidth()));
	scene_05.add(sc5_FFTyLoc.set("FFT Y location", sc5_FFTyLoc, 0, ofGetHeight()));
	scene_05.add(sc5_opac.set("circle opacity", sc5_opac, 0, 255));
	scene_05.add(sc5_sampleScale.set("sample scale", sc5_sampleScale, 0.0, 1000));
	scene_05.add(sc5_startFreq.set("start fequency", sc5_startFreq, 0, 100));
	scene_05.add(sc5_freqStep.set("frequency step", sc5_freqStep, 1, 100));
	scene_05.add(sc5_smoothAmount.set("smooth amount", sc5_smoothAmount, 0.5, 0.99));
	gui.add(scene_05);

	//****** SCENE 6 GUI CONTROLS ********//
	scene_06.setName("SCENE 06");
	scene_06.add(sc6_sampleScale.set("sample scale", sc6_sampleScale, 0.0, 1000));
	scene_06.add(sc6_xLoc.set("X location", sc6_xLoc, 0, ofGetWidth()));
	scene_06.add(sc6_yLoc.set("Y location", sc6_yLoc, 0, ofGetHeight()));
	scene_06.add(sc6_opac.set("circle opacity", sc6_opac, 0, 255));
	scene_06.add(sc6_freqStep.set("frequency step", sc6_freqStep, 1, 100));
	scene_06.add(sc6_smoothAmount.set("smooth amount", sc6_smoothAmount, 0.5, 0.99));
	scene_06.add(sc6_russBright.set("Russ Plink Brightness", sc6_russBright, 0, 255));
	scene_06.add(sc6_russFade.set("russ fade speed", sc6_russFade, 0.1, 10));

	gui.add(scene_06);

	//****** SCENE 7 GUI CONTROLS ********//
	scene_07.setName("SCENE 07");
	scene_07.add(sc7_xLoc.set("X location", sc7_xLoc, 0, ofGetWidth()));
	scene_07.add(sc7_yLoc.set("Y location", sc7_yLoc, 0, ofGetHeight()));
	scene_07.add(sc7_xStart.set("X hole size", sc7_xStart, 0, 200));
	scene_07.add(sc7_yStart.set("Y hole size", sc7_yStart, 0, 200));
	scene_07.add(sc7_xSpread.set("X Spread", sc7_xSpread, 0, 200));
	scene_07.add(sc7_ySpread.set("Y Spread", sc7_ySpread, 0, 200));

	gui.add(scene_07);

	//****** SCENE 8 GUI CONTROLS ********//
	scene_08.setName("SCENE 08");
	scene_08.add(sc8_sampleScale.set("sample scale", sc8_sampleScale, 0.0, 1000));
	scene_08.add(sc8_xLoc.set("X location", sc8_xLoc, 0, ofGetWidth()));
	scene_08.add(sc8_yLoc.set("Y location", sc8_yLoc, 0, ofGetHeight()));
	scene_08.add(sc8_opac.set("circle opacity", sc8_opac, 0, 255));
	scene_08.add(sc8_move.set("move left or right", sc8_move, -10, 10));
	scene_08.add(sc8_startFreq.set("star fequency", sc8_startFreq, 0, 100));
	scene_08.add(sc8_freqStep.set("frequency step", sc8_freqStep, 1, 100));
	scene_08.add(sc8_smoothAmount.set("smooth amount", sc8_smoothAmount, 0.5, 0.99));
	gui.add(scene_08);

	//****** SCENE 9 GUI CONTROLS ********//
	scene_09.setName("SCENE 09");
	scene_09.add(sc9_allBrightness.set("bulb brightness", sc9_allBrightness, 0, 255));
	scene_09.add(sc9_fineFadeSpeed.set("Fine Fade Speed", sc9_fineFadeSpeed, 0.0, 1));
	scene_09.add(sc9_fadeSpeed.set("fade speed", sc9_fadeSpeed, 0.0, 3));
	scene_09.add(sc9_changeChance.set("chance of change", sc9_changeChance, 0.0, 300));

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
	midiInMe.listPorts();

	midiInMe.openPort(1);
	midiIn.openPort(0);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, true, true);
	midiInMe.ignoreTypes(false, true, true);

	// add ofApp as a listener
	midiIn.addListener(this);
	midiInMe.addListener(this);

	//midiIn.getPort();

	// print received messages to the console
	//midiIn.setVerbose(true);

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

	for (int i = sc1_startFreq; i<numOfSamples; i += sc1_freqStep) {
		int x = 0; // ofMap(i, 0, numOfSamples - 1, 0, ofGetWidth() - sampleWidth);
		int y = 0;

		//smoothing for fft values
		float value = samples[i] * sc1_sampleScale;// *= 0.993f;
		fftSmooth[i] *= sc1_smoothAmount;
		if (fftSmooth[i] < value) {
			fftSmooth[i] = value;
		}

		int w = 1 + abs(-fftSmooth[i]);
		//int h = -samples[i] * sampleHeight;

		//ofDrawEllipse(x, y, w, w);
		ofDrawEllipse(0, 0, w, w);
	}

}

//-------------------------------------------------------------- SCENE 5
void ofApp::drawFFT_scene5() {

	ofSetColor(255, sc5_opac);
	ofPushMatrix();
	ofTranslate(sc5_FFTxLoc, sc5_FFTyLoc);
	drawSamples_scene5(fftChannelL.getFftNormData());
	ofPopMatrix();

	//uncomment to draw right channel
	/*
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() * 0.75);
	drawSamples(fftChannelR.getFftNormData());
	ofPopMatrix();
	*/
}

void ofApp::drawSamples_scene5(vector<float> samples) {

	int sampleWidth = ofGetWidth() / samples.size();
	int sampleHeight = ofGetHeight() / 2;
	int numOfSamples = samples.size();

	for (int i = sc5_startFreq; i<numOfSamples; i += sc5_freqStep) {
		int x = 0; // ofMap(i, 0, numOfSamples - 1, 0, ofGetWidth() - sampleWidth);
		int y = 0;

		//smoothing for fft values
		float value = samples[i] * sc5_sampleScale;// *= 0.993f;
		fftSmooth[i] *= sc5_smoothAmount;
		if (fftSmooth[i] < value) {
			fftSmooth[i] = value;
		}

		int w = abs(-fftSmooth[i]);
		//int h = -samples[i] * sampleHeight;

		//ofDrawEllipse(x, y, w, w);
		ofDrawRectangle(0, 0, w, 500);
		ofDrawRectangle(0, 0, -w, 500);
	}

}



//-------------------------------------------------------------- SCENE 6
void ofApp::drawFFT_scene6() {

	ofSetColor(255, sc6_opac);
	ofPushMatrix();
	ofTranslate(sc6_xLoc, sc6_yLoc);
	drawSamples_scene6(fftChannelL.getFftNormData());
	ofPopMatrix();

	//uncomment to draw right channel
	/*
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() * 0.75);
	drawSamples(fftChannelR.getFftNormData());
	ofPopMatrix();
	*/
}

void ofApp::drawSamples_scene6(vector<float> samples) {

	int sampleWidth = ofGetWidth() / samples.size();
	int sampleHeight = ofGetHeight() / 2;
	int numOfSamples = samples.size();

	for (int i = 0; i<numOfSamples; i += sc6_freqStep) {
		int x = 0; // ofMap(i, 0, numOfSamples - 1, 0, ofGetWidth() - sampleWidth);
		int y = 0;

		//smoothing for fft values
		float value = samples[i] * sc6_sampleScale;// *= 0.993f;
		fftSmooth[i] *= sc6_smoothAmount;
		if (fftSmooth[i] < value) {
			fftSmooth[i] = value;
		}

		int w = 1 + abs(-fftSmooth[i]);
		//int h = -samples[i] * sampleHeight;

		//ofDrawEllipse(x, y, w, w);
		ofDrawEllipse(0, 0, w, w);
	}

}

//-------------------------------------------------------------- SCENE 8
void ofApp::drawFFT_scene8() {

	ofSetColor(255, sc8_opac); //opacity can be used for midi key
	ofPushMatrix();
	ofTranslate(sc8_xLoc, sc8_yLoc);
	drawSamples_scene8(fftChannelL.getFftNormData());
	ofPopMatrix();

	//uncomment to draw right channel
	/*
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() * 0.75);
	drawSamples(fftChannelR.getFftNormData());
	ofPopMatrix();
	*/
}

void ofApp::drawSamples_scene8(vector<float> samples) {

	int sampleWidth = ofGetWidth() / samples.size();
	int sampleHeight = ofGetHeight() / 2;
	int numOfSamples = samples.size();

	for (int i = sc8_startFreq; i<numOfSamples; i += sc8_freqStep) {
		int x = 0; // ofMap(i, 0, numOfSamples - 1, 0, ofGetWidth() - sampleWidth);
		int y = 0;

		//smoothing for fft values
		float value = samples[i] * sc8_sampleScale;// *= 0.993f;
		fftSmooth[i] *= sc1_smoothAmount;
		if (fftSmooth[i] < value) {
			fftSmooth[i] = value;
		}

		int w = 1 + abs(-fftSmooth[i]);
		//int h = -samples[i] * sampleHeight;

		//**scene_8 specific
		for (int i = 0; i<NSPOTS; i++) {
			mySpots[i].draw(w);
		}

	}

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

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
	midiInMe.closePort();
	midiInMe.removeListener(this);
}
