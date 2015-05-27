#ifndef SPY_H
#define SPY_H

#include <string>
#include <vector>
#include "meas_type_array.hpp"

using namespace std;

class Spy {
public:
  Spy();
  void start();
  void announceAll();
  
  string hiveHost;
  string siteName;
  
  MeasTypeArray *measTypeArray;

  unsigned long int cycleInterval;
  unsigned long long lastTime;
  unsigned long long usDelay;
  
  bool quiet;
  bool enabled;
  
};

#include "spy.cpp"
#endif

