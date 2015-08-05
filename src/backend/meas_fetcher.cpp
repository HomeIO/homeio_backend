MeasFetcher::MeasFetcher() {
  // default intervals
  betweenMeasInterval = 5000;
  cycleInterval = 20000;
  maxBufferSize = 1000000; 
}

void MeasFetcher::start() {
  ioProxy->prepareSocket();
  
  while(true) {
    for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
      m->fetch();
      usleep(betweenMeasInterval);
      
      // if there is more than 2 measurements after fetching
      // other utils (overseer, storage, ...) can be started
      if ( (measTypeArray->isReady == false) && (m->buffer->count > 2) ) {
        measTypeArray->isReady = true;
      }
    } 

    usleep(cycleInterval);
  }
}
