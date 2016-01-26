#include "file_storage.hpp"

FileStorage::FileStorage() {
  cycleInterval = 20000000; // 10000000
  usDelay = 1000000; // wait 10s to warm up - get enough measurements
  lastTime = Helper::mTime();
  path = "data";

  isRunning = true;
  ready = false;
}

void FileStorage::start() {
  Helper::longSleep(usDelay);

  // not need to wait, because other modules can be run
  ready = true;

  // wait for enough measurements
  measTypeArray->delayTillReady();

  while(isRunning) {
    shutdownMutex.lock();
    performMeasStore();
    shutdownMutex.unlock();
    logArray->log("FileStorage", "done");

    Helper::longSleep(cycleInterval);
  };
}

void FileStorage::stop() {
  isRunning = false;
  shutdownMutex.lock();
  // perform the last storage before exiting
  performMeasStore();
  shutdownMutex.unlock();

  logArray->log("FileStorage", "stop");
}

void FileStorage::performMeasStore() {
  currentTime = Helper::mTime();

  logArray->log("FileStorage", "start meas file storage");

  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    storeMeasArray(&*it, it->storageArray(it->lastStored, currentTime));
  }

  logArray->log("FileStorage", "finish meas file storage");
}

void FileStorage::storeMeasArray(MeasType* measType, std::vector <StorageHash> storageVector) {
  if (storageVector.size() == 0) {
    logArray->log("FileStorage", "[" + measType->name + "] no data to store");
    return;
  } else {
    // mark last time stored
    measType->lastStored = storageVector.back().timeTo;
  }

  unsigned long int measCount = 0;
  std::ofstream outfile;
  std::string currentDate = Helper::currentDateSafe();
  std::string filename = path + "/" + measType->name + "_" + currentDate + ".csv";

  logArray->log("FileStorage", "[" + measType->name + "] path: " + filename);

  outfile.open(filename, std::ios_base::app);
  for(std::vector<StorageHash>::iterator it = storageVector.begin(); it != storageVector.end(); ++it) {
    outfile << measType->name << "; ";
    outfile << std::to_string(it->timeFrom) << "; ";
    outfile << std::to_string(it->timeTo) << "; ";
    outfile << std::to_string(it->value) << std::endl;

    measCount++;
  }
  outfile.close();

  logArray->log("FileStorage", "[" + measType->name + "] stored with " + std::to_string(measCount) + " meas");
}
