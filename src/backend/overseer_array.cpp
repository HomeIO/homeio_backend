OverseerArray::OverseerArray() {
  std::vector <Overseer> overseers;
}

unsigned int OverseerArray::add(Overseer *o) {
  overseers.push_back(*o);
  cout << "Overseer added: '" << o->name << "' (" << overseers.size() << " total overseers)" << endl;
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
  cout << "OverseerArray start" << endl;
  
  for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
    it->meas = measTypeArray->byName(it->measName);
    it->action = actionTypeArray->byName(it->actionName);
    it->check();
  }
  
  while(true) {
    cout << "OverseerArray loop start" << endl;
    for(std::vector<Overseer>::iterator it = overseers.begin(); it != overseers.end(); ++it) {
      it->check();
    }
    cout << "OverseerArray loop end" << endl;
    
    // check overseers every next 1s
    usleep(100000);
  }
  
  return 0;
}