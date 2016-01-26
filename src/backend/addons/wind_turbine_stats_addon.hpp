#ifndef WIND_TURBINE_STATS_ADDON_HPP
#define	WIND_TURBINE_STATS_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>

#include "abstract_addon.hpp"
#include "wind_turbine_stat.hpp"
#include "../utils/helper.hpp"

class WindTurbineStatsAddon : public AbstractAddon {
 public:
  WindTurbineStatsAddon();
  ~WindTurbineStatsAddon();

  void setup();
  void perform();
  void render();

  std::string measNameU;
  std::string measNameI;
  std::string measCoil;
  std::string measResistor;

  WindTurbineStat s, prevS;

  double coilThresholdVoltage;
  double batteryThresholdCurrent;

  static constexpr unsigned long long hour = 3600000;

  std::string path;
  unsigned long long lastTime;

  unsigned long long calculateTimeFrom();
  WindTurbineStat calculateStats(unsigned long long t);
  void store();
};

#endif
