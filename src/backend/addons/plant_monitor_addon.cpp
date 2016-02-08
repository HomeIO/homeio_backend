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
    pmi->logArray = logArray;
    plantMonitorItems.push_back(pmi);
  }
}

void PlantMonitorAddon::perform() {
  // first run
  // so it won't calculate and write
  if ((lastTime == 0) || (lastTime <= (Helper::mTime() - PLANT_MONITOR_ADDON_INTERVAL))) {
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

  int i = 0;

  for(std::vector<std::shared_ptr<PlantMonitorItem>>::iterator it = plantMonitorItems.begin(); it != plantMonitorItems.end(); ++it) {
    std::shared_ptr<PlantMonitorItem> pmi = (*it);

    // name and last watered
    wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
    mvwprintw(window, 3 + 5*i, NC_PLANT_MONITOR_ADDON_NAME, pmi->measName.c_str() );
    wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(window, 3 + 5*i, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->lastFormattedValue().c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(window, 3 + 5*i, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->msWateredAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_TEXT_SET);

    // max value
    wattron(window, NC_COLOR_PAIR_LESSER_SET);
    mvwprintw(window, 3 + 5*i + 1, NC_PLANT_MONITOR_ADDON_NAME, "max");
    wattroff(window, NC_COLOR_PAIR_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(window, 3 + 5*i + 1, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->valueToFormatted(pmi->maxValue).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(window, 3 + 5*i + 1, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->maxValueAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_TEXT_SET);

    // min value
    wattron(window, NC_COLOR_PAIR_LESSER_SET);
    mvwprintw(window, 3 + 5*i + 2, NC_PLANT_MONITOR_ADDON_NAME, "min");
    wattroff(window, NC_COLOR_PAIR_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(window, 3 + 5*i + 2, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->valueToFormatted(pmi->minValue).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(window, 3 + 5*i + 2, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->minValueAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_TEXT_SET);

    // dry speed
    wattron(window, NC_COLOR_PAIR_LESSER_SET);
    mvwprintw(window, 3 + 5*i + 3, NC_PLANT_MONITOR_ADDON_NAME, "dry speed");
    wattroff(window, NC_COLOR_PAIR_LESSER_SET);

    if (pmi->drySpeed > 0.0) {
      meas_time speedInterval = (meas_time) (1000.0 / pmi->drySpeed);
      wattron(window, NC_COLOR_PAIR_SYMBOL_SET);
      mvwprintw(window, 3 + 5*i + 3, NC_PLANT_MONITOR_ADDON_VALUE, Helper::intervalToString( speedInterval ).c_str() );
      wattroff(window, NC_COLOR_PAIR_SYMBOL_SET);
    } else {
      wattron(window, NC_COLOR_PAIR_LESSER_SET);
      mvwprintw(window, 3 + 5*i + 3, NC_PLANT_MONITOR_ADDON_VALUE, "---" );
      wattroff(window, NC_COLOR_PAIR_LESSER_SET);
    }

    i++;
  }
}
