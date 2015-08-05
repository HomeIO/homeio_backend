OverseerArray::OverseerArray() {
  std::vector <Overseer> overseers;
  cycleInterval = 1000000;
  usDelay = 200000; // wait 2s to warm up
}

void OverseerArray::logInfo(string log) {
  logWithColor(log, MAGENTA);
}

unsigned int OverseerArray::add(Overseer *o) {
  overseers.push_back(*o);
  logInfo("Overseer added: '" + o->name + "' (" + to_string(overseers.size()) + " total overseers)");
  return 0;
}

Overseer *OverseerArray::byName(string s) {
  for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}

int OverseerArray::start() {
  longSleep(usDelay);
  // wait for enough measurements
  measTypeArray->delayTillReady();
  
  
  logInfo("OverseerArray start");
  
  for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    it->meas = measTypeArray->byName(it->measName);
    it->action = actionTypeArray->byName(it->actionName);
    it->check();
  }
  
  while(true) {
    logInfo("OverseerArray loop start");
    for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
      it->check();
    }
    logInfo("OverseerArray loop end");
    
    // check overseers every next X useconds
    longSleep(cycleInterval);
  }
  
  return 0;
}