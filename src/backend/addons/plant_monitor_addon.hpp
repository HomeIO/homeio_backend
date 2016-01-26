#ifndef PLANT_MONITOR_ADDON_HPP
#define	PLANT_MONITOR_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>

#include "abstract_addon.hpp"
#include "../utils/helper.hpp"

class PlantMonitorAddon : public AbstractAddon {
 public:
  PlantMonitorAddon();
  ~PlantMonitorAddon();

  void setup();
  void perform();
  void calculateStats();
  void render();

  std::vector <std::string> plantMeasNames;

  unsigned long long lastTime;
  std::string path;
};

#endif
