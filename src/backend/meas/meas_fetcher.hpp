#ifndef MEAS_FETCHER
#define	MEAS_FETCHER

#include <mutex>

#include "meas_type_array.hpp"
#include "../io/io_proxy.hpp"
#include "../log/log_array.hpp"

class MeasFetcher {
 public:
  MeasFetcher();
  void start();
  void stop();

  std::shared_ptr<MeasTypeArray> measTypeArray;
  
  IoProxy *ioProxy;
  LogArray *logArray;

  unsigned long int betweenMeasInterval; // 5000
  unsigned long int cycleInterval; // 20000
  unsigned long int maxBufferSize; // 1000000

  bool ready;
  bool isRunning; // TODO remove id
  std::mutex shutdownMutex;
};

//#include "meas_fetcher.cpp"
#endif
