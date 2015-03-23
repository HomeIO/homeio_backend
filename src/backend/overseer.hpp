#ifndef OVERSEER
#define	OVERSEER

#include <string>
#include "action_type.hpp"
#include "meas_type.hpp"

class Overseer {
public:
  string name; // name of overseer
  string actionName; // name of action which execute when conditions are met
  string measName; // name of measurements which should observe
  
  double thresholdValue; // execute if value is higher than this
  bool isMax; // when true execute only if higher
  unsigned int windowSize; // use average of measurements
  
  MeasType *meas;
  ActionType *action;
  
  Overseer();
  bool check();
  double currentValue();
  unsigned int execute();
  
private:
};

#include "overseer.cpp"
#endif
