#include "plant_monitor_addon.hpp"

#define NC_PLANT_MONITOR_ADDON_NAME 1
#define NC_PLANT_MONITOR_ADDON_VALUE NC_PLANT_MONITOR_ADDON_NAME + 22
#define NC_PLANT_MONITOR_ADDON_LAST_WATERED NC_PLANT_MONITOR_ADDON_VALUE + 22

PlantMonitorAddon::PlantMonitorAddon() {
  name = "PlantMonitor";
  path = "stats";
}

void PlantMonitorAddon::setup() {
  // create path at start, no wait
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

  for(std::vector<std::string>::iterator it = plantMeasNames.begin(); it != plantMeasNames.end(); ++it) {
    std::shared_ptr<PlantMonitorItem> pmi = std::make_shared<PlantMonitorItem>();
    pmi->measName = *it;
    pmi->measType = measTypeArray->byName(*it);
    plantMonitorItems.push_back(pmi);
  }
}

void PlantMonitorAddon::perform() {
  // first run
  // so it won't calculate and write
  if ((lastTime == 0) || (lastTime <= (Helper::mTime() - 10*60*3600))) {
    calculateStats();
    lastTime = Helper::mTime();
  }
}

void PlantMonitorAddon::calculateStats() {
  for(std::vector<std::shared_ptr<PlantMonitorItem>>::iterator it = plantMonitorItems.begin(); it != plantMonitorItems.end(); ++it) {
    std::shared_ptr<PlantMonitorItem> pmi = (*it);
    pmi->process();

    logArray->log("PlantMonitorAddon", "finished process " + pmi-> measName + ":" + std::to_string(pmi->preWater) + "," + std::to_string(pmi->postWater));
  }

  logArray->log("PlantMonitorAddon", "finished calculateStats()");
}

void PlantMonitorAddon::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, ("Plant Monitor: " + std::to_string(plantMonitorItems.size()) + " plants").c_str() );
  wattroff(window, NC_COLOR_PAIR_NAME_SET);

  int i = 3;

  for(std::vector<std::shared_ptr<PlantMonitorItem>>::iterator it = plantMonitorItems.begin(); it != plantMonitorItems.end(); ++it) {
    std::shared_ptr<PlantMonitorItem> pmi = (*it);
    wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
    mvwprintw(window, i, NC_PLANT_MONITOR_ADDON_NAME, pmi->measName.c_str() );
    mvwprintw(window, i, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->lastFormattedValue().c_str() );
    mvwprintw(window, i, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->msWateredAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

    i++;
  }
}
