#ifndef PLANT_MONITOR_ADDON_HPP
#define	PLANT_MONITOR_ADDON_HPP

#include <string>
#include <sys/stat.h>
#include <fstream>

#include "abstract_addon.hpp"
#include "plant_monitor_item.hpp"
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
  std::vector <PlantMonitorItem> plantMonitorItems;

  unsigned long long lastTime;
  std::string path;
};

#endif
