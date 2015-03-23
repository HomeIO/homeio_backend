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
    failedCount++;
    
    txtColor(RESET, RED, BLACK);
    cout << currentTime() << " ActionType [" << name << "] FAILED, result=" << raw << ",command=" << command << ",responseSize=" << responseSize << endl;
    resetColor();
  }
  
  return raw;
}

string ActionType::toJson() {
  string json;
  
  json = "{";
  json += "\"name\":\"" + name + "\",";
  json += "\"executionCount\":" + to_string(executionCount) + ",";
  json += "\"failedCount\":" + to_string(failedCount);

  json += "}";
  
  return json;
}
  