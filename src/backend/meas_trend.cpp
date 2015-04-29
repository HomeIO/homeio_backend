MeasTrend::MeasTrend() {
  valueFrom = 0;
  valueTo = 0;

  rawFrom = 0;
  rawTo = 0;
  
  type = MeasTrend::typeStable;
  
  timeFrom = 0;
  timeTo = 0;
  
  indexFrom = 0;
  indexTo = 0;
}

unsigned long long MeasTrend::timeDiff() {
  return timeTo - timeFrom;
}

double MeasTrend::valueDiff() {
  return valueTo - valueFrom;
}

double MeasTrend::trend() {
  if (timeDiff() > 0) {
    return valueDiff() / timeDiff();
  } 
  else {
    return 0.0;
  }
}