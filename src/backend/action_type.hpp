#ifndef ACTION_TYPE_H
#define	ACTION_TYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "helpers.hpp"
#include "io_proxy.hpp"

using namespace std;

class ActionType {
public:
  ActionType();
  unsigned int execute();
  string toJson();
  string historyToJson();
  void logInfo(string log); // overwritten color
  unsigned long long lastExecutedAt();
  bool wasExecuted();

  string name; // name of action
  char command; // at this moment only 1 byte commands
  char responseSize; // amount of bytes returned from uC, normally 1 or 2
  unsigned int responseOkay; // value returned from uC when everything is okay

  unsigned long int executionCount;
  unsigned long int failedCount;

  // time buffer
  unsigned int maxTimeBufferSize; // how many execution times should be stored in
  vector < unsigned long long > timeBuffer;
  void markExecutionTime();

  IoProxy *ioProxy;

};

//#include "action_type.cpp"
#endif
