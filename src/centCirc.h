#pragma once

#include "ofMain.h" //included to have reference to the openFrameworks framework

class centCirc
{
public:
	centCirc();
	~centCirc();

	//methods, equivalent to specific functions of your class objects
	void setup(int _ID, int _x,int _y, float _rad, float _velocity, int _lifeSpan); //arguments you want to pass must have temp variable names inserted here
	void run();
	void addParticle();
	

	void update();
	void draw();

	

	int ID; //location in list
	float x;	//position
	float y;

	float rad;
	float velocity;

	int myLifeSpan; //how long to live
	int myAge; //measure current lifetime

};

