#include "overseer_array.hpp"

Overseer::Overseer() {
  isMax = false;
  thresholdValue = 0.0;
  minExecInterval = 1000;

  maxTimeBufferSize = 10;

  priority = 0;

}

void Overseer::logInfo(std::string log) {
  Helper::logWithColor(log, MAGENTA);
}

bool Overseer::check() {
  if (checkLastExecutionTime() == false) {
    return false;
  }

  Helper::logInfo("OverseerArray [" + name + "] check");

  double value = currentValue();
  bool result = false;

   if (isMax) {
    if (currentValue() > thresholdValue) {
      result = true;
    }
  }
  else {
    if (currentValue() < thresholdValue) {
      result = true;
    }
  }

  Helper::logInfo("OverseerArray [" + name + "] value=" + std::to_string(value));
  Helper::logInfo("OverseerArray [" + name + "] windowSize=" + std::to_string(windowSize));
  Helper::logInfo("OverseerArray [" + name + "] thresholdValue=" + std::to_string(thresholdValue));
  Helper::logInfo("OverseerArray [" + name + "] result=" + std::to_string(result));

  if (result) {
    execute();
  }

  return result;
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
  Helper::logInfo("OverseerArray [" + name + "] execute now");

  unsigned int result;
  result = action->execute();

  if (result == 0) {
    markExecutionTime();
    return 0;
  }
  else {
    return 1;
  }
}

void Overseer::markExecutionTime() {
  // mantain proper buffer size
  if (timeBuffer.size() >= maxTimeBufferSize) {
    timeBuffer.erase(timeBuffer.begin());
  }

  timeBuffer.push_back( (unsigned long int) time(0) );
}

unsigned long long Overseer::lastExecuteTime() {
  return timeBuffer.back();
}

bool Overseer::checkLastExecutionTime()
{
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
