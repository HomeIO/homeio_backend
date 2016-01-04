#ifndef MEAS_FETCHER
#define	MEAS_FETCHER

#include <mutex>

#include "meas_type_array.hpp"
#include "io/io_proxy.hpp"

class MeasFetcher {
public:
  MeasFetcher();
  void start();
  void stop();

  MeasTypeArray *measTypeArray;
  IoProxy *ioProxy;

  unsigned long int betweenMeasInterval; // 5000
  unsigned long int cycleInterval; // 20000
  unsigned long int maxBufferSize; // 1000000

  bool isRunning;
  std::mutex shutdownMutex;
};

//#include "meas_fetcher.cpp"
#endif
