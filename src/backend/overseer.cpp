#include "overseer_array.hpp"

Overseer::Overseer() {
  isMax = false;
  thresholdValue = 0.0;
  minExecInterval = 1000;

  maxTimeBufferSize = 10;

  priority = 0;

}

void Overseer::logInfo(string log) {
  logWithColor(log, MAGENTA);
}

bool Overseer::check() {
  if (checkLastExecutionTime() == false) {
    return false;
  }

  logInfo("OverseerArray [" + name + "] check");

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

  logInfo("OverseerArray [" + name + "] value=" + to_string(value));
  logInfo("OverseerArray [" + name + "] windowSize=" + to_string(windowSize));
  logInfo("OverseerArray [" + name + "] thresholdValue=" + to_string(thresholdValue));
  logInfo("OverseerArray [" + name + "] result=" + to_string(result));

  if (result) {
    execute();
  }

  return result;
}

string Overseer::toJson() {
  string json, timeBufferString;

  json = "{";
  json += "\"name\":\"" + name + "\",";
  json += "\"actionName\":\"" + actionName + "\",";
  json += "\"measName\":\"" + measName + "\",";

  json += "\"isMax\":" + to_string(isMax) + ",";
  json += "\"windowSize\":" + to_string(windowSize) + ",";
  json += "\"thresholdValue\":" + to_string(thresholdValue) + ",";

  json += "\"minExecInterval\":" + to_string(minExecInterval) + ",";
  json += "\"checkLastExecutionTime\":" + to_string(checkLastExecutionTime()) + ",";

  json += "\"currentValue\":" + to_string(currentValue()) + ",";
  json += "\"currentAvgValue\":" + to_string(meas->currentAvgValue());

  json += "}";

  return json;
}

string Overseer::timeBufferToJson() {
  string timeBufferString;

  for(std::vector<unsigned long long>::iterator it = timeBuffer.begin(); it != timeBuffer.end(); ++it) {
    timeBufferString += to_string(*it) + ",";
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
  logInfo("OverseerArray [" + name + "] execute now");

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

  if ( ( (unsigned long long) mTime() - lastExecuteTime() ) >= minExecInterval ) {
    return true;
  } else {
    return false;
  }
}
