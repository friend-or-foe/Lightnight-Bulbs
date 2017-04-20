#pragma once

#include "ofMain.h"
#include "bulbClass.h"
#include "ofxDmx.h" //include ofxDM addon
#include "ofxGui.h" //include GUI addon
#include "ofxXmlSettings.h"

#include "ofxFFTBase.h"

#define NBULBS 70

//define scene options
typedef enum {
	SCENE_1, // these can have any name you want, eg SCENE_BREATH etc  
	SCENE_2,
	SCENE_3
} Scene;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	///--------- XML STUFF --------------//
	void loadButtonPressed(); //load button for bulb locations
	void saveButtonPressed(); //load button for bulb locations
	void exit(); //remove listeners on app exit
	
	///--------- GENERAL PROGRAM STUFF --------------//
	Bulb myBulb[NBULBS]; //create bulb array

	ofImage tmpImage; //image for screengrab to be saved to. The bulbs pick their colour from this
	ofColor tmpCol; //stores the colour value to pass to bulbs

	////Create DMX object
	ofxDmx dmx;

	//Import floor plan image
	ofImage plan;
	int planWidth; //width of plan image
	int planHeight; //height of plan image
	int planOffsetX; //amount to offset drawing from left edge of stage
	int planOffsetY; //amount to offset drawing from top edge of stage


	///--------- GUI STUFF --------------//

	ofxPanel gui;

	ofParameterGroup master;

	ofParameter <float> masterBrightness = 255;
	ofParameter<float> bulbSize = 20.0;
	ofParameter<bool> drawPlan;
	ofxButton loadBulbLocations;
	ofxButton saveBulbLocations;

	///--------- FFT STUFF --------------//
	void initAudio();
	void drawSamples(vector<float> samples);
	void audioIn(float * input, int bufferSize, int nChannels);
	void drawFFT_scene1();

	ofSoundStream soundStream;
	vector<float> samplesChannelL;
	vector<float> samplesChannelR;

	float * fftSmooth;
	float sampleScale = 600;

	ofxFFTBase fftChannelL;
	ofxFFTBase fftChannelR;

	///--------- SCENE STUFF --------------//
	Scene myScene = SCENE_1;
	void mainScene_1();
};
