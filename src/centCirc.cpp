#include "centCirc.h"



centCirc::centCirc()
{
}


centCirc::~centCirc()
{
}

void centCirc::setup(int _ID, int _x, int _y, float _rad, float _velocity, int _lifeSpan) {

	x = _x;
	y = _y;
	rad = _rad;

	ID = _ID;

	velocity = _velocity;

	myLifeSpan = _lifeSpan;
	myAge = myLifeSpan;

}

void centCirc::run() {


}

void centCirc::addParticle() {


}

void centCirc::update() {

	rad += velocity;
	if (myAge > 0) {
		myAge--;
	}
	//printf("IM Alive : %i\n", ID);
	//if(rad > 300)

}

void centCirc::draw() {

	

	ofDrawCircle(x, y, rad, rad);
}
/*
bool centCirc::dead() {


}*/