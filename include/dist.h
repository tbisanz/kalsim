#ifndef dist_h
#define dist_h

#include<random>

#include "kalDefines.h"

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
#endif
