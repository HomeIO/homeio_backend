ActionType::ActionType() {
  executionCount = 0;
}

unsigned int ActionType::execute() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  
  if (raw == responseOkay) {
    executionCount++;
    cout << "ActionType [" << name << "] executed" << endl;
  } 
  else {
    cout << "ActionType [" << name << "] FAILED" << endl;
  }
  
  return raw;
}
