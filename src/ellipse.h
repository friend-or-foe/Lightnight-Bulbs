#pragma once
#include "ofMain.h" //included to have reference to the openFrameworks framework

class ellipse
{
public:
	ellipse();
	~ellipse();

	void setup(int _ID, float _xStart, float _yStart, float _xSpread, float _ySpread); //arguments you want to pass must have temp variable names inserted here
	void update(float _xStart, float _yStart, float _xSpread, float _ySpread);
	void draw();

	int ID;

	float originX, originY; //origin

	float x;
	float y;
	float radX; //radius X
	float radY; //radius Y

	float angle; //angle
	float rotSpeed; //rotation speed

	float width;
	float bright = 0;


	bool myTrigger = false;

};

