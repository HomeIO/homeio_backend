#ifndef OVERSEER
#define	OVERSEER

#include <string>
#include <ctime>

#include "../action/action_type.hpp"
#include "../meas/meas_type.hpp"
#include "../log/log_array.hpp"

class Overseer {
 public:
  bool enabled; // only checked if true
  std::string name; // name of overseer
  std::string actionName; // name of action which execute when conditions are met
  std::string measName; // name of measurements which should observe

  double thresholdValue; // execute if value is higher than this
  std::string thresholdValueFormatted(); // execute if value is higher than this - formatted version
  bool isMax; // when true execute only if higher
  unsigned long int windowSize; // use average of measurements
  unsigned char priority; // used in frontend

  double tempValue; // last calculated value
  bool tempResult; // last checked result
  std::string tempValueFormatted(); // last checked value formatted

  unsigned long long hitCount; // how many it was executed

  // related to execution time buffer
  unsigned long long minExecInterval; // allow execution only when X mili seconds passed since previous execution
  unsigned int maxTimeBufferSize; // how many execution times should be stored in
  std::vector < unsigned long long > timeBuffer;
  unsigned long long lastExecuteTime();
  unsigned long long currentTime();
  unsigned long long secondsTillLastExec();
  bool wasExecutedAtLeastOnce();
  void markExecutionTime();
  bool checkLastExecutionTime();

  std::shared_ptr<MeasType> measType;
  std::shared_ptr<ActionType> actionType;
  std::shared_ptr<LogArray> logArray;

  Overseer();
  bool check();
  double currentValue();
  unsigned int execute();
  std::string toJson();
  std::string timeBufferToJson();

 private:
};

//#include "overseer.cpp"
#endif
