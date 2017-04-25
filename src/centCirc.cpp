#include "centCirc.h"



centCirc::centCirc()
{
}


centCirc::~centCirc()
{
}

void centCirc::setup(int _x, int _y, float _rad) {

	x = _x;
	y = _y;
	rad = _rad;

}

void centCirc::run() {


}

void centCirc::addParticle() {


}

void centCirc::update() {

	rad += 2;

	//if(rad > 300)

}

void centCirc::draw() {

	

	ofDrawCircle(x, y, rad, rad);
}
/*
bool centCirc::dead() {


}*/