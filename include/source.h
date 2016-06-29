#ifndef source_h
#define source_h

#include "dist.h"
#include "track.h"
#include "kalDefines.h"

class source {
  public:
	source(double x, double z, dist<float> iDist, dist<float> aDist): intensityDist(iDist), angleDist(aDist){
		pos.x = x;	
		pos.z = z;	
	}
	dist<float> intensityDist;  
	dist<float> angleDist;
	point pos;
	track getTrack();
};
#endif
