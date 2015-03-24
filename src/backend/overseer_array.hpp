#ifndef OVERSEER_ARRAY
#define	OVERSEER_ARRAY

#include <iostream>
#include <vector>
#include "overseer.hpp"
#include "action_type_array.hpp"
#include "meas_type_array.hpp"

using namespace std;

class OverseerArray {
public:
  OverseerArray();
  unsigned int add(Overseer *m);
  int start();
  Overseer *byName(string s);
  void logInfo(string log); // overwritten color
  
  MeasTypeArray *measTypeArray;
  ActionTypeArray *actionTypeArray;
  std::vector <Overseer> overseers;
  unsigned long int cycleInterval;
};

#include "overseer_array.cpp"
#endif
