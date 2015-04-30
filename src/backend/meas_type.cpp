MeasType::MeasType() {
  buffer = new MeasBuffer;
  
  measTypeStorage = new MeasTypeStorage;
  // default storage parameters
  minTimeDiffToStore = 1000;
  maxTimeDiffToStore = 3600000;
  valueDiffToStore = 1.0;
  
  coefficientLinear = 1.0;
  coefficientOffset = 0;
  
  started = false;
  priority = 0;
  
  avgValue = 0.0;
  intervalAvg = 3600000; // recalculate avg every 1 hour
  windowAvg = 50000; // use this amount of measurements to calc avg

}

unsigned int MeasType::fetch() {
  prepareFetch();
  
  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);
  logInfo(fetchString(raw));
  
  return raw;
}

void MeasType::prepareFetch() {
  if (started) {
    return;
  }

  logPrefix = "MeasBuffer [" + name + "] ";
  logPrefix.append(35 - logPrefix.length(), ' ');
  started = true;
}

string MeasType::fetchString(unsigned int raw) {
  string tmpString = "";
  string partialString = "";
  
  tmpString += " raw ";
  tmpString += strColor(RESET, BLUE, BLACK);
  
  partialString = to_string(raw);
  partialString.insert(0, 10 - partialString.length(), ' ');
  tmpString += partialString;
  
  tmpString += strColor(RESET, GREEN, BLACK);
  tmpString += " , ";
  tmpString += strColor(RESET, BLUE, BLACK);

  ostringstream os;
  os << setprecision(5) << rawToValue(raw);
  partialString = os.str();
  partialString.insert(0, 12 - partialString.length(), ' ');
  tmpString += partialString;
  
  tmpString += strColor(RESET, GREEN, BLACK);
 
  return logPrefix + tmpString;
}

unsigned int MeasType::addRaw(unsigned int _raw) {
  buffer->add(_raw);
  return 0;
}

unsigned int MeasType::lastRaw() {
  return buffer->at(0);
}

double MeasType::lastValue() {
  return rawToValue(lastRaw());
}

// last avg value
double MeasType::lastValueFor(unsigned long int i) {
  long int tmpRaw = 0;
  unsigned int j = 0;
  unsigned int count = 0;
  
  for (j = 0; j < i; j++) {
    if (buffer->stored(j)) {
      tmpRaw += (long int) buffer->at(j);
      tmpRaw += coefficientOffset;
      count++;
    }
  }
  
  if (count == 0) {
    // warning: no data
    return 0.0;
  }
  
  return ((double) tmpRaw / (double) count) * coefficientLinear;
}

double MeasType::currentAvgValue() {
  recalculateAvg();
  return avgValue;
}

void MeasType::recalculateAvg() {
  if ( (mTime() - lastAvgTime) > intervalAvg) {
    logInfo("recalculateAvg() start");

    avgValue = lastValueFor(windowAvg);
    lastAvgTime = mTime();

    logInfo("recalculateAvg() finish");
  }
}

double MeasType::rawToValue(unsigned int _raw) {
  return (double)((int) _raw + coefficientOffset) * coefficientLinear;
}

double MeasType::valueAt(unsigned long int i) {
  rawToValue(buffer->at(i));
}

string MeasType::toJson() {
  string json;
  
  json = "{";
  json += "\"name\":\"" + name + "\",";
  json += "\"unit\":\"" + unit + "\",";
  json += "\"priority\":\"" + to_string(priority) + "\",";
  json += "\"buffer\":" + buffer->toJson() + ",";
  json += "\"coefficientLinear\":" + to_string(coefficientLinear) + ",";
  json += "\"coefficientOffset\":" + to_string(coefficientOffset) + ",";
  json += "\"value\":" + to_string(lastValue()) + ",";
  json += "\"currentAvgValue\":" + to_string(currentAvgValue()) + ",";
  json += "\"raw\":" + to_string(lastRaw());
  json += "}";
  
  return json;
}
  
void MeasType::logInfo(string log) {
  logWithColor(log, GREEN);
}

unsigned long int MeasType::timeToIndex(unsigned long long t) {
  if (t >= buffer->lastTime) {
    return 0; // out of range
  }
  
  unsigned long long tempTime = (buffer->lastTime - t);
  unsigned long int tempIndex = (unsigned long int) (tempTime / (unsigned long long) buffer->calcInterval() );
  
  return tempIndex;  
}

string MeasType::rawForTimeJson(unsigned long long timeFrom, unsigned long long timeTo) {
  return buffer->jsonArray(timeToIndex(timeFrom), timeToIndex(timeTo), 0);
}

string MeasType::rawHistoryForTimeJson(unsigned long long timeFrom, unsigned long long timeTo, unsigned long int responseMaxSize) {
  return buffer->jsonArray(timeToIndex(timeFrom), timeToIndex(timeTo), responseMaxSize);
}

string MeasType::rawForIndexJson(unsigned long int indexFrom, unsigned long long indexTo) {
  return buffer->jsonArray(indexFrom, indexTo, 0);
}

