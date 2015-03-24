#ifndef OVERSEER
#define	OVERSEER

#include <string>
#include <ctime>

#include "action_type.hpp"
#include "meas_type.hpp"

using namespace std;

class Overseer {
public:
  string name; // name of overseer
  string actionName; // name of action which execute when conditions are met
  string measName; // name of measurements which should observe
  
  double thresholdValue; // execute if value is higher than this
  bool isMax; // when true execute only if higher
  unsigned int windowSize; // use average of measurements
  
  // related to execution time buffer
  unsigned long int minExecInterval; // allow execution only when X seconds passed since previous execution
  unsigned int maxTimeBufferSize; // how many execution times should be stored in
  vector < unsigned long int > timeBuffer;
  void markExecutionTime();
  bool checkLastExecutionTime();
  
  MeasType *meas;
  ActionType *action;
  
  Overseer();
  bool check();
  double currentValue();
  unsigned int execute();
  string toJson();
  
  void logInfo(string log); // overwritten color

  
private:
};

#include "overseer.cpp"
#endif
