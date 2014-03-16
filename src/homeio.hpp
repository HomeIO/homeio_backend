#ifndef HOMEIO
#define	HOMEIO

#include "meas_type.hpp"

using namespace std;

class HomeIO {
public:
  HomeIO();
  int start();
  
  IoProxy *ioProxy;
  std::vector <MeasType> measTypes;
};

#include "homeio.cpp"
#endif
