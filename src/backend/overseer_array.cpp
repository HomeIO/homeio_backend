OverseerArray::OverseerArray() {
  std::vector <Overseer> overseers;
  cycleInterval = 1000000;
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
  // wait 2s to warm up
  usleep(200000);
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
    usleep(cycleInterval);
  }
  
  return 0;
}