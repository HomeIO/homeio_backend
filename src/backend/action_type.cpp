#include "action_type.hpp"

ActionType::ActionType() {
  executionCount = 0;
  failedCount = 0;
  maxTimeBufferSize = 100;
}

void ActionType::logInfo(std::string log) {
  Helper::logWithColor(log, YELLOW);
}

unsigned int ActionType::execute() {
  unsigned int raw = ioProxy->fetch(command, responseSize);

  if (raw == responseOkay) {
    executionCount++;
    markExecutionTime();
    Helper::logInfo("ActionType [" + name + "] executed");

    return 0;
  }
  else {
    failedCount++;
    Helper::logError("ActionType [" + name + "] FAILED, result=" + std::to_string(raw) + ",command=" + std::to_string(command) + ",responseSize=" + std::to_string(responseSize) );

    return 1;
  }

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

std::string ActionType::toJson() {
  std::string json;

  json = "{";
  json += "\"name\":\"" + name + "\",";

  if (wasExecuted()) {
    json += "\"lastExecutedAt\":" + std::to_string(lastExecutedAt()) + ",";
  }
  else {
    json += "\"lastExecutedAt\":null,";
  }

  json += "\"executionCount\":" + std::to_string(executionCount) + ",";
  json += "\"failedCount\":" + std::to_string(failedCount);

  json += "}";

  return json;
}

std::string ActionType::historyToJson() {
  std::string json, timeBufferString;

  for(std::vector<unsigned long long>::iterator it = timeBuffer.begin(); it != timeBuffer.end(); ++it) {
    timeBufferString += std::to_string(*it) + ",";
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

  timeBuffer.push_back( Helper::mTime() );
}
