#ifndef MEAS_TYPE_H
#define	MEAS_TYPE_H

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <time.h>

#include "io_proxy.hpp"
#include "meas_buffer.hpp"
#include "meas_type_storage.hpp"

using namespace std;

class MeasType {
public:
  MeasType();
  unsigned int fetch();
  unsigned int addRaw(unsigned int);
  double rawToValue(unsigned int);
  double valueAt(unsigned long int i);
  unsigned int lastRaw();
  double lastValue();
  double lastValueFor(unsigned int);
  string toJson();
  void logInfo(string log); // overwritten color
  string storageBuffer(unsigned long long timeFrom, unsigned long long timeTo);

  
  void prepareFetch();
  string fetchString(unsigned int raw);

  MeasBuffer *buffer;
  
  string name; // name of measurements
  char command; // at this moment only 1 byte commands
  char responseSize; // amount of bytes returned from uC, normally 1 or 2
  
  double coefficientLinear;
  int coefficientOffset;
  
  bool started;
  string logPrefix;
  
  IoProxy *ioProxy;
  MeasTypeStorage *measTypeStorage;
  
  unsigned long int timeToIndex(unsigned long long t);
  string storageJson(unsigned long long timeFrom, unsigned long long timeTo);

	
};

#include "meas_type.cpp"
#endif	
