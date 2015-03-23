MeasType::MeasType() {
  buffer = new MeasBuffer;
  
  coefficientLinear = 1.0;
  coefficientOffset = 0;
  
  started = false;
}

unsigned int MeasType::fetch() {
  prepareFetch();
  
  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);
  
  logInfo(logPrefix + "raw,value =  " + strColor(RESET, BLUE, BLACK) + to_string(raw) + strColor(RESET, GREEN, BLACK) + " , " + strColor(RESET, BLUE, BLACK) + to_string(rawToValue(raw)) + strColor(RESET, GREEN, BLACK) );
  
  return raw;
}

void MeasType::prepareFetch() {
  if (started) {
    return;
  }

  logPrefix = "MeasBuffer [" + name + "] ";
  logPrefix.append(30 - logPrefix.length(), ' ');
  started = true;
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
  
