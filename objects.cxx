#include "objects.h"
#include <iostream>

trID track::internalID = 0;

track::track(): trackID(track::internalID++) {}

track source::getTrack(){
	track tr;
    tr.posX = posX+intensityDist.getRandomNumber();
    tr.posZ = posZ;
    tr.angle = angleDist.getRandomNumber();
    tr.currentPlane = -1;
	return tr;
}	
