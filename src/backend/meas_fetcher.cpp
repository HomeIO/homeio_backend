MeasFetcher::MeasFetcher() {
  // default intervals
  betweenMeasInterval = 5000;
  cycleInterval = 20000;
  maxBufferSize = 1000000; 
}

void MeasFetcher::start() {
  ioProxy->prepareSocket();
  
  unsigned int i;
  while(true) {
    for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
      m->fetch();
      usleep(betweenMeasInterval);
    } 

    usleep(cycleInterval);
  }
}
