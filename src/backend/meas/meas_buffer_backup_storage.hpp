#ifndef MEAS_BUFFER_BACKUP_STORAGE_H
#define MEAS_BUFFER_BACKUP_STORAGE_H

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <mutex>

#include "meas_type_array.hpp"
#include "meas_type.hpp"
#include "meas_buffer.hpp"
#include "../log/log_array.hpp"

class MeasBufferBackupStorage {
 public:
  MeasBufferBackupStorage();
  void start();
  void stop();
  void performDump();
  void performRestore();

  std::string pathForMeasType(std::shared_ptr<MeasType> measType);

  std::string path;
  unsigned long long cycleInterval;
  unsigned long long thresholdTimeRange;
  unsigned long long usDelay;

  bool isRunning;
  bool ready;
  bool changing;
  std::mutex shutdownMutex;

  std::shared_ptr<MeasTypeArray> measTypeArray;

  std::shared_ptr<LogArray> logArray;
};

//#include "meas_buffer_backup_storage.cpp"
#endif
