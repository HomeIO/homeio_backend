#ifndef MEAS_TYPE_STORAGE_H
#define	MEAS_TYPE_STORAGE_H

#include <time.h>
#include <string>

#include "storage_hash.hpp"

class MeasTypeStorage {
public:
  MeasTypeStorage();
  
  unsigned long long timeFrom, timeTo;
  unsigned long int interval;
  
  unsigned long int minTimeDiffToStore;
  unsigned long int maxTimeDiffToStore;
  double valueDiffToStore;
  
  vector < double > buffer;
  
  void clearBuffer();
  vector < StorageHash > prepareStorageBuffer();
  string storageJson();
};

#include "meas_type_storage.cpp"
#endif	
