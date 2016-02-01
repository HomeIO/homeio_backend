#ifndef MEAS_TYPE_STORAGE_H
#define	MEAS_TYPE_STORAGE_H

#include <cmath>
#include <string>
#include <vector>
#include <memory>

#include "storage_hash.hpp"

class MeasTypeStorage {
 public:
  MeasTypeStorage();

  unsigned long long timeFrom, timeTo;
  unsigned long int interval;
  // just a temp storage, not used directly
  unsigned long int indexFrom, indexTo, count;
  unsigned long long lastTime, firstTime;

  unsigned long int minTimeDiffToStore;
  unsigned long int maxTimeDiffToStore;
  double valueDiffToStore;

  std::vector < double > buffer;

  std::shared_ptr<StorageHash> sh;
  void clearBuffer();
  std::vector < std::shared_ptr<StorageHash> > storageBuffer;
  std::vector < std::shared_ptr<StorageHash> > prepareStorageBuffer();
  std::string storageBufferJson();
  std::string storageFullJson();


  bool valueDiffCheck(double p, double n);
  bool minTimeDiffCheck(unsigned long long p, unsigned long long n);
  bool maxTimeDiffCheck(unsigned long long p, unsigned long long n);
};

//#include "meas_type_storage.cpp"
#endif
