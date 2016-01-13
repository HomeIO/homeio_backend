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
  logArray->log("Overseer", "added: '" + o->name + "' (" + std::to_string(overseers.size()) + " total overseers)");
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


  logArray->log("Overseer", "start");

  for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    it->meas = measTypeArray->byName(it->measName);
    it->action = actionTypeArray->byName(it->actionName);
    it->logArray = logArray;
    it->check();
  }

  while(isRunning) {
    shutdownMutex.lock();
    logArray->log("Overseer", "loop start");

    for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
      it->check();
    }

    logArray->log("Overseer", "loop end");
    shutdownMutex.unlock();

    // check overseers every next X useconds
    Helper::longSleep(cycleInterval);
  }
}

void OverseerArray::stop() {
  shutdownMutex.lock();
  logArray->log("Overseer", "stop");
}
