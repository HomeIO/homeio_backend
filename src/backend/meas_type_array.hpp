#ifndef MEAS_TYPE_ARRAY
#define	MEAS_TYPE_ARRAY

#include <iostream>
#include <vector>
#include "meas_type.hpp"
#include "io_proxy.hpp"

using namespace std;

class MeasTypeArray {
public:
  MeasTypeArray();
  unsigned int add(MeasType *m);
  MeasType *byName(string s);
  
  vector <MeasType> measTypes;
  IoProxy *ioProxy;
};

#include "meas_type_array.cpp"
#endif
