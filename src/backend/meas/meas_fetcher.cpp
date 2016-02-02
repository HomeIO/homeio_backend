#include "meas_fetcher.hpp"

MeasFetcher::MeasFetcher() {
  ready = false;
  changing = false;
  work = false;

  // default intervals
  betweenMeasInterval = 5000;
  cycleInterval = 20000;
  maxBufferSize = 1000000;

  isRunning = true;
}

void MeasFetcher::start() {
  changing = true;

  ioProxy->prepareSocket();
  logArray->log("MeasFetcher", "start");

  ready = true;
  changing = false;

  while(isRunning) {
    shutdownMutex.lock();
    work = true;

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

    work = false;
    shutdownMutex.unlock();

    Helper::longSleep(cycleInterval);
  }
}

void MeasFetcher::stop() {
  isRunning = false;
  changing = true;

  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("MeasFetcher", "stop");

  changing = false;
  ready = false;
}
