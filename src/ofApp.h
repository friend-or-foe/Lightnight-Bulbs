#pragma once

#include "ofMain.h"
#include "bulbClass.h"
#include "ofxDmx.h" //include ofxDM addon
#include "ofxGui.h" //include GUI addon
#include "ofxXmlSettings.h"
#include "ofxMidi.h" 

#include "ofxFFTBase.h"

#define NBULBS 70

//define scene options
typedef enum {
	SCENE_1, // these can have any name you want, eg SCENE_BREATH etc  
	SCENE_2,
	SCENE_3,
	SCENE_9,
	SCENE_0,
} Scene;

class ofApp : public ofBaseApp, public ofxMidiListener {

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
	
	///--------- MIDI STUFF --------------//
	void initMIDI();
	void newMidiMessage(ofxMidiMessage& eventArgs);
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
	stringstream MIDItext;

	///--------- GENERAL PROGRAM STUFF --------------//
	Bulb myBulb[NBULBS]; //create bulb array

	ofImage tmpImage; //image for screengrab to be saved to. The bulbs pick their colour from this
	ofColor tmpCol; //stores the colour value to pass to bulbs

	////Create DMX object
	ofxDmx dmx;
	void sendDMXVals(int _bulbNum); //function to send DMX Values

	//Import floor plan image
	ofImage plan;
	int planWidth; //width of plan image
	int planHeight; //height of plan image
	int planOffsetX; //amount to offset drawing from left edge of stage
	int planOffsetY; //amount to offset drawing from top edge of stage


	///--------- GUI STUFF --------------//

	void initGUI();

	ofxPanel gui;

	ofParameterGroup master;
	ofParameterGroup scene_01; //controller group scene 1
	ofParameterGroup scene_02; //controller group scene 1

	ofParameterGroup scene_09; //controller group scene 9 - glimmer
	ofParameterGroup scene_00; //controller group scene 0

	ofParameter <float> masterBrightness = 255;
	ofParameter<float> bulbSize = 20.0;
	ofParameter<bool> drawPlan;
	ofxButton loadBulbLocations;
	ofxButton saveBulbLocations;

	///--------- FFT STUFF --------------//
	void initAudio();
	void drawSamples_scene1(vector<float> samples);
	void audioIn(float * input, int bufferSize, int nChannels);
	void drawFFT_scene1();

	ofSoundStream soundStream;
	vector<float> samplesChannelL;
	vector<float> samplesChannelR;

	float * fftSmooth;

	ofxFFTBase fftChannelL;
	ofxFFTBase fftChannelR;

	//---------------------------------------- scene 01
	//GUI controllable
	ofParameter<float> sc1_sampleScale = 600;
	ofParameter<int> sc1_xLoc = 1163;
	ofParameter<int> sc1_yLoc = 362;
	ofParameter<float> sc1_opac = 20;

	//---------------------------------------- scene 02
	ofImage sc2_grad[8];
	float sc2_baseRot = 0;
	ofParameter<int> sc2_xLoc = 1163;
	ofParameter<int> sc2_yLoc = 362;
	ofParameter<float> sc2_rotSpeed = 4;
	ofParameter<float> sc2_scale = 1;
	ofParameter<float> sc2_opac = 20;
	
	//---------------------------------------- scene 09
	ofParameter<int> sc9_allBrightness = 0;
	ofParameter<float> sc9_fadeSpeed = 0.1;
	ofParameter<float> sc9_noiseScale = 0;
	ofParameter<float> sc9_changeChance = 0;
	int sc9_flashCount = 0;

	//---------------------------------------- scene 00
	ofParameter<int> sc0_allBrightness = 0;
	

	///--------- SCENE STUFF --------------//
	Scene myScene = SCENE_1;
	void mainScene_1();
	void mainScene_2();

	void mainScene_9();
	void mainScene_0();
};
