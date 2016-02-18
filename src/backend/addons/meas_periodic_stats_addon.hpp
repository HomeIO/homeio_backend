#ifndef MEAS_PERIODIC_STATS_ADDON_HPP
#define	MEAS_PERIODIC_STATS_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "abstract_addon.hpp"
#include "meas_periodic_stat.hpp"
#include "../utils/helper.hpp"
#include "../meas/meas_definitions.hpp"

class MeasPeriodicStatsAddon : public AbstractAddon {
 public:
  MeasPeriodicStatsAddon();

  void setup();
  std::string backupFilename();
  std::string storeFilename(std::shared_ptr<MeasPeriodicStat> s);
  void restore();
  void dump();
  meas_time normalizeTime(meas_time t);
  void perform();
  void render();
  void stop();
  std::string toJson();

  std::string measName;
  std::shared_ptr<MeasType> meas;

  unsigned int bufferMax;
  std::vector<std::shared_ptr<MeasPeriodicStat>> bufferStat;
  void addToBuffer(std::shared_ptr<MeasPeriodicStat> wts);

  meas_time interval;
  bool storeDailyFiles; // if true store in different file everyday
  bool storeEnabled; // if false not store stats to file

  std::string path;
  meas_time lastTime;
  meas_time calcInterval;
  meas_time lastStoreTime;

  void updateStats(std::shared_ptr<MeasPeriodicStat> s);
  void store(std::shared_ptr<MeasPeriodicStat> s);
  void repopulateFromBuffer();
};

#endif
