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
  
  MeasType *meas;
  ActionType *action;
  
  bool check();
  
private:
};

#include "overseer.cpp"
#endif
