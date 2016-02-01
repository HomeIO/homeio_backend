#ifndef OVERSEER_ARRAY
#define	OVERSEER_ARRAY

#include <iostream>
#include <vector>
#include <mutex>

#include "overseer.hpp"
#include "../action/action_type_array.hpp"
#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"

class OverseerArray {
 public:
  OverseerArray();
  unsigned int add(Overseer *m);
  void start();
  void stop();
  Overseer *byName(std::string s);

  std::shared_ptr<MeasTypeArray> measTypeArray;
  
  ActionTypeArray *actionTypeArray;
  LogArray *logArray;
  std::vector <Overseer> overseers;
  unsigned long int cycleInterval;

  unsigned long long usDelay;

  bool isRunning;
  bool ready;
  std::mutex shutdownMutex;
};

//#include "overseer_array.cpp"
#endif
