#ifndef MEAS_TYPE_STORAGE_H
#define	MEAS_TYPE_STORAGE_H

#include <cmath>
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
  
  StorageHash *sh;
  void clearBuffer();
  vector < StorageHash > prepareStorageBuffer();
  string storageJson();
  
  bool valueDiffCheck(double p, double n);
  bool minTimeDiffCheck(unsigned long long p, unsigned long long n);
  bool maxTimeDiffCheck(unsigned long long p, unsigned long long n);
};

#include "meas_type_storage.cpp"
#endif	
