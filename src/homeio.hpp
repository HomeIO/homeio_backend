#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>

#include "meas_type.hpp"

using namespace std;

class HomeIO {
public:
  HomeIO();
  int start();
  void *measStart(void *arguments);
  
  IoProxy *ioProxy;
  std::vector <MeasType> measTypes;
};

#include "homeio.cpp"
#endif
