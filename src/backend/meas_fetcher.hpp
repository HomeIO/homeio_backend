#ifndef MEAS_FETCHER
#define	MEAS_FETCHER

#include "meas_type_array.hpp"
#include "io_proxy.hpp"

using namespace std;

class MeasFetcher {
public:
  MeasFetcher();
  void start();
  
  MeasTypeArray *measTypeArray;
  IoProxy *ioProxy;
  
  unsigned long int betweenMeasInterval; // 5000
  unsigned long int cycleInterval; // 20000
  unsigned long int maxBufferSize; // 1000000
  
};

#include "meas_fetcher.cpp"
#endif
