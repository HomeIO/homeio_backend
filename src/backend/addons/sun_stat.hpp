#ifndef SUN_STAT_HPP
#define	SUN_STAT_HPP

#include <string>
#include <sstream>
#include <cstdlib>

#include "../meas/meas_definitions.hpp"

class SunStat {
 public:
  SunStat();

  meas_time time;
  meas_time timeLength;

  bool foundSunriseTime2;
  meas_time sunriseTime2;

  bool foundSunriseTime8;
  meas_time sunriseTime8;

  bool foundSunTime40;
  meas_time sunTime40;
  meas_time sunInterval40;

  float sunIntegrated;

  std::string toCsv();
  void fromCsv(std::string s);
  std::string toJson();
} ;

#endif
