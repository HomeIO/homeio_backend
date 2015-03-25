MeasTypeStorage::MeasTypeStorage() {
  minTimeDiffToStore = 1000;
  maxTimeDiffToStore = 3600000;
  valueDiffToStore = 1.0;

}

void MeasTypeStorage::clearBuffer() {
  buffer.clear();
}

vector < StorageHash > MeasTypeStorage::prepareStorageBuffer() {
  vector < StorageHash > shBuffer;
  unsigned long int i = 0;
  bool storeElement = false;
  unsigned long long currentTimeFrom, currentTimeTo;
  
  // empty buffer, nothing to do
  if (buffer.size() == 0) {
    return shBuffer;
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
      shBuffer.push_back(*sh);
      
      sh = new StorageHash( currentTimeFrom, currentTimeTo, *it ); 
    }
    
    // end of loop
    i++;
  }
  
  return shBuffer;
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


string MeasTypeStorage::storageJson() {
  string json;
  
  vector < StorageHash > shBuffer = prepareStorageBuffer();
  
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
  
