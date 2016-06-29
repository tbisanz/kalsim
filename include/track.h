#ifndef track_h
#define track_h

#include<vector>
#include "kalDefines.h"
#include "plane.h"

class track {
  private:
	static trID internalID;
	std::vector<plane> const* planeVec;
  public:
	track();
	point currPos;
	double angle;
	plID currentPlane;
	trID trackID;
	std::vector<plane>::const_iterator nextPlane;
	std::vector<point> hits;

	bool fullyPropagated;
	bool allPlaneHit;

	void setPlaneVec(std::vector<plane> const& vec) {
		planeVec = &vec;
		nextPlane = planeVec->cbegin();
	}
		
	bool propagate2next() {
		if(nextPlane != planeVec->end()) {	
			double cosTheta = std::cos(angle*PI/180); 
			double sinTheta = std::sin(angle*PI/180);
			double cosPhi = std::cos(nextPlane->angle*PI/180);
			double sinPhi = std::sin(nextPlane->angle*PI/180);
			double planeX = nextPlane->pos.x;
			double planeZ = nextPlane->pos.z;
			
			double term1 = currPos.x-planeX+(planeZ-currPos.z)*sinTheta/cosTheta;
			double term2 = cosPhi-sinPhi*sinTheta/cosTheta;
			double v = term1/term2;

			//double term3 = planeX-currPos.x+(currPos.z-planeZ)*cosPhi/sinPhi;
			//double term4 = sinTheta-cosTheta*cosPhi/sinPhi;
			//double t = term3/term4;
			
			double posZ1 = planeZ+v*sinPhi;
			double posX1 = planeX+v*cosPhi;

			//double posZ2 = currPos.z+t*cosTheta;
			//double posX2 = currPos.x+t*sinTheta;
			//std::cout << "next angle: " << nextPlane->angle << std::endl;
			//std::cout << "Z/X1: " << posZ1 << "," << posX1 << std::endl;	
			//std::cout << "Z/X2: " << posZ2 << "," << posX2 << std::endl;	
			//std::cout << "theta cos/sin " << cosTheta << "," << sinTheta << std::endl;	
			//std::cout << "phi cos/sin " << cosPhi << "," << sinPhi << std::endl;	

			currPos.x = posX1;
			currPos.z = posZ1;
		
			double dist = std::sqrt((currPos.z-planeZ)*(currPos.z-planeZ)+(currPos.x-planeX)*(currPos.x-planeX));
			if(dist > nextPlane->size/2) {
				fullyPropagated = true;
				return false;
			}
		
			hits.push_back(currPos);
			
			//angle = angle+nextPlane->angleDist.getRandomNumber();
			nextPlane++;
			return true;
		} else {
			fullyPropagated = true;
			allPlaneHit = true;
			return false;
		}
	}

	void propagateFully() {
		while (propagate2next());
	}

};
#endif
