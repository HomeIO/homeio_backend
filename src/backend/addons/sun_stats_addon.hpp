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
  std::vector<std::shared_ptr<SunStat>> bufferStat;
  void addToBuffer(std::shared_ptr<SunStat> ss);

  const meas_time interval = 24*3600*1000; // how often calculate, default - daily stats;
  bool storeDailyFiles; // if true store in different file everyday
  bool storeEnabled; // if false not store stats to file

  std::string path;
  meas_time lastTime;
  meas_time calcInterval;
  meas_time lastStoreTime;

  void updateStats(std::shared_ptr<SunStat> s);
  void store(std::shared_ptr<SunStat> s);
  void repopulateFromBuffer();
};

#endif
