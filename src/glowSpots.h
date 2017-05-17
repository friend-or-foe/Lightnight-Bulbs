#pragma once

#include "ofMain.h" //included to have reference to the openFrameworks framework

class glowSpots
{
public:
	glowSpots();
	~glowSpots();

	void setup(); //arguments you want to pass must have temp variable names inserted here
	void update(int _move); //int _numSpots, float _x, float _y, int _spacing, 
	void draw(float _diameter);

    ofPoint pos;

};

