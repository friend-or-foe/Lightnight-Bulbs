#include "Spinners.h"



Spinners::Spinners()
{
}


Spinners::~Spinners()
{
}

void Spinners::setup(int _ID, ofImage _myImage) {

	ID = _ID;
	x = 1000;
	y = 500;

	myImage = _myImage;

	myImage.setAnchorPercent(0.5, 0.5);

}

void Spinners::update() {


}

void Spinners::draw() {


	if (ID == 8) {
		ofRotate(180);
	}
	myImage.draw(0, 0);


}