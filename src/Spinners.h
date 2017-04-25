#pragma once

#include "ofMain.h" //included to have reference to the openFrameworks framework


class Spinners
{
public:
	Spinners(); //constructor - used to initialise object, if
				//no properties are passed the program sets them to the default value
	~Spinners();

	//methods, equivalent to specific functions of your class objects
	void setup(int _ID, ofImage _myImage); //arguments you want to pass must have temp variable names inserted here
	void update();
	void draw();

	int ID; //location in list
	float x;	//position
	float y;

	ofImage myImage; //.png image to use
};

