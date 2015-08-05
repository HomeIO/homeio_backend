MeasTrend::MeasTrend() {
  valueFrom = 0;
  valueTo = 0;

  rawFrom = 0;
  rawTo = 0;
  
  tmpType = MeasTrend::typeStable;
  
  timeFrom = 0;
  timeTo = 0;
  
  indexFrom = 0;
  indexTo = 0;
}

unsigned char MeasTrend::type() {
  if (valueFrom > valueTo) {
    return typeDescend;
  }
  else if (valueFrom < valueTo) {
    return typeAscend;
  }
  else {
    return typeStable;
  }
}

unsigned long long MeasTrend::timeDiff() {
  return timeTo - timeFrom;
}

double MeasTrend::valueDiff() {
  return valueTo - valueFrom;
}

double MeasTrend::trend() {
  if (timeDiff() > 0) {
    return (double) valueDiff() / (double) timeDiff();
  } 
  else {
    return 0.0;
  }
}