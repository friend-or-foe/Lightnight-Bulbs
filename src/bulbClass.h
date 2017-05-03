#pragma once

#include "ofMain.h" //included to have reference to the openFrameworks framework
#include "ofxDmx.h" //include ofxDM addon

//test change for github

class bulbClass
{
public: // place public functions or variables declared here
	bulbClass();
	~bulbClass();

};

class Bulb {

public: // place public functions or variables declared here

		//methods, equivalent to specific functions of your class objects
	void setup(int _ID, int _planWidth, int planHeight, int _offX, int _offY, float _bulbSize); //arguments you want to pass must have temp variable names inserted here
	void update(float _bulbSize);
	void mousePressed();
	void mouseDragged();
	void mouseReleased();

	//scene draw functions
	void draw_sc1(int _tmpCol); //scene 1
	void draw_sc3(float _tmpDist); //scene 1
	void draw_sc4(); //scene 4

	void draw_sc9(int _tmpCol, float _fineFade, float _fadeSpeed); //Scene 0
	void draw_sc0(int _tmpCol); //Scene 0 is the test scene - all bulbs controlled by a master fader.


	void setLoc(int _x, int _y);
	//variables

	//****** POSITION AND DRAG STUFF ******//
	int dmxID; //ID to store DMX number = list position + 1 as DMX channels start at 1 not 0
	float x;	//position
	float y;
	float difx; //work out drag position x
	float dify; //work out drag position y
	bool bOver; //check if mouse is over bool
	bool locked; //lock if clicked

	//****** LOAD AND SAVE POSITIONS ******//


	//****** APPEARENCE PROPERTIES  ******//
	int dim;			//size
	ofColor color;		//Line colour
						//ofColor fillColor;	//fill color
	unsigned char colorGet[3]; //colourGet for fill
	int dmxLightVal;	//brightness value for light. Passed to dmx

	float txtOffsetX; //text offset (x) for number Identifier
	float txtOffsetY; //text offset (y) for number Identifier



					  //float bdifx
	Bulb(); //constructor - used to initialise object, if
			//no properties are passed the program sets them to the default value


	bool myTrigger = false;
};

