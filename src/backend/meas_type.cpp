MeasType::MeasType() {
  buffer = new MeasBuffer;
  
  measTypeStorage = new MeasTypeStorage;
  
  coefficientLinear = 1.0;
  coefficientOffset = 0;
  
  started = false;
}

string MeasType::storageBuffer(unsigned long long timeFrom, unsigned long long timeTo) {
  return storageJson(timeFrom, timeTo);
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
  json += "\"buffer\":{\"count\":" + to_string(buffer->count) + "},";
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
  
  cout << "tempTime " << tempTime << endl;
  cout << "tempIndex " << tempIndex << endl;
  cout << "t " << t << endl;
  cout << "buffer->lastTime " << buffer->lastTime << endl;
  
  
  return tempIndex;  
}

string MeasType::storageJson(unsigned long long timeFrom, unsigned long long timeTo) {
  unsigned long int indexFrom = timeToIndex(timeFrom);
  unsigned long int indexTo = timeToIndex(timeTo);
  
  string detailsString, valueArrayString, storageArray, response;
  double tmpValue;
  
  // just debug data
  detailsString = "{";
  detailsString += "\"indexFrom\":" + to_string(indexFrom) + ",";
  detailsString += "\"indexTo\":" + to_string(indexTo) + ",";
  detailsString += "\"timeFrom\":" + to_string(timeFrom) + ",";
  detailsString += "\"timeTo\":" + to_string(timeTo) + ",";
  detailsString += "\"interval\":" + to_string(buffer->calcInterval()) + ",";
  detailsString += "\"count\":" + to_string(buffer->count) + ",";
  detailsString += "\"lastTime\":" + to_string(buffer->lastTime) + ",";
  detailsString += "\"firstTime\":" + to_string(buffer->firstTime);
  detailsString += "}";

  cout << indexFrom << endl;
  cout << indexTo << endl;
  
  // values array
  valueArrayString = "[";
  // something weird if "i >=", so better stay "i >"
  for (unsigned long int i = indexFrom; i > indexTo; i--) {
    cout << i << endl;
    tmpValue = valueAt(i);
    valueArrayString += to_string(tmpValue) + ","; 
    // will be used in storage
    measTypeStorage->buffer.push_back(tmpValue);
  }
  // remove last coma
  if (valueArrayString[valueArrayString.size() - 1] == ',') {
    valueArrayString.resize(valueArrayString.size() - 1);
  }
  valueArrayString += "]"; 
  
  // processed to storage format
  // TODO copy storage parameters, value array, time ranges to storage class
  
  measTypeStorage->timeTo = timeTo;
  measTypeStorage->timeFrom = timeFrom;
  measTypeStorage->interval = buffer->calcInterval();
  storageArray = measTypeStorage->storageJson();
  
  response = "{";
  response += "\"details\":" + detailsString + ",";
  response += "\"valueArray\":" + valueArrayString + ",";  
  response += "\"storageArray\":" + storageArray;  
  response += "}";
  
  return response;
}