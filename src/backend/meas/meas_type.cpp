#include "meas_type.hpp"

MeasType::MeasType() {
  buffer = std::make_shared<MeasBuffer>();
  measTypeStorage = std::make_shared<MeasTypeStorage>();

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
  // copy everything which could not be done in constructor

  // mark buffer to fix spikes
  buffer->removeSpikes = extBackendRemoveSpikes;

  started = true;
}

void MeasType::enableRangeFilter(double filterMin, double filterMax, double defaultValue) {
  buffer->filterSpike = true;
  buffer->defaultRaw = valueToRaw(defaultValue);

  if (coefficientLinear > 0.0) {
    buffer->filterMin = valueToRaw(filterMin);
    buffer->filterMax = valueToRaw(filterMax);
  } else {
    // some measurements (like soil moisture) use negative coefficient
    // so we must reverse ranges
    buffer->filterMin = valueToRaw(filterMax);
    buffer->filterMax = valueToRaw(filterMin);
  }

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

double MeasType::rawToValue(meas_buffer_element _raw) {
  return (double)((int) _raw + coefficientOffset) * coefficientLinear;
}

meas_buffer_element MeasType::valueToRaw(double v) {
  double tmp = v;
  tmp = tmp / coefficientLinear;
  tmp = tmp - (double) coefficientOffset;
  return (meas_buffer_element) tmp;
}


double MeasType::valueAt(meas_buffer_index i) {
  return rawToValue(buffer->at(i));
}

double MeasType::avgValueAt(meas_buffer_index j, meas_buffer_index window) {
  int c = 0;
  double s = 0;

  meas_buffer_index i, iFrom, iTo;

  if (j >= window) {
    iFrom = j - window;
  } else {
    iFrom = 0;
  }
  iTo = j + window;

  for (i = iFrom; i < iTo; i++) {
    if (buffer->stored(i)) {
      c++;
      s += valueAt(i);
    }
  }

  if (c > 0) {
    return s / (double) (c);
  } else {
    return 0.0;
  }

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
  unsigned long long tempInterval = buffer->calcInterval();
  if (0 == tempInterval) {
    return 0;
  }

  unsigned long long tempTime = (buffer->lastTime - t);
  unsigned long int tempIndex = (unsigned long int) (tempTime / tempInterval );

  return tempIndex;
}

meas_time MeasType::indexToTime(meas_buffer_index i) {
  return buffer->lastTime - (((meas_time) i) * buffer->calcInterval());
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

std::vector < std::shared_ptr<StorageHash> > MeasType::prepareStorage(unsigned long long timeFrom, unsigned long long timeTo) {
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

std::vector < std::shared_ptr<StorageHash> > MeasType::storageArray(unsigned long long timeFrom, unsigned long long timeTo) {
  Helper::storageMutex.lock();
  std::vector < std::shared_ptr<StorageHash> > tmp = prepareStorage(timeFrom, timeTo);
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

std::vector < std::shared_ptr<MeasTrend> > MeasType::getTrendsBetween(unsigned long long timeFrom, unsigned long long timeTo) {
  std::vector < std::shared_ptr<MeasTrend> > result;

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

  std::shared_ptr<MeasTrend> tempTrend = std::make_shared<MeasTrend>();
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
        result.push_back(tempTrend);
      }

      tempTrend = std::make_shared<MeasTrend>();
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
  result.push_back(tempTrend);

  return result;
}

std::string MeasType::statsJson(unsigned long long timeFrom, unsigned long long timeTo) {
  std::vector < std::shared_ptr<MeasTrend> > result = getTrendsBetween(timeFrom, timeTo);

  std::string trendString = "[";

  for(std::vector<std::shared_ptr<MeasTrend>>::iterator it = result.begin(); it != result.end(); ++it) {
    std::shared_ptr<MeasTrend> mt = *it;

    trendString += "{";
    trendString += "\"type\":" + std::to_string(mt->type()) + ",";
    trendString += "\"timeDiff\":" + std::to_string(mt->timeDiff() ) + ",";
    trendString += "\"valueDiff\":" + std::to_string(mt->valueDiff() ) + ",";
    trendString += "\"trend\":" + std::to_string(mt->trend() ) + ",";

    trendString += "\"rawFrom\":" + std::to_string(mt->rawFrom) + ",";
    trendString += "\"rawTo\":" + std::to_string(mt->rawTo) + ",";
    trendString += "\"valueFrom\":" + std::to_string(mt->valueFrom) + ",";
    trendString += "\"valueTo\":" + std::to_string(mt->valueTo) + ",";
    trendString += "\"timeFrom\":" + std::to_string(mt->timeFrom) + ",";
    trendString += "\"timeTo\":" + std::to_string(mt->timeTo);

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

std::string MeasType::rawToFormattedValue(unsigned int raw) {
  return valueToFormatted(rawToValue(raw));
}

std::string MeasType::valueToFormatted(double v) {
  return formattedValue(v, unit);
}

std::string MeasType::formattedValue(double v, std::string un) {
  std::string tmpString;
  std::ostringstream os;
  os << std::setprecision(5) << v;
  tmpString = os.str();
  if (un != "") {
    tmpString += " ";
    tmpString += un;
  }
  return tmpString;
}

std::string MeasType::valueAtFormatted(unsigned long int i) {
  return rawToFormattedValue(buffer->at(i));
}

std::string MeasType::lastFormattedValue() {
  return rawToFormattedValue(lastRaw());
}
