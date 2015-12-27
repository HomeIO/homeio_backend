#ifndef OVERSEER_ARRAY
#define	OVERSEER_ARRAY

#include <iostream>
#include <vector>
#include <mutex>

#include "overseer.hpp"
#include "action_type_array.hpp"
#include "meas_type_array.hpp"

using namespace std;

class OverseerArray {
public:
  OverseerArray();
  unsigned int add(Overseer *m);
  void start();
  void stop();
  Overseer *byName(string s);
  void logInfo(string log); // overwritten color

  MeasTypeArray *measTypeArray;
  ActionTypeArray *actionTypeArray;
  std::vector <Overseer> overseers;
  unsigned long int cycleInterval;

  unsigned long long usDelay;

  bool isRunning;
  mutex shutdownMutex;
};

//#include "overseer_array.cpp"
#endif
