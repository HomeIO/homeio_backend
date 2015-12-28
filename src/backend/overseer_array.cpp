#include "overseer_array.hpp"

OverseerArray::OverseerArray() {
  std::vector <Overseer> overseers;
  cycleInterval = 1000000;
  usDelay = 200000; // wait 2s to warm up

  isRunning = true;
}

void OverseerArray::logInfo(std::string log) {
  Helper::logWithColor(log, MAGENTA);
}

unsigned int OverseerArray::add(Overseer *o) {
  overseers.push_back(*o);
  Helper::logInfo("Overseer added: '" + o->name + "' (" + std::to_string(overseers.size()) + " total overseers)");
  return 0;
}

Overseer *OverseerArray::byName(std::string s) {
  for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}

void OverseerArray::start() {
  Helper::longSleep(usDelay);
  // wait for enough measurements
  measTypeArray->delayTillReady();


  Helper::logInfo("OverseerArray start");

  for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    it->meas = measTypeArray->byName(it->measName);
    it->action = actionTypeArray->byName(it->actionName);
    it->check();
  }

  while(isRunning) {
    shutdownMutex.lock();
    Helper::logInfo("OverseerArray loop start");

    for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
      it->check();
    }

    Helper::logInfo("OverseerArray loop end");
    shutdownMutex.unlock();

    // check overseers every next X useconds
    Helper::longSleep(cycleInterval);
  }
}

void OverseerArray::stop() {
  shutdownMutex.lock();
  Helper::logInfo("OverseerArray - stop");
}
