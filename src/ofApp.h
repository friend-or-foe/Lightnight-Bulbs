#pragma once

#include "ofMain.h"
#include "bulbClass.h"
#include "Spinners.h"
#include "centCirc.h"
#include "vertBars.h"
#include "ellipse.h"
#include "MidiShapes.h"
#include "glowSpots.h"

#include "ofxDmx.h" //include ofxDM addon
#include "ofxGui.h" //include GUI addon
#include "ofxXmlSettings.h"
#include "ofxMidi.h" 

#include "ofxFFTBase.h"

#define NBULBS 70
#define NSPINNERS 16
#define NBARS 32
#define NELLIPSES 60
#define NSHAPES 25
#define NSPOTS 10


//define scene options
typedef enum {
	SCENE_1, // these can have any name you want, eg SCENE_BREATH etc  
	SCENE_2,
	SCENE_3,
	SCENE_4,
	SCENE_5,
	SCENE_6,
	SCENE_7,
	SCENE_8,
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
	ofxMidiIn midiIn; //Adams MIDI in (sending Channel 2 computer, channel 3 drums)
	ofxMidiIn midiInMe; //Korg Keyboard in (sending on Channel 1)
	ofxMidiMessage midiMessage;
	stringstream MIDItext;

	///--------- GENERAL PROGRAM STUFF --------------//
	Bulb myBulb[NBULBS]; //create bulb array
	Spinners mySpinner[NSPINNERS];
	Bar myBars[NBARS];
	ellipse myEllipses[NELLIPSES];
	MidiShapes myShapes[NSHAPES];
	glowSpots mySpots[NSPOTS];

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

	int testBulb = 0;
	///--------- GUI STUFF --------------//

	void initGUI();

	ofxPanel gui;

	ofParameterGroup master;
	ofParameterGroup scene_01; //controller group scene 1
	ofParameterGroup scene_02; //controller group scene 2
	ofParameterGroup scene_03; //controller group scene 3
	ofParameterGroup scene_04; //controller group scene 4
	ofParameterGroup scene_05; //controller group scene 5
	ofParameterGroup scene_06; //controller group scene 6
	ofParameterGroup scene_07; //controller group scene 7

	ofParameterGroup scene_08; //controller group scene 8

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
	void drawSamples_scene5(vector<float> samples);
	void drawSamples_scene6(vector<float> samples);
	void drawSamples_scene8(vector<float> samples);

	void audioIn(float * input, int bufferSize, int nChannels);
	void drawFFT_scene1();
	void drawFFT_scene5();
	void drawFFT_scene6();
	void drawFFT_scene8();

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
	ofParameter<int> sc1_startFreq = 0;
	ofParameter<int> sc1_freqStep = 10;
	ofParameter<float> sc1_smoothAmount = 0.98f;

	//---------------------------------------- scene 02
	ofImage sc2_grad[8];
	float sc2_baseRot = 0;
	ofParameter<int> sc2_xLoc = 1163;
	ofParameter<int> sc2_yLoc = 362;
	ofParameter<float> sc2_rotSpeed = 4;
	ofParameter<float> sc2_scale = 1;
	ofParameter<float> sc2_opac = 255;
	ofParameter<int> sc2_startFreq = 0;
	ofParameter<int> sc2_freqStep = 10;
	ofParameter<float> sc2_smoothAmount = 0.90f;
	
	//---------------------------------------- scene 03
	vector <centCirc> myCirc; //vector array for adding/storing particles
	ofParameter<int> sc3_xLoc = 1163;
	ofParameter<int> sc3_yLoc = 362;
	ofParameter<float> sc3_gate = 0.2;
	ofParameter<int> sc3_count = 4; //count down gap before next particle can be triggered
	int sc3_countDown = 4; //keeps track of count
	ofParameter<float> sc3_velocityMultiplier = 5;
	ofParameter<int> sc3_lifeSpan = 80; //life of particle
	ofParameter<float> sc3_range = 64.0; //range that is affected by line

	//---------------------------------------- scene 04
	int plinkCount = 0; //keep traack of number of 'plink' shapes on screen
	int bassCount = 10; //keep traack of number of 'Bass' shapes on screen
	int keyCount = 19; //keep traack of number of 'key' shapes on screen

	ofParameter<float> sc4_leftEdge = 300; //left edge border
	ofParameter<float> sc4_rightEdge = 1100; //right edge border
	ofParameter<float> sc4_topEdge = 200; //top edge border
	ofParameter<float> sc4_botEdge = 400; //bottom edge border

	//GUI
	ofParameter<float> sc4_fade = 10;
	ofParameter<float> sc4_plinkBright = 127;
	ofParameter<float> sc4_bassBright = 255;
	ofParameter<float> sc4_keyBright = 255;

	ofParameter<float> sc4_plinkMin = 50; //minimum size for plink Shapes
	ofParameter<float> sc4_plinkMax = 300; //maximum size for plink shapes
	//---------------------------------------- scene 05
	
	ofParameter<int> sc5_xLoc = 600;
	ofParameter<int> sc5_yLoc = 362;
	ofParameter<int> sc5_numBars = 11;
	ofParameter<int> sc5_width = 1200;
	ofParameter<float> sc5_fade = 10;

	ofParameter<bool> sc5_drawFFT = false;
	ofParameter<int> sc5_FFTxLoc = 1065;
	ofParameter<int> sc5_FFTyLoc = 50;
	ofParameter<int> sc5_opac = 30;
	ofParameter<float> sc5_sampleScale = 600;
	ofParameter<int> sc5_startFreq = 10;
	ofParameter<int> sc5_freqStep = 10;
	ofParameter<float> sc5_smoothAmount = 0.98f;
	//---------------------------------------- scene 06
	//GUI controllable
	ofParameter<float> sc6_sampleScale = 200;
	ofParameter<int> sc6_xLoc = 1163;
	ofParameter<int> sc6_yLoc = 362;
	ofParameter<float> sc6_opac = 20;
	ofParameter<int> sc6_freqStep = 10;
	ofParameter<float> sc6_smoothAmount = 0.98f;

	ofParameter<float> sc6_russBright = 125;
	ofParameter<float> sc6_russFade = 10;

	//---------------------------------------- scene 07
	ofParameter<int> sc7_xLoc = 1163;
	ofParameter<int> sc7_yLoc = 362;
	ofParameter<float> sc7_xStart = 100;
	ofParameter<float> sc7_yStart = 50;
	ofParameter<float> sc7_xSpread = 60;
	ofParameter<float> sc7_ySpread = 30;

	//---------------------------------------- scene 08
	ofParameter<float> sc8_sampleScale = 300;
	ofParameter<float> sc8_xLoc = 300;
	ofParameter<float> sc8_yLoc = 85;
	ofParameter<float> sc8_opac = 20;
	ofParameter<int> sc8_move = 0;
	ofParameter<int> sc8_startFreq = 0;
	ofParameter<int> sc8_freqStep = 10;
	ofParameter<float> sc8_smoothAmount = 0.98f;
	int sc8_spacing = sc8_sampleScale;


	//---------------------------------------- scene 09
	ofParameter<int> sc9_allBrightness = 0;
	ofParameter<float> sc9_fineFadeSpeed = 0.01;
	ofParameter<float> sc9_fadeSpeed = 0.0;
	ofParameter<float> sc9_changeChance = 0;
	int sc9_flashCount = 0;

	//---------------------------------------- scene 00
	ofParameter<int> sc0_allBrightness = 0;
	

	///--------- SCENE STUFF --------------//
	Scene myScene = SCENE_0;
	void mainScene_1(); //expanding centre circle
	void mainScene_2(); //spinning gradients
	void mainScene_3(); //wipe objects
	void mainScene_4(); //MIDI Test
	void mainScene_5(); //MIDI bars
	void mainScene_6(); //place frequencies
	void mainScene_7(); //elliptical orbiters

	void mainScene_8(); //scott glowspots


	void mainScene_9(); //glimmer
	void mainScene_0(); //test scene Fade all
};
