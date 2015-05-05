#ifndef FILE_STORAGE_H
#define	FILE_STORAGE_H

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <vector>

#include "meas_type_array.hpp"
#include "meas_type.hpp"
#include "storage_hash.hpp"

using namespace std;

class FileStorage {
public:
  FileStorage();
  void start();
  void performMeasStore();
  void storeMeasArray(MeasType* measType, vector <StorageHash> storageVector);

  string path;
  string measPrefix;
  unsigned long int cycleInterval;
  unsigned long long lastTime, currentTime;
  
  unsigned long long usDelay;
  
  MeasTypeArray *measTypeArray;
};

#include "file_storage.cpp"
#endif

