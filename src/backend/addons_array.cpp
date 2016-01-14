#include "addons_array.hpp"

AddonsArray::AddonsArray() {
  // default intervals
  cycleInterval = 1000000; //1s
  isRunning = true;
}

void AddonsArray::start() {
  logArray->log("AddonsArray", "start");

  for (auto itr = addons.begin(); itr != addons.end(); ++itr) {
      (*itr)->measTypeArray = measTypeArray;
      (*itr)->logArray = logArray;
      (*itr)->setup();
  }

  while(isRunning) {
    for (auto itr = addons.begin(); itr != addons.end(); ++itr) {
        (*itr)->perform();
    }

    Helper::longSleep(cycleInterval);
  }
}

void AddonsArray::stop() {
  logArray->log("AddonsArray", "stop");
}
