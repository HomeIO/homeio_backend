#include "meas_type.hpp"

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
  index = 0;

  avgValue = 0.0;
  intervalAvg = 3600000; // recalculate avg every 1 hour
  windowAvg = 50000; // use this amount of measurements to calc avg

  extRemoveSpikes = false;
  extBackendRemoveSpikes = false;

  lastStored = Helper::mTime();
}

void MeasType::resizeBuffer(unsigned long int _maxSize = 1000000) {
  buffer->clearAndResize(_maxSize);
}

unsigned int MeasType::fetch() {
  prepareFetch();

  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);

  return raw;
}

void MeasType::prepareFetch() {
  if (started) {
    return;
  }

  // mark buffer to fix spikes
  buffer->removeSpikes = extBackendRemoveSpikes;

  // deprecated
  //logPrefix = "MeasBuffer [" + name + "] ";
  //logPrefix.append(35 - logPrefix.length(), ' ');
  started = true;
}

// deprecated
// used in old meas fetched log output
std::string MeasType::fetchString(unsigned int raw) {
  std::string tmpString = "";
  std::string partialString = "";

  tmpString += " raw ";
  tmpString += Helper::strColor(RESET, BLUE, BLACK);

  partialString = std::to_string(raw);
  partialString.insert(0, 10 - partialString.length(), ' ');
  tmpString += partialString;

  tmpString += Helper::strColor(RESET, GREEN, BLACK);
  tmpString += " , ";
  tmpString += Helper::strColor(RESET, BLUE, BLACK);

  std::ostringstream os;
  os << std::setprecision(5) << rawToValue(raw);
  partialString = os.str();
  partialString.insert(0, 12 - partialString.length(), ' ');
  tmpString += partialString;

  tmpString += Helper::strColor(RESET, GREEN, BLACK);

  return "MeasBuffer [" + name + "] " + tmpString;
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
  if ( (Helper::mTime() - lastAvgTime) > intervalAvg) {
    logArray->log("Meas", "[" + name + "] recalculateAvg() start");

    avgValue = lastValueFor(windowAvg);
    lastAvgTime = Helper::mTime();

    logArray->log("Meas", "[" + name + "] recalculateAvg() finish - " + std::to_string(avgValue));
  }
}

double MeasType::rawToValue(unsigned int _raw) {
  return (double)((int) _raw + coefficientOffset) * coefficientLinear;
}

double MeasType::valueAt(unsigned long int i) {
  return rawToValue(buffer->at(i));
}

std::string MeasType::toJson() {
  std::string json;

  json = "{";
  json += "\"name\":\"" + name + "\",";
  json += "\"unit\":\"" + unit + "\",";
  json += "\"priority\":\"" + std::to_string(priority) + "\",";
  json += "\"buffer\":" + buffer->toJson() + ",";
  json += "\"coefficientLinear\":" + std::to_string(coefficientLinear) + ",";
  json += "\"coefficientOffset\":" + std::to_string(coefficientOffset) + ",";
  json += "\"value\":" + std::to_string(lastValue()) + ",";
  json += "\"currentAvgValue\":" + std::to_string(currentAvgValue()) + ",";
  json += "\"raw\":" + std::to_string(lastRaw()) + ",";


  // extensions
  json += "\"options\": {";
  json += "\"removeSpikes\": " + std::to_string(extRemoveSpikes);
  json += "}";

  json += "}";

  return json;
}

unsigned long int MeasType::timeToIndex(unsigned long long t) {
  if (t >= buffer->lastTime) {
    return 0; // out of range
  }

  unsigned long long tempTime = (buffer->lastTime - t);
  unsigned long int tempIndex = (unsigned long int) (tempTime / (unsigned long long) buffer->calcInterval() );

  return tempIndex;
}

std::string MeasType::rawForTimeJson(unsigned long long timeFrom, unsigned long long timeTo) {
  return buffer->jsonArray(timeToIndex(timeFrom), timeToIndex(timeTo), 0);
}

