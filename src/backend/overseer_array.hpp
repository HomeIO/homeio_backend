#ifndef OVERSEER_ARRAY
#define	OVERSEER_ARRAY

#include <iostream>
#include <vector>
#include <mutex>

#include "overseer.hpp"
#include "action_type_array.hpp"
#include "meas_type_array.hpp"

class OverseerArray {
public:
  OverseerArray();
  unsigned int add(Overseer *m);
  void start();
  void stop();
  Overseer *byName(std::string s);
  void logInfo(std::string log); // overwritten color

  MeasTypeArray *measTypeArray;
  ActionTypeArray *actionTypeArray;
  std::vector <Overseer> overseers;
  unsigned long int cycleInterval;

  unsigned long long usDelay;

  bool isRunning;
  std::mutex shutdownMutex;
};

//#include "overseer_array.cpp"
#endif
