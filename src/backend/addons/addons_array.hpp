#ifndef ADDONS_ARRAY
#define	ADDONS_ARRAY

#include <vector>
#include <mutex>
#include <memory>

#include "../utils/helper.hpp"
#include "abstract_addon.hpp"
#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"

class AddonsArray {
 public:
  AddonsArray();
  void start();
  void stop();
  void add(AbstractAddon *a);

  std::vector <std::unique_ptr<AbstractAddon>> addons;
  unsigned long int cycleInterval;
  bool isRunning;
  bool ready;
  bool changing;
  std::mutex shutdownMutex;

  std::shared_ptr<MeasTypeArray> measTypeArray;

  std::shared_ptr<LogArray> logArray;
};

//#include "addons_array.cpp"
#endif
