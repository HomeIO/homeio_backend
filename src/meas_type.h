#ifndef MEAS_TYPE_H
#define	MEAS_TYPE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MeasType {
public:
  MeasType(string);
  int addRaw(unsigned int);
  int assignBufferSize(unsigned int);
	
private:
  // variables
  
  string name;
  std::vector < unsigned int > buffer;
    
};

#endif	/* TCP_SERVER_H */
