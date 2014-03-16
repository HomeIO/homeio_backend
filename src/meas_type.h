#ifndef MEAS_TYPE_H
#define	MEAS_TYPE_H

#include <iostream>
#include <string>
#include <vector>

#include "io_proxy.h"

using namespace std;

class MeasType {
public:
  MeasType(string);
  unsigned int fetch();
  
  int addRaw(unsigned int);
  int assignBufferSize(unsigned int);
  
  char command; // at this moment only 1 byte commands
  char responseSize; // amount of bytes returned from uC, normally 1 or 2
  IoProxy *ioProxy;
	
private:
  // variables
  
  string name;
  std::vector < unsigned int > buffer;
    
};

#endif	/* TCP_SERVER_H */
