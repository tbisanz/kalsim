#include "track.h"

trID track::internalID = 0;

track::track(): trackID(track::internalID++),fullyPropagated(false), allPlaneHit(false) {}


