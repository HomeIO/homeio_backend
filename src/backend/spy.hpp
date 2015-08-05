#ifndef SPY_H
#define SPY_H

#include <string>
#include <vector>
#include "meas_type_array.hpp"

#include <cstdlib>
#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

using namespace std;

class Spy {
public:
  Spy();
  void start();
  void announceAll();
  unsigned char annouceMeas(string name, double value);
  
  string hiveHost;
  string siteName;
  string urlPath;
  string url;
  
  MeasTypeArray *measTypeArray;

  unsigned long int cycleInterval;
  unsigned long long lastTime;
  unsigned long long usDelay;
  
  bool quiet;
  bool enabled;
};

#include "spy.cpp"
#endif

