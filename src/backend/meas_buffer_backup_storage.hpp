#ifndef MEAS_BUFFER_BACKUP_STORAGE_H
#define MEAS_BUFFER_BACKUP_STORAGE_H

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <vector>

#include "meas_type_array.hpp"
#include "meas_type.hpp"

using namespace std;

class MeasBufferBackupStorage {
public:
  MeasBufferBackupStorage();
  void start();
  void performMeasStore();
  void performLoad();
    
  string pathForMeasType(MeasType *measType);

  string path;
  unsigned long int cycleInterval;
  unsigned long long usDelay;
 
  MeasTypeArray *measTypeArray;
};

#include "meas_buffer_backup_storage.cpp"
#endif

