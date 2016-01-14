#include "meas_type_storage.hpp"

MeasTypeStorage::MeasTypeStorage() {
  minTimeDiffToStore = 1000;
  maxTimeDiffToStore = 3600000;
  valueDiffToStore = 1.0;

}

void MeasTypeStorage::clearBuffer() {
  buffer.clear();
}

std::vector < StorageHash > MeasTypeStorage::prepareStorageBuffer() {
  storageBuffer.clear();
  unsigned long int i = 0;
  bool storeElement = false;
  unsigned long long currentTimeFrom, currentTimeTo;

  // empty buffer, nothing to do
  if (buffer.size() == 0) {
    return storageBuffer;
  }

  // create first StorageHash
  sh = new StorageHash( timeFrom, timeFrom + interval, buffer.front() );

  for(std::vector<double>::iterator it = buffer.begin(); it != buffer.end(); ++it) {
    storeElement = false;
    currentTimeFrom = timeFrom + i*interval;
    currentTimeTo = timeFrom + (i+1)*interval;

    // value change - store
    if ( valueDiffCheck(sh->value, *it ) ) {
      storeElement = true;
    }
    // min time - no store if time is very short
    if ( minTimeDiffCheck(sh->timeTo, currentTimeTo ) == false ) {
      storeElement = false;
    }
    // max time - must store
    if ( maxTimeDiffCheck(sh->timeTo, currentTimeTo ) ) {
      storeElement = true;
    }

    // if store, push hash to buffer and create new one
    if (storeElement) {
      sh->timeTo = currentTimeFrom;
      storageBuffer.push_back(*sh);

      sh = new StorageHash( currentTimeFrom, currentTimeTo, *it );
    }

    // end of loop
    i++;
  }

  return storageBuffer;
}

bool MeasTypeStorage::valueDiffCheck(double p, double n) {
  return ( fabs(p - n) >= fabs(valueDiffToStore) );
}

bool MeasTypeStorage::minTimeDiffCheck(unsigned long long p, unsigned long long n) {
  if (p > n) {
    return ( (p - n) >= (unsigned long long)minTimeDiffToStore );
  } else {
    return ( (n - p) >= (unsigned long long)minTimeDiffToStore );
  }
}

bool MeasTypeStorage::maxTimeDiffCheck(unsigned long long p, unsigned long long n) {
  if (p > n) {
    return ( (p - n) >= (unsigned long long)maxTimeDiffToStore );
  } else {
    return ( (n - p) >= (unsigned long long)maxTimeDiffToStore );
  }
}


std::string MeasTypeStorage::storageBufferJson() {
  std::string json;

  std::vector < StorageHash > shBuffer = prepareStorageBuffer();

  json = "[";

  for(std::vector<StorageHash>::iterator it = shBuffer.begin(); it != shBuffer.end(); ++it) {
    json += it->toJson() + ",";
  }
  // remove last coma
  if (json[json.size() - 1] == ',') {
    json.resize(json.size() - 1);
  }

  json += "]";

  return json;
}

std::string MeasTypeStorage::storageFullJson() {
  std::string detailsString, response;

  // just debug data
  detailsString = "{";
  detailsString += "\"indexFrom\":" + std::to_string(indexFrom) + ",";
  detailsString += "\"indexTo\":" + std::to_string(indexTo) + ",";
  detailsString += "\"timeFrom\":" + std::to_string(timeFrom) + ",";
  detailsString += "\"timeTo\":" + std::to_string(timeTo) + ",";
  detailsString += "\"interval\":" + std::to_string(interval) + ",";
  detailsString += "\"count\":" + std::to_string(count) + ",";
  detailsString += "\"lastTime\":" + std::to_string(lastTime) + ",";
  detailsString += "\"firstTime\":" + std::to_string(firstTime);
  detailsString += "}";

  response = "{";
  response += "\"details\":" + detailsString + ",";
  response += "\"storageArray\":" + storageBufferJson();
  response += "}";

  return response;
}
