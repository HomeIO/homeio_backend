#ifndef WIND_TURBINE_STATS_ADDON_HPP
#define	WIND_TURBINE_STATS_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>

#include "abstract_addon.hpp"
#include "wind_turbine_stat.hpp"
#include "../utils/helper.hpp"

using namespace std;

class WindTurbineStatsAddon : public AbstractAddon {
 public:
  WindTurbineStatsAddon();
  ~WindTurbineStatsAddon();

  void setup();
  void perform();
  void render();

  string measNameU;
  string measNameI;
  string measCoil;
  string measResistor;

  WindTurbineStat s, prevS;

  double coilThresholdVoltage;
  double batteryThresholdCurrent;

  static constexpr unsigned long long hour = 3600000;

  string path;
  unsigned long long lastTime;

  unsigned long long calculateTimeFrom();
  WindTurbineStat calculateStats(unsigned long long t);
  void store();
};

//#include "wind_turbine_stats_addon.cpp"
#endif
