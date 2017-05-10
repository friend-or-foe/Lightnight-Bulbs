#pragma once

#include "ofMain.h" //included to have reference to the openFrameworks framework

class MidiShapes
{
public:
	MidiShapes();
	~MidiShapes();

	void setup(int _ID, float _x, float _y); //arguments you want to pass must have temp variable names inserted here
	void update();
	void draw(float _fadeSpeed);

	//draw various shapes
	void drawPlink(float _fadeSpeed);
	void drawBass(float _fadeSpeed);
	void drawKey(float _fadeSpeed);

	//initiate new shapes
	void initPlink(int _ID, float _x, float _y, float _initBright);
	void initBass(int _ID, float _x, float _y, float _initBright);
	void initKey(int _ID, float _x, float _y, float _initBright);

	int ID;
	float x;
	float y;
	float width = 0;
	float height = 0;

	float bright = 0;

	float angle = 0;
	float rotSpeed = 0;

	bool myTrigger = false;

};