vector < StorageHash > MeasType::prepareStorage(unsigned long long timeFrom, unsigned long long timeTo) {
  measTypeStorage->minTimeDiffToStore = minTimeDiffToStore;
  measTypeStorage->maxTimeDiffToStore = maxTimeDiffToStore;
  measTypeStorage->valueDiffToStore = valueDiffToStore;
  measTypeStorage->timeTo = timeTo;
  measTypeStorage->timeFrom = timeFrom;
  measTypeStorage->interval = buffer->calcInterval();
  
  // less important, used for details in json
  measTypeStorage->indexFrom = timeToIndex(timeFrom);
  measTypeStorage->indexTo = timeToIndex(timeTo);
  measTypeStorage->count = buffer->count;
  measTypeStorage->lastTime = buffer->lastTime;
  measTypeStorage->firstTime = buffer->firstTime;
  
  double tmpValue;
  measTypeStorage->clearBuffer();
  for (unsigned long int i = measTypeStorage->indexFrom; i > measTypeStorage->indexTo; i--) {
    tmpValue = valueAt(i);
    measTypeStorage->buffer.push_back(tmpValue);
  }
  return measTypeStorage->prepareStorageBuffer();
}

vector < StorageHash > MeasType::storageArray(unsigned long long timeFrom, unsigned long long timeTo) {
  storageMutex.lock();
  vector < StorageHash > tmp = prepareStorage(timeFrom, timeTo);
  storageMutex.unlock();
  
  return tmp;
}

string MeasType::storageJson(unsigned long long timeFrom, unsigned long long timeTo) {
  storageMutex.lock();
  string tmp;
  prepareStorage(timeFrom, timeTo);
  tmp = measTypeStorage->storageFullJson();
  storageMutex.unlock();
  
  return tmp;
}

vector < MeasTrend > MeasType::getTrendsBetween(unsigned long long timeFrom, unsigned long long timeTo) {
  vector < MeasTrend > result;
  
  vector < unsigned int > rawBuffer = buffer->getFromBuffer(timeToIndex(timeFrom), timeToIndex(timeTo), 0);
  
  // no meas
  if (rawBuffer.size() == 0) {
    return result;
  }
  
  unsigned long long tempInterval = buffer->calcInterval();
  long int tmpDeviation = 0;
  long int maxDeviation = 4;
  bool createNew = false;
  unsigned char newType = MeasTrend::typeStable;
  
  MeasTrend *tempTrend = new MeasTrend;
  tempTrend->tmpType = 0; // default equal
  tempTrend->rawFrom = rawBuffer[0];
  tempTrend->rawTo = rawBuffer[0];
  tempTrend->timeFrom = timeFrom;
  tempTrend->timeTo = timeFrom;
  
  for(std::vector<unsigned int>::iterator it = rawBuffer.begin(); it != rawBuffer.end(); ++it) {
    tmpDeviation = (long int) *it - (long int) tempTrend->rawFrom;
    if (labs(tmpDeviation) < maxDeviation) {
      // stable
      if (tempTrend->tmpType != MeasTrend::typeStable) {
        // was asc/desc, now is stable
        createNew = true;
        newType = MeasTrend::typeStable;
      }
    }
    else if (tmpDeviation > maxDeviation) {
      // ascending
      if (tempTrend->tmpType != MeasTrend::typeAscend) {
        // was stable/desc, now is asc
        createNew = true;
        newType = MeasTrend::typeAscend;
      }
    }
    else {
      // descening
      if (tempTrend->tmpType != MeasTrend::typeDescend) {
        // was stable/asc, now is asc
        createNew = true;
        newType = MeasTrend::typeDescend;
      }
    }
    
    if (createNew) {
      tempTrend->rawTo = *it;
      tempTrend->valueTo = rawToValue(tempTrend->rawTo);
      tempTrend->timeTo = timeFrom + tempInterval * (it - rawBuffer.begin() );
      
      if (tempTrend->timeDiff() > 0) {
        result.push_back(*tempTrend);
      }

      tempTrend = new MeasTrend;
      tempTrend->tmpType = newType; // default equal
      tempTrend->rawFrom = *it;
      tempTrend->valueFrom = rawToValue(tempTrend->valueFrom);
      tempTrend->timeFrom = timeFrom + tempInterval * (it - rawBuffer.begin() );
      
      createNew = false;
    }
    
    tempTrend->rawTo = *it;
    tempTrend->timeTo = timeFrom + tempInterval * (it - rawBuffer.begin() );
    
    // set to current
  }
  
  // add last element
  result.push_back(*tempTrend);
  
  return result;
}

string MeasType::statsJson(unsigned long long timeFrom, unsigned long long timeTo) {
  vector < MeasTrend > result = getTrendsBetween(timeFrom, timeTo);
  
  string trendString = "[";
  
  for(vector<MeasTrend>::iterator it = result.begin(); it != result.end(); ++it) {
    trendString += "{";
    trendString += "\"type\":" + to_string(it->type()) + ",";
    trendString += "\"timeDiff\":" + to_string(it->timeDiff() ) + ",";
    trendString += "\"valueDiff\":" + to_string(it->valueDiff() ) + ",";
    trendString += "\"trend\":" + to_string(it->trend() ) + ",";
    
    trendString += "\"rawFrom\":" + to_string(it->rawFrom) + ",";
    trendString += "\"rawTo\":" + to_string(it->rawTo) + ",";
    trendString += "\"valueFrom\":" + to_string(it->valueFrom) + ",";
    trendString += "\"valueTo\":" + to_string(it->valueTo) + ",";
    trendString += "\"timeFrom\":" + to_string(it->timeFrom) + ",";
    trendString += "\"timeTo\":" + to_string(it->timeTo);
    trendString += "},";
  }

  // remove last coma
  if (trendString[trendString.size() - 1] == ',') {
    trendString.resize(trendString.size() - 1);
  }
  
  trendString += "]";
  
  string responseString = "{";
  responseString += "\"trends\":" + trendString + "}";
  
  return responseString;
}