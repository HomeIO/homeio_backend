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
  rawToValue(lastRaw());
}

// last avg value
double MeasType::lastValueFor(unsigned int i) {
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
  json += "\"priority\":\"" + to_string(priority) + "\",";
  json += "\"buffer\":" + buffer->toJson() + ",";
  json += "\"coefficientLinear\":" + to_string(coefficientLinear) + ",";
  json += "\"coefficientOffset\":" + to_string(coefficientOffset) + ",";
  json += "\"value\":" + to_string(lastValue()) + ",";
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
  return buffer->jsonArray(timeToIndex(timeFrom), timeToIndex(timeTo));
}

string MeasType::rawForIndexJson(unsigned long int indexFrom, unsigned long long indexTo) {
  return buffer->jsonArray(indexFrom, indexTo);
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