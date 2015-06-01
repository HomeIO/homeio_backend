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
  void performDump();
  void performRestore();
    
  string pathForMeasType(MeasType *measType);

  string path;
  unsigned long long cycleInterval;
  unsigned long long thresholdTimeRange;
  unsigned long long usDelay;
 
  MeasTypeArray *measTypeArray;
};

#include "meas_buffer_backup_storage.cpp"
#endif