std::string MeasType::rawHistoryForTimeJson(unsigned long long timeFrom, unsigned long long timeTo, unsigned long int responseMaxSize) {
  return buffer->jsonArray(timeToIndex(timeFrom), timeToIndex(timeTo), responseMaxSize);
}

std::string MeasType::rawForIndexJson(unsigned long int indexFrom, unsigned long long indexTo) {
  return buffer->jsonArray(indexFrom, indexTo, 0);
}

std::vector < StorageHash > MeasType::prepareStorage(unsigned long long timeFrom, unsigned long long timeTo) {
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

std::vector < StorageHash > MeasType::storageArray(unsigned long long timeFrom, unsigned long long timeTo) {
  Helper::storageMutex.lock();
  std::vector < StorageHash > tmp = prepareStorage(timeFrom, timeTo);
  Helper::storageMutex.unlock();

  return tmp;
}

std::string MeasType::storageJson(unsigned long long timeFrom, unsigned long long timeTo) {
  Helper::storageMutex.lock();
  std::string tmp;
  prepareStorage(timeFrom, timeTo);
  tmp = measTypeStorage->storageFullJson();
  Helper::storageMutex.unlock();

  return tmp;
}

std::vector < MeasTrend > MeasType::getTrendsBetween(unsigned long long timeFrom, unsigned long long timeTo) {
  std::vector < MeasTrend > result;

  std::vector < unsigned int > rawBuffer = buffer->getFromBuffer(timeToIndex(timeFrom), timeToIndex(timeTo), 0);

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
    } else if (tmpDeviation > maxDeviation) {
      // ascending
      if (tempTrend->tmpType != MeasTrend::typeAscend) {
        // was stable/desc, now is asc
        createNew = true;
        newType = MeasTrend::typeAscend;
      }
    } else {
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
      //tempTrend->timeTo = timeFrom + tempInterval * ( (unsigned long long) it - (unsigned long long) rawBuffer.begin() );
      tempTrend->timeTo = timeFrom + tempInterval * ( *it - *rawBuffer.begin() );

      if (tempTrend->timeDiff() > 0) {
        result.push_back(*tempTrend);
      }

      tempTrend = new MeasTrend;
      tempTrend->tmpType = newType; // default equal
      tempTrend->rawFrom = *it;
      tempTrend->valueFrom = rawToValue(tempTrend->rawFrom);
      tempTrend->timeFrom = timeFrom + tempInterval * (*it - *rawBuffer.begin() );

      createNew = false;
    }

    tempTrend->rawTo = *it;
    tempTrend->timeTo = timeFrom + tempInterval * (*it - *rawBuffer.begin() );

    // set to current
  }

  // add last element
  result.push_back(*tempTrend);

  return result;
}

std::string MeasType::statsJson(unsigned long long timeFrom, unsigned long long timeTo) {
  std::vector < MeasTrend > result = getTrendsBetween(timeFrom, timeTo);

  std::string trendString = "[";

  for(std::vector<MeasTrend>::iterator it = result.begin(); it != result.end(); ++it) {
    trendString += "{";
    trendString += "\"type\":" + std::to_string(it->type()) + ",";
    trendString += "\"timeDiff\":" + std::to_string(it->timeDiff() ) + ",";
    trendString += "\"valueDiff\":" + std::to_string(it->valueDiff() ) + ",";
    trendString += "\"trend\":" + std::to_string(it->trend() ) + ",";

    trendString += "\"rawFrom\":" + std::to_string(it->rawFrom) + ",";
    trendString += "\"rawTo\":" + std::to_string(it->rawTo) + ",";
    trendString += "\"valueFrom\":" + std::to_string(it->valueFrom) + ",";
    trendString += "\"valueTo\":" + std::to_string(it->valueTo) + ",";
    trendString += "\"timeFrom\":" + std::to_string(it->timeFrom) + ",";
    trendString += "\"timeTo\":" + std::to_string(it->timeTo);

    trendString += "},";
  }

  // remove last coma
  if (trendString[trendString.size() - 1] == ',') {
    trendString.resize(trendString.size() - 1);
  }

  trendString += "]";

  std::string responseString = "{";
  responseString += "\"trends\":" + trendString + "}";

  return responseString;
}
