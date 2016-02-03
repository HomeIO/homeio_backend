#include "overseer_array.hpp"

OverseerArray::OverseerArray() {
  cycleInterval = 1000000;
  usDelay = 10000;

  isRunning = true;
  ready = false;
  changing = false;
  work = false;
  intStatus = OVERSEER_ARRAY_STATUS_NULL;
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
  changing = true;
  intStatus = OVERSEER_ARRAY_STATUS_WAIT_FOR_MEAS;

  //Helper::longSleep(usDelay);
  // wait for enough measurements

  measTypeArray->delayTillReady();
  logArray->log("Overseer", "start");

  work = true;
  for(std::vector<std::shared_ptr<Overseer>>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    std::shared_ptr<Overseer> overseer = *it;
    overseer->measType = measTypeArray->byName(overseer->measName);
    overseer->actionType = actionTypeArray->byName(overseer->actionName);
    overseer->logArray = logArray;
    overseer->check();
  }
  work = false;

  ready = true;
  changing = false;
  intStatus = OVERSEER_ARRAY_STATUS_NULL;

  while(isRunning) {
    shutdownMutex.lock();
    work = true;
    logArray->log("Overseer", "loop start");

    for(std::vector<std::shared_ptr<Overseer>>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
      std::shared_ptr<Overseer> overseer = *it;
      overseer->check();
    }

    logArray->log("Overseer", "loop end");
    work = false;
    shutdownMutex.unlock();

    // check overseers every next X useconds
    Helper::longSleep(cycleInterval);
  }
}

void OverseerArray::stop() {
  isRunning = false;
  changing = true;

  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("Overseer", "stop");

  changing = false;
  ready = false;
}

std::string OverseerArray::statusText() {
  if (intStatus == OVERSEER_ARRAY_STATUS_WAIT_FOR_MEAS) {
    return "Waiting for enough meas";
  }
  return "";
}
