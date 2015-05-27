MeasBufferBackupStorage::MeasBufferBackupStorage() {
  //cycleInterval = 10*60*1000*1000;
  cycleInterval = 10*1000*1000;
  usDelay = 1000000;
  path = "data";
}

void MeasBufferBackupStorage::start()
{
  usleep(usDelay);
  
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
  // TODO maybe add S_IWOTH 
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
  
  while(true) {
    performMeasStore();
    performLoad();
    usleep(cycleInterval);
  };
}

string MeasBufferBackupStorage::pathForMeasType(MeasType *measType) {
  string filename = path + "/buffer_" + measType->name + ".txt";
  return filename;
}

// TODO add parameters used in MeasBuffer::calcInterval()

void MeasBufferBackupStorage::performMeasStore() {
  unsigned long int i = 0;
  logInfo("MeasBufferBackupStorage - start");
  
  for(vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    ofstream outfile;
    string filename = pathForMeasType(&*it);
    MeasBuffer *measBuffer = it->buffer;
    
    logInfo("MeasBufferBackupStorage [" + it->name + "] path " + filename);
  
    outfile.open(filename, ios_base::out);

    outfile << to_string(measBuffer->maxSize) << "\n";
    outfile << to_string(measBuffer->count) << "\n";
    outfile << to_string(measBuffer->offset) << "\n";
    outfile << to_string(measBuffer->lastTimeForCount) << "\n";
    outfile << to_string(measBuffer->firstTime) << "\n";

    outfile << "\n\n";
    
    for(i = measBuffer->count; i > 0; i--) {
      outfile << to_string(measBuffer->at(i)) << "\n";
    }
    
    outfile.close();
  }
  
  logInfo("MeasBufferBackupStorage - end");
}

void MeasBufferBackupStorage::performLoad() {
  unsigned long int i = 0;
  string line;
  unsigned int tmpRaw, count;
  unsigned long long firstTime, lastTimeForCount;
  
  logInfo("MeasBufferBackupStorage - start");
  
  for(vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    ifstream infile;
    string filename = pathForMeasType(&*it);
    MeasBuffer *measBuffer = it->buffer;
    
    logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] path " + filename);
    logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] PRE interval " + to_string(measBuffer->calcInterval()));
    
    infile.open(filename, ios_base::in);

    infile >> measBuffer->maxSize;
    logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] updated max size " + to_string(measBuffer->maxSize));
    
    // TODO resize buffer
    //measBuffer->buffer = 
    
    // count
    infile >> count;
    logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] count " + to_string(count));
    
    // offset - not used now; and 2 new lines
    infile >> line;
    // needer for proper interval calculation
    infile >> lastTimeForCount;
    infile >> firstTime;
    // 2 empty lines
    infile >> line;
    infile >> line;
    
    for(i = count; i > 0; i--) {
      infile >> tmpRaw;
      measBuffer->add(tmpRaw);
    }
    logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] loaded");
    
    measBuffer->lastTimeForCount = lastTimeForCount;
    measBuffer->firstTime = firstTime;
    
    logInfo("MeasBufferBackupStorage RESTORE [" + it->name + "] PRE interval " + to_string(measBuffer->calcInterval()));
    
    infile.close();
  }
  
  logInfo("MeasBufferBackupStorage - end");
}