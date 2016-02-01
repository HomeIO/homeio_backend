#ifndef FILE_STORAGE_H
#define	FILE_STORAGE_H

#include <string>
#include <vector>
#include <mutex>

#include "../utils/helper.hpp"
#include "meas_type_array.hpp"
#include "meas_type.hpp"
#include "storage_hash.hpp"
#include "../log/log_array.hpp"

class FileStorage {
 public:
  FileStorage();
  void start();
  void stop();
  void performMeasStore();
  void storeMeasArray(std::shared_ptr<MeasType>, std::vector <std::shared_ptr<StorageHash>> storageVector);

  std::string path;
  std::string measPrefix;
  unsigned long int cycleInterval;
  unsigned long long lastTime, currentTime;

  unsigned long long usDelay;

  bool isRunning;
  bool ready;
  bool changing;
  std::mutex shutdownMutex;

  std::shared_ptr<MeasTypeArray> measTypeArray;

  std::shared_ptr<LogArray> logArray;
};

//#include "file_storage.cpp"
#endif
