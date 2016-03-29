#ifndef MEAS_PERIODIC_STAT_HPP
#define	MEAS_PERIODIC_STAT_HPP

#include <string>
#include <sstream>
#include <cstdlib>

#include "../meas/meas_definitions.hpp"

class MeasPeriodicStat {
 public:
  MeasPeriodicStat();

  unsigned long long time;
  unsigned long long timeLength;

  meas_buffer_index count;

  double min;
  double avg;
  double max;

  std::string toCsv();
  void fromCsv(std::string s);
  std::string toJson();
} ;

#endif
