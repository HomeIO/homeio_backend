#include "meas_buffer_backup_storage.hpp"

MeasBufferBackupStorage::MeasBufferBackupStorage() {
  cycleInterval = 10*60*1000*1000;
  thresholdTimeRange = cycleInterval * 2;
  usDelay = 10000;
  path = "data";

  isRunning = true;
  ready = false;
}

void MeasBufferBackupStorage::start() {
  Helper::longSleep(usDelay);

  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
  // TODO maybe add S_IWOTH
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html

  // not need to wait, because other modules can be run
  ready = true;

  // wait for enough measurements
  measTypeArray->delayTillReady();

  // debug loop
  /*
  cycleInterval = 10*1000*1000;
  while(isRunning) {
    performDump();
    performRestore();
    Helper::longSleep(cycleInterval);
  };
  */

  while(isRunning) {
    shutdownMutex.lock();
    performDump();
    shutdownMutex.unlock();
    Helper::longSleep(cycleInterval);
  };
}

void MeasBufferBackupStorage::stop() {
  isRunning = false;
  shutdownMutex.lock();
  // perform the last storage before exiting
  performDump();
  shutdownMutex.unlock();
  logArray->log("MeasBufferBackupStorage", "stop");
}


std::string MeasBufferBackupStorage::pathForMeasType(MeasType *measType) {
  std::string filename = path + "/buffer_" + measType->name + ".txt";
  return filename;
}

void MeasBufferBackupStorage::performDump() {
  unsigned long int i = 0;
  logArray->log("MeasBufferBackupStorage", "start");

  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::ofstream outfile;
    std::string filename = pathForMeasType(&*it);
    MeasBuffer *measBuffer = it->buffer;

    logArray->log("MeasBufferBackupStorage", "[" + it->name + "] DUMP path " + filename);

    outfile.open(filename, std::ios_base::out);

    outfile << std::to_string(Helper::mTime()) << "\n";
    outfile << std::to_string(measBuffer->maxSize) << "\n";
    outfile << std::to_string(measBuffer->count) << "\n";
    outfile << std::to_string(measBuffer->offset) << "\n";
    outfile << std::to_string(measBuffer->lastTimeForCount) << "\n";
    outfile << std::to_string(measBuffer->firstTime) << "\n";
    outfile << std::to_string(measBuffer->calcInterval()) << "\n";

    outfile << "\n\n";

    for(i = measBuffer->count; i > 0; i--) {
      outfile << std::to_string(measBuffer->at(i)) << "\n";
    }

    outfile.close();
  }

  logArray->log("MeasBufferBackupStorage", "end");
}

void MeasBufferBackupStorage::performRestore() {
  unsigned long int i = 0;
  std::string line;
  unsigned int tmpRaw;
  unsigned long long storeTime, count, interval;
  //struct stat sBuffer;

  logArray->log("MeasBufferBackupStorage", "start");

  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::ifstream infile;
    std::string filename = pathForMeasType(&*it);
    MeasBuffer *measBuffer = it->buffer;

    infile.open(filename, std::ios_base::in);

    if (infile.good()) {
      // buffer file exists
      logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE path " + filename);
      logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE interval=" + std::to_string(measBuffer->calcInterval()) + ",count=" + std::to_string(measBuffer->count));

      // time of dump
      infile >> storeTime;

      if (thresholdTimeRange > (Helper::mTime() - storeTime) ) {
        // fresh enough

        infile >> measBuffer->maxSize;
        // resize buffer
        measBuffer->clearAndResize(measBuffer->maxSize);

        // count
        infile >> count;
        logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE updatedMaxSize=" + std::to_string(measBuffer->maxSize) + ",count=" + std::to_string(count));

        // offset - not used now
        infile >> line;
        // lastTimeForCount - not used now
        infile >> line;
        // firstTime - not needed now
        infile >> line;

        // interval - needed for recalculate times to meet proper interval calculation
        // time offset error is not a very bad thing
        infile >> interval;

        // 2 empty lines
        infile >> line;
        infile >> line;

        for(i = count; i > 0; i--) {
          infile >> tmpRaw;
          measBuffer->add(tmpRaw);
        }
        logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE loaded");

        // modify times
        measBuffer->lastTimeForCount = Helper::mTime();
        measBuffer->firstTime = measBuffer->lastTimeForCount - count * interval;

        logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE POST loadedInterval=" + std::to_string(interval) + ",interval=" + std::to_string(measBuffer->calcInterval()) + ",count=" + std::to_string(measBuffer->count));

        // extension - backend remove spikes
        if (it->extBackendRemoveSpikes) {
          measBuffer->removeSpikes = it->extBackendRemoveSpikes;
          measBuffer->filterStoredSpikes();
          logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE removed spikes");
        }

      } else {
        logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE buffer is too old");
      }


    } else {
      // buffer file not exists
      logArray->log("MeasBufferBackupStorage", "[" + it->name + "] RESTORE file not exists " + filename);
    }

    infile.close();

  }

  logArray->log("MeasBufferBackupStorage", "end");
}
