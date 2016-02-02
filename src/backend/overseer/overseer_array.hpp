#ifndef OVERSEER_ARRAY
#define	OVERSEER_ARRAY

#include <iostream>
#include <vector>
#include <mutex>
#include <memory>

#include "overseer.hpp"
#include "../action/action_type_array.hpp"
#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"

#define OVERSEER_ARRAY_STATUS_NULL 0
#define OVERSEER_ARRAY_STATUS_WAIT_FOR_MEAS 1

class OverseerArray {
 public:
  OverseerArray();
  unsigned int add(std::shared_ptr<Overseer> o);
  void start();
  void stop();
  std::shared_ptr<Overseer> byName(std::string s);
  std::string statusText();

  std::shared_ptr<MeasTypeArray> measTypeArray;
  std::shared_ptr<ActionTypeArray> actionTypeArray;

  std::shared_ptr<LogArray> logArray;
  std::vector <std::shared_ptr<Overseer>> overseers;
  unsigned long int cycleInterval;

  unsigned long long usDelay;

  bool isRunning;
  bool ready;
  bool changing;
  bool work;
  unsigned char intStatus;
  std::mutex shutdownMutex;
};

//#include "overseer_array.cpp"
#endif
