#include "overseer_array.hpp"

Overseer::Overseer() {
  isMax = false;
  thresholdValue = 0.0;
  minExecInterval = 1000;

  maxTimeBufferSize = 10;

  priority = 0;

}

bool Overseer::check() {
  if (checkLastExecutionTime() == false) {
    return false;
  }

  tempValue = currentValue();
  tempResult = false;

  if (isMax) {
    if (tempValue > thresholdValue) {
      tempResult = true;
    }
  } else {
    if (tempValue < thresholdValue) {
      tempResult = true;
    }
  }

  logArray->log("Overseer", "[" + name + "] value=" + std::to_string(tempValue) + ",windowSize=" + std::to_string(windowSize) + ",thresholdValue=" + std::to_string(thresholdValue) + "," + "result=" + std::to_string(tempResult));

  if (tempResult) {
    execute();
  }

  return tempResult;
}

std::string Overseer::toJson() {
  std::string json, timeBufferString;

  json = "{";
  json += "\"name\":\"" + name + "\",";
  json += "\"actionName\":\"" + actionName + "\",";
  json += "\"measName\":\"" + measName + "\",";

  json += "\"isMax\":" + std::to_string(isMax) + ",";
  json += "\"windowSize\":" + std::to_string(windowSize) + ",";
  json += "\"thresholdValue\":" + std::to_string(thresholdValue) + ",";

  json += "\"minExecInterval\":" + std::to_string(minExecInterval) + ",";
  json += "\"checkLastExecutionTime\":" + std::to_string(checkLastExecutionTime()) + ",";

  json += "\"currentValue\":" + std::to_string(currentValue()) + ",";
  json += "\"currentAvgValue\":" + std::to_string(meas->currentAvgValue());

  json += "}";

  return json;
}

std::string Overseer::timeBufferToJson() {
  std::string timeBufferString;

  for(std::vector<unsigned long long>::iterator it = timeBuffer.begin(); it != timeBuffer.end(); ++it) {
    timeBufferString += std::to_string(*it) + ",";
  }

  // remove last coma
  if (timeBufferString[timeBufferString.size() - 1] == ',') {
    timeBufferString.resize(timeBufferString.size() - 1);
  }

  return timeBufferString;
}

double Overseer::currentValue() {
  double value = meas->lastValueFor(windowSize);

  return value;
}

unsigned int Overseer::execute() {
  //logArray->log("Overseer", "[" + name + "] execute now");

  unsigned int result;
  result = action->execute();

  if (result == 0) {
    markExecutionTime();
    return 0;
  } else {
    return 1;
  }
}

void Overseer::markExecutionTime() {
  // mantain proper buffer size
  if (timeBuffer.size() >= maxTimeBufferSize) {
    timeBuffer.erase(timeBuffer.begin());
  }

  timeBuffer.push_back( currentTime() );
}

unsigned long long Overseer::currentTime() {
  return (unsigned long int) time(0);
}

unsigned long long Overseer::lastExecuteTime() {
  if (wasExecutedAtLeastOnce()) {
    return timeBuffer.back();
  } else {
    return currentTime();
  }
}

unsigned long long Overseer::secondsTillLastExec() {
  return currentTime() - lastExecuteTime();
}

bool Overseer::wasExecutedAtLeastOnce() {
  if (timeBuffer.size() > 0) {
    return true;
  } else {
    return false;
  }
}

bool Overseer::checkLastExecutionTime() {
  // empty list
  if ( timeBuffer.size() == 0 ) {
    return true;
  }

  if ( ( (unsigned long long) Helper::mTime() - lastExecuteTime() ) >= minExecInterval ) {
    return true;
  } else {
    return false;
  }
}
