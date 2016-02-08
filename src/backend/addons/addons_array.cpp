#include "addons_array.hpp"

AddonsArray::AddonsArray() {
  // default intervals
  cycleInterval = 1000000; //1s
  isRunning = true;
  ready = false;
  changing = false;
  work = false;
}

void AddonsArray::start() {
  changing = true;

  logArray->log("AddonsArray", "start");

  work = true;
  for (auto itr = addons.begin(); itr != addons.end(); ++itr) {
    (*itr)->measTypeArray = measTypeArray;
    (*itr)->logArray = logArray;
    (*itr)->setup();
  }
  work = false;

  ready = true;
  changing = false;

  while(isRunning) {
    work = true;
    // logArray->log("AddonsArray", "cycle start");

    shutdownMutex.lock();
    for (auto itr = addons.begin(); itr != addons.end(); ++itr) {
      (*itr)->perform();
    }
    shutdownMutex.unlock();

    // logArray->log("AddonsArray", "cycle stop");
    work = false;

    Helper::longSleep(cycleInterval);
  }
}

void AddonsArray::stop() {
  isRunning = false;
  changing = true;

  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("AddonsArray", "stop");

  changing = false;
  ready = false;
}
