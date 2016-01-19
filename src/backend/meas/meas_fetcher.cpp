#include "meas_fetcher.hpp"

MeasFetcher::MeasFetcher() {
  ready = false;

  // default intervals
  betweenMeasInterval = 5000;
  cycleInterval = 20000;
  maxBufferSize = 1000000;

  isRunning = true;
}

void MeasFetcher::start() {
  ioProxy->prepareSocket();
  logArray->log("MeasFetcher", "start");

  ready = true;

  while(isRunning) {
    for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
      shutdownMutex.lock();
      m->fetch();
      shutdownMutex.unlock();

      Helper::longSleep(betweenMeasInterval);

      // if there is more than 2 measurements after fetching
      // other utils (overseer, storage, ...) can be started
      if ( (measTypeArray->isReady == false) && (m->buffer->count > 2) ) {
        measTypeArray->isReady = true;
      }
    }

    Helper::longSleep(cycleInterval);
  }
}

void MeasFetcher::stop() {
  shutdownMutex.lock();
  logArray->log("MeasFetcher", "stop");
}
