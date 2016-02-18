#ifndef MEAS_PERIODIC_STAT_HPP
#define	MEAS_PERIODIC_STAT_HPP

#include <string>
#include <sstream>
#include <cstdlib>

class MeasPeriodicStat {
 public:
  MeasPeriodicStat();

  unsigned long long time;
  unsigned long long timeLength;

  double min;
  double avg;
  double max;

  std::string toCsv();
  void fromCsv(std::string s);
  std::string toJson();
} ;

#endif
