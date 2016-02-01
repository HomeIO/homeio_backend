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
    shutdownMutex.lock();

    for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
      std::shared_ptr<MeasType> m = *it;
      m->fetch();

      Helper::longSleep(betweenMeasInterval);

      // if there is more than 2 measurements after fetching
      // other utils (overseer, storage, ...) can be started
      if ( (measTypeArray->isReady == false) && (m->buffer->count > 2) ) {
        measTypeArray->isReady = true;
      }
    }
    shutdownMutex.unlock();

    Helper::longSleep(cycleInterval);
  }
}

void MeasFetcher::stop() {
  isRunning = false;
  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("MeasFetcher", "stop");
}
