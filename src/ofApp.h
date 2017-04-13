#pragma once

#include "ofMain.h"
#include "bulbClass.h"
#include "ofxDmx.h" //include ofxDM addon
#include "ofxGui.h" //include GUI addon

#define NBULBS 5

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

	Bulb myBulb[NBULBS];

	ofImage tmpImage; //image for screengrab to be saved to. The bulbs pick their colour from this
	ofColor tmpCol; //stores the colour value to pass to bulbs

	////Create DMX object
	ofxDmx dmx;


	//Import floor plan image
	ofImage plan;
	int planOffsetX;
	int planOffsetY;

	//bool drawPlan = false; //draw floor plan to screen


	///--------- GUI STUFF --------------//

	ofxPanel gui;

	ofParameter<float> bulbSize = 20.0;
	ofParameter<bool> drawPlan;

};
