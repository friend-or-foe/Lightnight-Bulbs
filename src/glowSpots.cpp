#include "glowSpots.h"



glowSpots::glowSpots()
{
}


glowSpots::~glowSpots()
{
}

void glowSpots::setup() {

    pos.set( 0,0,0 );

}

void glowSpots::update(int _move) {

    pos.x += _move;
    
    if (_move < 0){
    if (pos.x < 0){
        pos.x = 1500;
    }
    }else{
    if (pos.x > 1500){
        pos.x = 0;
    }
    }

}

void glowSpots::draw(float _diameter) {
    
    ofDrawEllipse(pos.x, pos.y, _diameter, _diameter);

}
