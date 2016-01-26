#include "plant_monitor_addon.hpp"

#define NC_PLANT_MONITOR_ADDON_NAME 0

PlantMonitorAddon::PlantMonitorAddon() {
  name = "PlantMonitor";
  path = "stats";
}

void PlantMonitorAddon::setup() {
  // create path at start, no wait
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
}

void PlantMonitorAddon::perform() {
  // first run
  // so it won't calculate and write
  if ((lastTime == 0) || (lastTime <= (Helper::mTime() - 10*60*3600))){
    calculateStats();
    lastTime = Helper::mTime();
  }
}

void PlantMonitorAddon::calculateStats() {
}

void PlantMonitorAddon::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, "Plant Monitor" );
  wattroff(window, NC_COLOR_PAIR_NAME_SET);

  int i = 3;
}
