#ifndef SUN_STAT_HPP
#define	SUN_STAT_HPP

#include <string>
#include <sstream>
#include <cstdlib>

#include "../meas/meas_definitions.hpp"

class SunStat {
 public:
  SunStat();

  unsigned long long time;
  unsigned long long timeLength;

  bool foundSunriseTime2;
  unsigned long long sunriseTime2;

  bool foundSunriseTime8;
  unsigned long long sunriseTime8;

  bool foundSunTime40;
  unsigned long long sunTime40;


  meas_buffer_index count;

  double min;
  double avg;
  double max;

  std::string toCsv();
  void fromCsv(std::string s);
  std::string toJson();
} ;

#endif
