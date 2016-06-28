#include "objects.h"
#include <iostream>

trID track::internalID = 0;

track::track(): trackID(track::internalID++),fullyPropagated(false), allPlaneHit(false) {}

track source::getTrack(){
	track tr;
    tr.currPos.x = pos.x+intensityDist.getRandomNumber();
    tr.currPos.z = pos.z;
	tr.hits.push_back(tr.currPos);
    tr.angle = angleDist.getRandomNumber();
    tr.currentPlane = -1;
	return tr;
}	
