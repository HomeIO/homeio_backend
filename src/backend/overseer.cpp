Overseer::Overseer() {
  isMax = false;
  thresholdValue = 0.0;
}

bool Overseer::check() {
  cout << currentTime() << " OverseerArray [" << name << "] check" << endl;
  
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
  
  cout << currentTime() << " OverseerArray [" << name << "] value=" << value << endl;
  cout << currentTime() << " OverseerArray [" << name << "] windowSize=" << windowSize << endl;
  cout << currentTime() << " OverseerArray [" << name << "] thresholdValue=" << thresholdValue << endl;
  cout << currentTime() << " OverseerArray [" << name << "] result=" << result << endl;
  
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
  cout << currentTime() << " OverseerArray [" << name << "] execute" << endl;
  action->execute();
}