#include "meas_buffer_backup_storage.hpp"

MeasBufferBackupStorage::MeasBufferBackupStorage() {
  cycleInterval = 10*60*1000*1000;
  thresholdTimeRange = cycleInterval * 2;
  usDelay = 10000;
  path = "data";

  isRunning = true;
  ready = false;
  changing = false;
  work = false;
  currentMeasIndex = 0;
  intStatus = MEAS_BUFFER_BACKUP_STATUS_NULL;
}

void MeasBufferBackupStorage::start() {
  changing = true;

  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
  // TODO maybe add S_IWOTH
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html

  // not need to wait, because other modules can be run
  ready = true;
  changing = false;

  // first run not at start
  Helper::longSleep(usDelay);

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
  changing = true;

  shutdownMutex.lock();
  // perform the last storage before exiting
  performDump();
  shutdownMutex.unlock();
  logArray->log("MeasBufferBackupStorage", "stop");

  changing = false;
  ready = false;
}


std::string MeasBufferBackupStorage::pathForMeasType(std::shared_ptr<MeasType> measType) {
  std::string filename = path + "/buffer_" + measType->name + ".txt";
  return filename;
}

void MeasBufferBackupStorage::performDump() {
  intStatus = MEAS_BUFFER_BACKUP_STATUS_DUMP;
  work = true;
  unsigned long int i = 0;
  logArray->log("MeasBufferBackupStorage", "start");

  currentMeasIndex = 0;
  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> measType = *it;
    std::ofstream outfile;
    std::string filename = pathForMeasType(measType);
    std::shared_ptr<MeasBuffer> measBuffer = measType->buffer;

    logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] DUMP path " + filename);

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
    currentMeasIndex++;
  }

  logArray->log("MeasBufferBackupStorage", "end");
  work = false;
  intStatus = MEAS_BUFFER_BACKUP_STATUS_NULL;
}

void MeasBufferBackupStorage::performRestore() {
  work = true;
  intStatus = MEAS_BUFFER_BACKUP_STATUS_RESTORE;

  unsigned long int i = 0;
  std::string line;
  unsigned int tmpRaw;
  unsigned long long storeTime, count, interval;
  currentMeasIndex = 0;

  logArray->log("MeasBufferBackupStorage", "start");

  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> measType = *it;
    std::ifstream infile;
    std::string filename = pathForMeasType(measType);
    std::shared_ptr<MeasBuffer> measBuffer = measType->buffer;

    infile.open(filename, std::ios_base::in);

    if (infile.good()) {
      // buffer file exists
      logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE path " + filename);
      logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE interval=" + std::to_string(measBuffer->calcInterval()) + ",count=" + std::to_string(measBuffer->count));

      // time of dump
      infile >> storeTime;

      if (thresholdTimeRange > (Helper::mTime() - storeTime) ) {
        // fresh enough

        infile >> measBuffer->maxSize;
        // resize buffer
        measBuffer->clearAndResize(measBuffer->maxSize);

        // count
        infile >> count;
        logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE updatedMaxSize=" + std::to_string(measBuffer->maxSize) + ",count=" + std::to_string(count));

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
        logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE loaded");

        // modify times
        measBuffer->lastTimeForCount = Helper::mTime();
        measBuffer->firstTime = measBuffer->lastTimeForCount - count * interval;

        logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE POST loadedInterval=" + std::to_string(interval) + ",interval=" + std::to_string(measBuffer->calcInterval()) + ",count=" + std::to_string(measBuffer->count));

        // extension - backend remove spikes
        if (measType->extBackendRemoveSpikes) {
          measBuffer->removeSpikes = measType->extBackendRemoveSpikes;
          measBuffer->filterStoredSpikes();
          logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE removed spikes");
        }

      } else {
        logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE buffer is too old");
      }


    } else {
      // buffer file not exists
      logArray->log("MeasBufferBackupStorage", "[" + measType->name + "] RESTORE file not exists " + filename);
    }

    infile.close();
    currentMeasIndex++;
  }

  logArray->log("MeasBufferBackupStorage", "end");
  work = false;
  intStatus = MEAS_BUFFER_BACKUP_STATUS_NULL;
}

std::string MeasBufferBackupStorage::statusText() {
  if (intStatus == MEAS_BUFFER_BACKUP_STATUS_RESTORE) {
    return "Restoring " + std::to_string(currentMeasIndex + 1) + "/" + std::to_string(measTypeArray->measTypes.size());
  }
  if (intStatus == MEAS_BUFFER_BACKUP_STATUS_DUMP) {
    return "Store " + std::to_string(currentMeasIndex + 1) + "/" + std::to_string(measTypeArray->measTypes.size());
  }
  return "";
}
