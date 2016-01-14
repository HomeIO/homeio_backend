#ifndef ACTION_TYPE_H
#define	ACTION_TYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "helper.hpp"
#include "io/io_proxy.hpp"
#include "log/log_array.hpp"

class ActionType {
public:
  ActionType();
  unsigned int execute();
  std::string toJson();
  std::string historyToJson();
  unsigned long long lastExecutedAt();
  bool wasExecuted();

  std::string name; // name of action
  unsigned char command; // at this moment only 1 byte commands
  unsigned char responseSize; // amount of bytes returned from uC, normally 1 or 2
  unsigned int responseOkay; // value returned from uC when everything is okay

  unsigned long int executionCount;
  unsigned long int failedCount;

  // time buffer
  unsigned int maxTimeBufferSize; // how many execution times should be stored in
  std::vector < unsigned long long > timeBuffer;
  void markExecutionTime();

  IoProxy *ioProxy;
  LogArray *logArray;

};

//#include "action_type.cpp"
#endif
