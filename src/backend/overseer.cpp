Overseer::Overseer() {
  isMax = false;
  thresholdValue = 0.0;
}

bool Overseer::check() {
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

double Overseer::currentValue() {
  double value = meas->lastValueFor(windowSize);
  
  return value;
}

unsigned int Overseer::execute() {
  logInfo("OverseerArray [" + name + "] execute now");
  action->execute();
}