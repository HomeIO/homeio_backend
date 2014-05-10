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
};

#include "meas_fetcher.cpp"
#endif
