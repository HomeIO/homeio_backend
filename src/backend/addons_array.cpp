#include "addons_array.hpp"

AddonsArray::AddonsArray() {
  // default intervals
  cycleInterval = 1000000; //1s
  isRunning = true;
}

void AddonsArray::start() {
  logInfo("AddonsArray - start");
  for (auto itr = addons.begin(); itr != addons.end(); ++itr) {
      (*itr)->measTypeArray = measTypeArray;
      (*itr)->setup();
  }

  while(isRunning) {
    for (auto itr = addons.begin(); itr != addons.end(); ++itr) {
        (*itr)->perform();
    }

    longSleep(cycleInterval);
  }
}

void AddonsArray::stop() {
  logInfo("AddonsArray - stop");
}
