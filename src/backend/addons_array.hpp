#ifndef ADDONS_ARRAY
#define	ADDONS_ARRAY

#include <vector>
#include <mutex>

#include "helpers.hpp"
#include "addons/abstract_addon.hpp"
#include "meas_type_array.hpp"

using namespace std;

class AddonsArray {
public:
  AddonsArray();
  void start();
  void stop();
  void add(AbstractAddon *a);

  vector <std::unique_ptr<AbstractAddon>> addons;
  unsigned long int cycleInterval;
  bool isRunning;
  mutex shutdownMutex;

  MeasTypeArray* measTypeArray;
};

//#include "addons_array.cpp"
#endif
