#include "file_storage.hpp"

FileStorage::FileStorage() {
  cycleInterval = 20000000; // 10000000
  usDelay = 1000000; // wait 10s to warm up - get enough measurements
  lastTime = mTime();
  path = "data";

  isRunning = true;
}

void FileStorage::start()
{
  // create path at start, no wait
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
  // TODO maybe add S_IWOTH
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html

  longSleep(usDelay);

  // wait for enough measurements
  measTypeArray->delayTillReady();

  while(isRunning) {
    performMeasStore();
    logInfo("FileStorage");

    longSleep(cycleInterval);
  };
}

void FileStorage::stop() {
  isRunning = false;
  // wait for end storage
  shutdownMutex.lock();
  logInfo("FileStorage - stop");

  // reset mutex
  shutdownMutex.unlock();
  // perform the last storage before exiting
  performMeasStore();
  logInfo("FileStorage - terminal storage done");
}

void FileStorage::performMeasStore() {
  shutdownMutex.lock();

  currentTime = mTime();

  logInfo("FileStorage - start meas file storage");

  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    storeMeasArray(&*it, it->storageArray(it->lastStored, currentTime));
  }

  logInfo("FileStorage - end meas file storage");

  shutdownMutex.unlock();
}

void FileStorage::storeMeasArray(MeasType* measType, vector <StorageHash> storageVector) {
  if (storageVector.size() == 0) {
    logInfo("FileStorage [" + measType->name + "] no data to store");
    return;
  }
  else {
    // mark last time stored
    measType->lastStored = storageVector.back().timeTo;
  }

  unsigned long int measCount = 0;
  ofstream outfile;
  string currentDate( currentDateSafe() );
  string filename = path + "/" + measType->name + "_" + currentDate + ".csv";

  logInfo("FileStorage [" + measType->name + "] path " + filename);

  outfile.open(filename, ios_base::app);
  for(std::vector<StorageHash>::iterator it = storageVector.begin(); it != storageVector.end(); ++it) {
    outfile << measType->name << "; ";
    outfile << to_string(it->timeFrom) << "; ";
    outfile << to_string(it->timeTo) << "; ";
    outfile << to_string(it->value) << endl;

    measCount++;
  }
  outfile.close();

  logInfo("FileStorage [" + measType->name + "] stored " + to_string(measCount));
}
