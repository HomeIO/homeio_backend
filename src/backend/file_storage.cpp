FileStorage::FileStorage() {
  cycleInterval = 20000000; // 10000000
  lastTime = mTime();
  path = "data";
}

void FileStorage::start()
{
  // wait 10s to warm up - get enough measurements
  usleep(1000000);
  
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  // TODO maybe add S_IWOTH 
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
  
  while(true) {
    performMeasStore();
    logInfo("FileStorage");
    usleep(cycleInterval);
  };
}

void FileStorage::performMeasStore() {
  currentTime = mTime();
  
  logInfo("FileStorage - start meas file storage");
  
  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    storeMeasArray(*it, it->storageArray(lastTime, currentTime));
  }
  
  lastTime = currentTime;
  
  logInfo("FileStorage - end meas file storage");
}

void FileStorage::storeMeasArray(MeasType measType, vector <StorageHash> storageVector) {
  if (storageVector.size() == 0) {
    logInfo("FileStorage [" + measType.name + "] no data to store");
    return;
  }
  
  unsigned long int measCount = 0;
  ofstream outfile;
  string filename = path + "/" + measType.name + "_" + to_string(mTime()) + ".csv";

  logInfo("FileStorage [" + measType.name + "] path " + filename);
  
  outfile.open(filename, ios_base::app);
  for(std::vector<StorageHash>::iterator it = storageVector.begin(); it != storageVector.end(); ++it) {
    outfile << measType.name << "; ";
    outfile << to_string(it->timeFrom) << "; ";
    outfile << to_string(it->timeTo) << "; ";
    outfile << to_string(it->value) << endl;
    
    measCount++;
  }
  
  logInfo("FileStorage [" + measType.name + "] stored " + to_string(measCount));
}
