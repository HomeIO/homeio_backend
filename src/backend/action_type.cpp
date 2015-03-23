ActionType::ActionType() {
  executionCount = 0;
}

void ActionType::logInfo(string log) {
  logWithColor(log, YELLOW);
}

unsigned int ActionType::execute() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  
  if (raw == responseOkay) {
    executionCount++;
    logInfo("ActionType [" + name + "] executed");
  } 
  else {
    failedCount++;
    logError("ActionType [" + name + "] FAILED, result=" + to_string(raw) + ",command=" + to_string(command) + ",responseSize=" + to_string(responseSize) );
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
  
