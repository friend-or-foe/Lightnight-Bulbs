#include "vertBars.h"



vertBars::vertBars()
{
}


vertBars::~vertBars()
{
}

Bar::Bar()
{
}

void Bar::setup(int _ID, float _x, float _y) {

}

void Bar::update(float _x, float _w) {

	x = _x;
	width = _w;

	



}

void Bar::draw(float _fadeSpeed) {

	ofSetColor(255, bright);
	ofDrawRectangle(x, y, width, 600);

	bright -= _fadeSpeed;
}