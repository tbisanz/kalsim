#ifndef objects_h
#define objects_h

#include<string>
#include<random>
#include<iostream>

const float PI = 3.14159265359;

using plID = int;
using trID = int;

template<typename T>
class dist {
  public:
	dist(double mean, double sigma);
	dist(dist const & copy);
	T getRandomNumber() const;
  private:
	std::random_device rd;
	mutable std::default_random_engine generator;
	mutable std::normal_distribution<T> distri;
};

template<typename T>
dist<T>::dist(double mean, double sigma): distri(mean, sigma){
	generator.seed(rd());	
}

template<typename T>
dist<T>::dist(dist const & copy): distri(copy.distri.mean(), copy.distri.stddev()){
	generator.seed(rd());	
}

template<typename T>
T dist<T>::getRandomNumber() const {
	return distri(generator);
}

class plane {
  public:
	plane(): angleDist(0,2) {};
	double posX;
	double posZ;
	double angle;
	double size;
	double radLen;
	dist<float> angleDist;
	plID ID;
};

class track {
  private:
	static trID internalID;
	std::vector<plane> const* planeVec;
  public:
	track();
	double posX;
	double posZ;
	double angle;
	plID currentPlane;
	trID trackID;
	std::vector<plane>::const_iterator nextPlane;

	void setPlaneVec(std::vector<plane> const& vec) {
		planeVec = &vec;
		nextPlane = planeVec->cbegin();
	}
		
	bool propagate() {
		if(nextPlane != planeVec->end()) {	
			double cosTheta = std::cos(angle*PI/180); 
			double sinTheta = std::sin(angle*PI/180);
			double cosPhi = std::cos(nextPlane->angle*PI/180);
			double sinPhi = std::sin(nextPlane->angle*PI/180);
			double planeX = nextPlane->posX;
			double planeZ = nextPlane->posZ;
			
			double term1 = posX-planeX+(planeZ-posZ)*sinTheta/cosTheta;
			double term2 = cosPhi-sinPhi*sinTheta/cosTheta;
			double v = term1/term2;

			double term3 = planeX-posX+(posZ-planeZ)*cosPhi/sinPhi;
			double term4 = sinTheta-cosTheta*cosPhi/sinPhi;
			double t = term3/term4;
			
			double posZ1 = planeZ+v*sinPhi;
			double posX1 = planeX+v*cosPhi;

			double posZ2 = posZ+t*cosTheta;
			double posX2 = posX+t*sinTheta;
			std::cout << "next angle: " << nextPlane->angle << std::endl;
			std::cout << "Z/X1: " << posZ1 << "," << posX1 << std::endl;	
			std::cout << "Z/X2: " << posZ2 << "," << posX2 << std::endl;	
			std::cout << "theta cos/sin " << cosTheta << "," << sinTheta << std::endl;	
			std::cout << "phi cos/sin " << cosPhi << "," << sinPhi << std::endl;	

			posZ = posZ1;
			posX = posX1;

			double dist = std::sqrt((posZ-planeZ)*(posZ-planeZ)+(posX-planeX)*(posX-planeX));
			if(dist > nextPlane->size/2) {
				return false;
			}
		
			//angle = angle+nextPlane->angleDist.getRandomNumber();
			nextPlane++;
			return true;
		} else {
			return false;
		}
	}
};

class source {
  public:
	source(double x, double z, dist<float> iDist, dist<float> aDist): intensityDist(iDist), angleDist(aDist), posX(x), posZ(z) {}
	dist<float> intensityDist;  
	dist<float> angleDist;
	double posX;
	double posZ;
	track getTrack();
};

#endif
