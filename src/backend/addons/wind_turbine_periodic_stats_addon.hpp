#ifndef WIND_TURBINE_DAILY_STATS_ADDON_HPP
#define	WIND_TURBINE_DAILY_STATS_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "abstract_addon.hpp"
#include "wind_turbine_stat.hpp"
#include "../utils/helper.hpp"
#include "../meas/meas_definitions.hpp"

class WindTurbinePeriodicStatsAddon : public AbstractAddon {
 public:
  WindTurbinePeriodicStatsAddon();

  void setup();
  std::string backupFilename();
  std::string storeFilename(std::shared_ptr<WindTurbineStat> s);
  void restore();
  void dump();
  meas_time normalizeTime(meas_time t);
  void perform();
  void render();
  void stop();
  std::string toJson();

  std::string measNameU;
  std::string measNameI;
  std::string measCoil;
  std::string measResistor;

  std::shared_ptr<MeasType> u;
  std::shared_ptr<MeasType> i;
  std::shared_ptr<MeasType> coil;
  std::shared_ptr<MeasType> resistor;

  unsigned int bufferMax;
  std::vector<std::shared_ptr<WindTurbineStat>> bufferStat;
  void addToBuffer(std::shared_ptr<WindTurbineStat> wts);

  double coilThresholdVoltage;
  double batteryThresholdCurrent;

  meas_time interval;
  bool storeDailyFiles; // if true store in different file everyday
  bool storeEnabled; // if false not store stats to file

  std::string path;
  meas_time lastTime;
  meas_time calcInterval;

  void updateStats(std::shared_ptr<WindTurbineStat> s);
  void store(std::shared_ptr<WindTurbineStat> s);
  void repopulateFromBuffer();
};

#endif
