#ifndef plane_h
#define plane_h

#include "dist.h"
#include "kalDefines.h"

class plane {
  public:
	plane(): angleDist(0,2) {};
	point pos;
	double angle;
	double size;
	double radLen;
	dist<float> angleDist;
	plID ID;
};
#endif
