#ifndef OVERSEER
#define	OVERSEER

#include <string>
#include <ctime>

#include "action_type.hpp"
#include "meas_type.hpp"

class Overseer {
public:
  std::string name; // name of overseer
  std::string actionName; // name of action which execute when conditions are met
  std::string measName; // name of measurements which should observe

  double thresholdValue; // execute if value is higher than this
  bool isMax; // when true execute only if higher
  unsigned long int windowSize; // use average of measurements
  unsigned char priority; // used in frontend

  // related to execution time buffer
  unsigned long long minExecInterval; // allow execution only when X mili seconds passed since previous execution
  unsigned int maxTimeBufferSize; // how many execution times should be stored in
  std::vector < unsigned long long > timeBuffer;
  unsigned long long lastExecuteTime();
  void markExecutionTime();
  bool checkLastExecutionTime();

  MeasType *meas;
  ActionType *action;

  Overseer();
  bool check();
  double currentValue();
  unsigned int execute();
  std::string toJson();
  std::string timeBufferToJson();

  void logInfo(std::string log); // overwritten color


private:
};

//#include "overseer.cpp"
#endif
