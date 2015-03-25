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
  StorageHash *sh;
  unsigned long int i = 0;
  
  // empty buffer, nothing to do
  if (buffer.size() == 0) {
    return shBuffer;
  }
  
  // create first StorageHash
  sh = new StorageHash( timeFrom, timeFrom + i*interval, buffer.front() ); 
  
  for(std::vector<double>::iterator it = buffer.begin(); it != buffer.end(); ++it) {
    //if (it->name == s) {
    //  return &*it;
    //}
  }
  
  return shBuffer;
}

string MeasTypeStorage::storageJson() {
  string json;
  
  vector < StorageHash > shBuffer = prepareStorageBuffer();
  
  json = "[";
  
  // iteration
  // TODO add code here, important change of value, required time change
  // TODO process to some kind of structure
  
  json += "]";
  
  return json;
}
  
