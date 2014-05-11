#ifndef MEAS_TYPE_H
#define	MEAS_TYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "io_proxy.hpp"
#include "meas_buffer.hpp"

using namespace std;

class MeasType {
public:
  MeasType();
  unsigned int fetch();
  
  MeasBuffer *buffer;
  
  unsigned int addRaw(unsigned int);
  
  time_t lastTime;
  
  string name; // name of measurements
  char command; // at this moment only 1 byte commands
  char responseSize; // amount of bytes returned from uC, normally 1 or 2
  IoProxy *ioProxy;
	
};

#include "meas_type.cpp"
#endif	/* TCP_SERVER_H */
