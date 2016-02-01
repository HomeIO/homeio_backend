#include "overseer_array.hpp"

OverseerArray::OverseerArray() {
  cycleInterval = 1000000;
  usDelay = 10000;

  isRunning = true;
  ready = false;
}

unsigned int OverseerArray::add(std::shared_ptr<Overseer> o) {
  overseers.push_back(o);
  logArray->log("Overseer", "added: '" + o->name + "' (" + std::to_string(overseers.size()) + " total overseers)");
  return 0;
}

std::shared_ptr<Overseer> OverseerArray::byName(std::string s) {
  for(std::vector<std::shared_ptr<Overseer>>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    std::shared_ptr<Overseer> overseer = *it;
    if (overseer->name == s) {
      return overseer;
    }
  }
  return NULL;
}

void OverseerArray::start() {
  Helper::longSleep(usDelay);
  // wait for enough measurements
  measTypeArray->delayTillReady();

  logArray->log("Overseer", "start");

  for(std::vector<std::shared_ptr<Overseer>>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    std::shared_ptr<Overseer> overseer = *it;
    overseer->meas = measTypeArray->byName(overseer->measName);
    overseer->action = actionTypeArray->byName(overseer->actionName);
    overseer->logArray = logArray;
    overseer->check();
  }

  ready = true;

  while(isRunning) {
    shutdownMutex.lock();
    logArray->log("Overseer", "loop start");

    for(std::vector<std::shared_ptr<Overseer>>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
      std::shared_ptr<Overseer> overseer = *it;
      overseer->check();
    }

    logArray->log("Overseer", "loop end");
    shutdownMutex.unlock();

    // check overseers every next X useconds
    Helper::longSleep(cycleInterval);
  }
}

void OverseerArray::stop() {
  isRunning = false;
  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("Overseer", "stop");
}
