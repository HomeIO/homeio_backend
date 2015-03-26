FileStorage::FileStorage() {
  cycleInterval = 10000000;
  lastTime = mTime();
}

void FileStorage::start()
{
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  
  while(true) {
    performMeasStore();
    logInfo("FileStorage");
    usleep(cycleInterval);
  };
}

void FileStorage::performMeasStore() {
  currentTime = mTime();
  
  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    cout << it->name << endl;
    //storeMeasArray(*it, it->storageArray(lastTime, currentTime));
  }
  
  lastTime = currentTime;
}

/*
void FileStorage::storeMeasArray(MeasType measType, vector <StorageHash> storageVector) {
  ofstream outfile;
  string filename = path + "/" + measType.name + "_" + to_string(mTime()) + ".csv";

  outfile.open(filename, ios_base::app);
  for(std::vector<StorageHash>::iterator it = storageVector.begin(); it != storageVector.end(); ++it) {
    outfile << measType.name << "; ";
    outfile << to_string(it->timeFrom) << "; ";
    outfile << to_string(it->timeTo) << "; ";
    outfile << to_string(it->value) << endl;
  }
}
*/