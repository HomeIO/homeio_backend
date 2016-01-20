#ifndef SPY_H
#define SPY_H

#include <string>
#include <vector>
#include <mutex>

#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"

#include <cstdlib>
#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

class Spy {
 public:
  Spy();
  void start();
  void stop();
  void announceAll();
  unsigned char annouceMeas(std::string name, double value);

  std::string hiveHost;
  std::string siteName;
  std::string urlPath;
  std::string url;

  MeasTypeArray *measTypeArray;
  LogArray *logArray;

  unsigned long int cycleInterval;
  unsigned long long lastTime;
  unsigned long long usDelay;

  bool quiet;
  bool enabled;

  bool isRunning;
  bool ready;
  std::mutex shutdownMutex;
};

//#include "spy.cpp"
#endif
