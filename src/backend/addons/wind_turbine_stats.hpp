#ifndef WIND_TURBINE_STATS_HPP
#define	WIND_TURBINE_STATS_HPP

#include <string>
#include "abstract_addon.hpp"


using namespace std;

class WindTurbineStats : public AbstractAddon {
public:
  WindTurbineStats();
  ~WindTurbineStats();

  void start();
  void stop();
  void perform();

  string measNameU;
  string measNameI;

  static constexpr unsigned long long hour = 3600000;

  string path;
  unsigned long long lastTime;

  unsigned long long calculateTimeFrom();
  double calculatePowerForHour(unsigned long long t);
  void store(unsigned long long t, double w);
};

#include "wind_turbine_stats.cpp"
#endif
