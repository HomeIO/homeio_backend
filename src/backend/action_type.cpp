ActionType::ActionType() {
  executionCount = 0;
  failedCount = 0;
  maxTimeBufferSize = 100;
}

void ActionType::logInfo(string log) {
  logWithColor(log, YELLOW);
}

unsigned int ActionType::execute() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  
  if (raw == responseOkay) {
    executionCount++;
    markExecutionTime();
    logInfo("ActionType [" + name + "] executed");
  } 
  else {
    failedCount++;
    logError("ActionType [" + name + "] FAILED, result=" + to_string(raw) + ",command=" + to_string(command) + ",responseSize=" + to_string(responseSize) );
  }
  
  return raw;
}

unsigned long long ActionType::lastExecutedAt() {
  return timeBuffer.back();
}

bool ActionType::wasExecuted() {
  if (timeBuffer.size() > 0) {
    return true;
  }
  else {
    return false;
  }
}

string ActionType::toJson() {
  string json;
  
  json = "{";
  json += "\"name\":\"" + name + "\",";
  
  if (wasExecuted()) {
    json += "\"lastExecutedAt()\":" + to_string(lastExecutedAt()) + ",";
  }
  else {
    json += "\"lastExecutedAt()\":null,";
  }
  
  json += "\"executionCount\":" + to_string(executionCount) + ",";
  json += "\"failedCount\":" + to_string(failedCount);
  
  json += "}";
  
  return json;
}

string ActionType::historyToJson() {
  string json, timeBufferString;
  
  for(std::vector<unsigned long long>::iterator it = timeBuffer.begin(); it != timeBuffer.end(); ++it) {
    timeBufferString += to_string(*it) + ",";
  }
  
  // remove last coma
  if (timeBufferString[timeBufferString.size() - 1] == ',') {
    timeBufferString.resize(timeBufferString.size() - 1);
  }
  
  json = "{";
  json += "\"name\":\"" + name + "\",";
  json += "\"history\":[" + timeBufferString + "]";
  json += "}";
  
  return json;
}
  
  
void ActionType::markExecutionTime() {
  // mantain proper buffer size
  if (timeBuffer.size() >= maxTimeBufferSize) {
    timeBuffer.erase(timeBuffer.begin());
  }
  
  timeBuffer.push_back( mTime() );
}