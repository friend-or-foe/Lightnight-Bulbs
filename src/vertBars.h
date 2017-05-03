#pragma once
#include "ofMain.h" //included to have reference to the openFrameworks framework



class vertBars
{
public:
	vertBars();
	~vertBars();

	

};

class Bar {

public:

	void setup(int _ID, float _x, float _y); //arguments you want to pass must have temp variable names inserted here
	void update(float _x, float _w);
	void draw(float _fadeSpeed);

	float x;
	float y;
	float width;
	float bright = 0;


	bool myTrigger = false;

	Bar();

};