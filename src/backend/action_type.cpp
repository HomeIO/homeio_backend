ActionType::ActionType() {
  executionCount = 0;
}

unsigned int ActionType::execute() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  
  if (raw == responseOkay) {
    executionCount++;
    cout << currentTime() << " ActionType [" << name << "] executed" << endl;
  } 
  else {
    cout << currentTime() << " ActionType [" << name << "] FAILED, result=" << raw << ",command=" << command << ",responseSize=" << responseSize << endl;
  }
  
  return raw;
}
