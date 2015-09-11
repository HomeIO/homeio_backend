MeasBufferBackupStorage::MeasBufferBackupStorage() {
  cycleInterval = 10*60*1000*1000;
  thresholdTimeRange = cycleInterval * 2;
  usDelay = 30*1000*1000;
  path = "data";
  
  isRunning = true;
}

void MeasBufferBackupStorage::start()
{
  longSleep(usDelay);
  
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
  // TODO maybe add S_IWOTH 
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
  
  // wait for enough measurements
  measTypeArray->delayTillReady();
  
  // debug loop
  /*
  cycleInterval = 10*1000*1000;
  while(isRunning) {
    performDump();
    performRestore();
    longSleep(cycleInterval);
  };
  */

  while(isRunning) {
    performDump();
    longSleep(cycleInterval);
  };
}

void MeasBufferBackupStorage::stop() {
  isRunning = false;
  shutdownMutex.lock();
  logInfo("MeasBufferBackupStorage - stop");
  
  // reset mutex
  shutdownMutex.unlock();
  // perform the last storage before exiting
  performDump();
  logInfo("MeasBufferBackupStorage - terminal storage done");
}


string MeasBufferBackupStorage::pathForMeasType(MeasType *measType) {
  string filename = path + "/buffer_" + measType->name + ".txt";
  return filename;
}

void MeasBufferBackupStorage::performDump() {
  shutdownMutex.lock();
  
  unsigned long int i = 0;
  logInfo("MeasBufferBackupStorage - start");
  
  for(vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    ofstream outfile;
    string filename = pathForMeasType(&*it);
    MeasBuffer *measBuffer = it->buffer;
    
    logInfo("MeasBufferBackupStorage DUMP [" + it->name + "] path " + filename);
  
    outfile.open(filename, ios_base::out);

    outfile << to_string(mTime()) << "\n";
    outfile << to_string(measBuffer->maxSize) << "\n";
    outfile << to_string(measBuffer->count) << "\n";
    outfile << to_string(measBuffer->offset) << "\n";
    outfile << to_string(measBuffer->lastTimeForCount) << "\n";
    outfile << to_string(measBuffer->firstTime) << "\n";
    outfile << to_string(measBuffer->calcInterval()) << "\n";
    
    outfile << "\n\n";
    
    for(i = measBuffer->count; i > 0; i--) {
      outfile << to_string(measBuffer->at(i)) << "\n";
    }
    
    outfile.close();
  }
  
  logInfo("MeasBufferBackupStorage - end");
  shutdownMutex.unlock();
}

void MeasBufferBackupStorage::performRestore() {
  unsigned long int i = 0;
  string line;
  unsigned int tmpRaw;
  unsigned long long storeTime, count, interval;
  //struct stat sBuffer;   
  
  logInfo("MeasBufferBackupStorage - start");
  
  for(vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    ifstream infile;
    string filename = pathForMeasType(&*it);
    MeasBuffer *measBuffer = it->buffer;
    
    infile.open(filename, ios_base::in);
    
    if (infile.good()) {
      // buffer file exists
      logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] path " + filename);
      logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] PRE interval " + to_string(measBuffer->calcInterval()));
      logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] PRE count " + to_string(measBuffer->count));

      // time of dump
      infile >> storeTime;
    
      if (thresholdTimeRange > (mTime() - storeTime) ) {
        // fresh enough

        infile >> measBuffer->maxSize;
        // resize buffer
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] updated max size " + to_string(measBuffer->maxSize));
        measBuffer->clearAndResize();
    
        // count
        infile >> count;
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] count " + to_string(count));
    
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
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] loaded");
    
        // modify times
        measBuffer->lastTimeForCount = mTime();
        measBuffer->firstTime = measBuffer->lastTimeForCount - count * interval;
    
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] loaded interval " + to_string(interval));
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] POST interval " + to_string(measBuffer->calcInterval()));
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] POST count " + to_string(measBuffer->count));
        
        // extension - backend remove spikes
        if (it->extBackendRemoveSpikes) {
          measBuffer->removeSpikes = it->extBackendRemoveSpikes;
          measBuffer->filterStoredSpikes();
          logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] removed spikes");
        }
        
      }
      else {
        logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] buffer is too old");
      }
    
      
    }
    else {
      // buffer file not exists
      logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] file not exists " + filename);
    }
    
  infile.close();
    
  }
  
  logInfo("MeasBufferBackupStorage - end");
}