#ifndef WIND_TURBINE_STATS_HPP
#define	WIND_TURBINE_STATS_HPP

#include <string>
#include <sstream>
#include <cstdlib>

class WindTurbineStat {
 public:
  WindTurbineStat();

  unsigned long long time;
  unsigned long long timeLength;
  double work;

  unsigned long int coilTime;
  unsigned long int battCurrentTime;
  unsigned long int resistorTime;

  double maxBattCurrent;
  double maxCoilVoltage;
  double maxBattVoltage;

  std::string toCsv();
  void fromCsv(std::string s);
  std::string toJson();
} ;

//#include "wind_turbine_stat.cpp"
#endif
