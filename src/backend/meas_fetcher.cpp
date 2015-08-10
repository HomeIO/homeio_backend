MeasFetcher::MeasFetcher() {
  // default intervals
  betweenMeasInterval = 5000;
  cycleInterval = 20000;
  maxBufferSize = 1000000;
  
  isRunning = true;
}

void MeasFetcher::start() {
  ioProxy->prepareSocket();
  
  while(isRunning) {
    for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
      shutdownMutex.lock();
      m->fetch();
      shutdownMutex.unlock();
      
      longSleep(betweenMeasInterval);
      
      // if there is more than 2 measurements after fetching
      // other utils (overseer, storage, ...) can be started
      if ( (measTypeArray->isReady == false) && (m->buffer->count > 2) ) {
        measTypeArray->isReady = true;
      }
    } 

    longSleep(cycleInterval);
  }
}

void MeasFetcher::stop() {
  shutdownMutex.lock();
  logInfo("MeasFetcher - stop");
}
