#include "ellipse.h"



ellipse::ellipse()
{
}


ellipse::~ellipse()
{
}


void ellipse::setup(int _ID, float _xStart, float _yStart, float _xSpread, float _ySpread) {

	ID = _ID;
	int spreadMult = ID % 10;

	radX = _xStart + (spreadMult * _xSpread);
	radY = _yStart + (spreadMult * _ySpread);

	angle = ofRandom(360);
	rotSpeed = ofRandom(0.1, 2.0);

	width = 40;

}

void ellipse::update(float _xStart, float _yStart, float _xSpread, float _ySpread) {

	int spreadMult = ID % 10;

	radX = _xStart + (spreadMult * _xSpread);
	radY = _yStart + (spreadMult * _ySpread);

	angle+= rotSpeed;

	x = radX * cos(ofDegToRad(angle));
	y = radY * sin(ofDegToRad(angle));

}

void ellipse::draw() {

	ofSetColor(255);
	ofDrawEllipse(x, y, width, width);

}

