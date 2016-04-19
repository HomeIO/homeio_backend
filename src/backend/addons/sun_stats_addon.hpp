#ifndef SUN_STATS_ADDON_HPP
#define	SUN_STATS_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "abstract_addon.hpp"
#include "sun_stat.hpp"
#include "../meas/meas_type.hpp"

class SunStatsAddon : public AbstractAddon {
 public:
  SunStatsAddon();
  void setup();
  std::string backupFilename();
  std::string storeFilename(std::shared_ptr<SunStat> s);
  void restore();
  void dump();
  meas_time normalizeTime(meas_time t);
  void perform();
  void render();
  void stop();
  std::string toJson();

  std::string name;

  std::string lightMeasName;
  std::shared_ptr<MeasType> lightMeas;

  unsigned int bufferMax;

  meas_time interval;
  bool storeDailyFiles; // if true store in different file everyday
  bool storeEnabled; // if false not store stats to file

  std::string path;
  meas_time lastTime;
  meas_time calcInterval;
  meas_time lastStoreTime;

};

#endif
