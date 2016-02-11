#include "plant_monitor_addon.hpp"

#define NC_PLANT_MONITOR_ADDON_NAME 1
#define NC_PLANT_MONITOR_ADDON_VALUE NC_PLANT_MONITOR_ADDON_NAME + 23
#define NC_PLANT_MONITOR_ADDON_LAST_WATERED NC_PLANT_MONITOR_ADDON_VALUE + 23

#define NC_PLANT_PER_ITEM 5

PlantMonitorAddon::PlantMonitorAddon() {
  name = "plant_monitor";
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
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i, NC_PLANT_MONITOR_ADDON_NAME, pmi->measName.c_str() );
    wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->lastFormattedValue().c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->msWateredAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_TEXT_SET);

    // max value
    wattron(window, NC_COLOR_PAIR_LESSER_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 1, NC_PLANT_MONITOR_ADDON_NAME, "max");
    wattroff(window, NC_COLOR_PAIR_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 1, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->valueToFormatted(pmi->maxValue).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 1, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->maxValueAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_TEXT_SET);

    // min value
    wattron(window, NC_COLOR_PAIR_LESSER_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 2, NC_PLANT_MONITOR_ADDON_NAME, "min");
    wattroff(window, NC_COLOR_PAIR_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 2, NC_PLANT_MONITOR_ADDON_VALUE, pmi->measType->valueToFormatted(pmi->minValue).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

    wattron(window, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 2, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString(pmi->minValueAgo).c_str() );
    wattroff(window, NC_COLOR_PAIR_TEXT_SET);

    // dry speed
    wattron(window, NC_COLOR_PAIR_LESSER_SET);
    mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 3, NC_PLANT_MONITOR_ADDON_NAME, "dry speed");
    wattroff(window, NC_COLOR_PAIR_LESSER_SET);

    if (pmi->drySpeed > 0.0) {
      meas_time speedInterval = (meas_time) (1000.0 / pmi->drySpeed);
      wattron(window, NC_COLOR_PAIR_SYMBOL_SET);
      mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 3, NC_PLANT_MONITOR_ADDON_VALUE, (Helper::intervalToString( speedInterval ) + " per unit").c_str() );
      wattroff(window, NC_COLOR_PAIR_SYMBOL_SET);
    } else {
      wattron(window, NC_COLOR_PAIR_LESSER_SET);
      mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 3, NC_PLANT_MONITOR_ADDON_VALUE, "---" );
      wattroff(window, NC_COLOR_PAIR_LESSER_SET);
    }

    if (pmi->waterNeededIn > 0) {
      wattron(window, NC_COLOR_PAIR_SYMBOL_SET);
      mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 3, NC_PLANT_MONITOR_ADDON_LAST_WATERED, Helper::intervalToString( pmi->waterNeededIn ).c_str() );
      wattroff(window, NC_COLOR_PAIR_SYMBOL_SET);
    } else {
      wattron(window, NC_COLOR_PAIR_LESSER_SET);
      mvwprintw(window, 3 + NC_PLANT_PER_ITEM*i + 3, NC_PLANT_MONITOR_ADDON_LAST_WATERED, "---" );
      wattroff(window, NC_COLOR_PAIR_LESSER_SET);
    }

    i++;
  }
}


std::string PlantMonitorAddon::toJson() {
  std::string json = "{\"array\": [";
  std::string itemJson;

  for(std::vector<std::shared_ptr<PlantMonitorItem>>::iterator it = plantMonitorItems.begin(); it != plantMonitorItems.end(); ++it) {
    std::shared_ptr<PlantMonitorItem> pmi = (*it);

    itemJson = "{";
    itemJson += "\"msWateredAgo\": " + std::to_string(pmi->msWateredAgo);
    itemJson += ", \"preWater\": " + std::to_string(pmi->preWater);
    itemJson += ", \"postWater\": " + std::to_string(pmi->postWater);
    itemJson += ", \"drySpeed\": " + std::to_string(pmi->drySpeed);
    itemJson += ", \"waterNeededIn\": " + std::to_string(pmi->waterNeededIn);
    itemJson += ", \"waterAtValue\": " + std::to_string(pmi->waterAtValue);
    itemJson += ", \"minValue\": " + std::to_string(pmi->minValue);
    itemJson += ", \"minValueAgo\": " + std::to_string(pmi->minValueAgo);
    itemJson += ", \"maxValue\": " + std::to_string(pmi->maxValue);
    itemJson += ", \"maxValueAgo\": " + std::to_string(pmi->maxValueAgo);

    itemJson += "}";
    itemJson += ",";

    json += itemJson;
  }

  // remove last coma
  if (json[json.size() - 1] == ',') {
    json.resize(json.size() - 1);
  }

  std::string keyDesc = "[";
  keyDesc += "{\"key\": \"msWateredAgo\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"preWater\", \"type\": \"float\", \"unit\": \"%\"},";
  keyDesc += "{\"key\": \"postWater\", \"type\": \"float\", \"unit\": \"%\"},";
  keyDesc += "{\"key\": \"drySpeed\", \"type\": \"float\", \"unit\": \"%/s\"},";
  keyDesc += "{\"key\": \"waterNeededIn\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"waterAtValue\", \"type\": \"float\", \"unit\": \"%\"},";
  keyDesc += "{\"key\": \"minValue\", \"type\": \"float\", \"unit\": \"%\"},";
  keyDesc += "{\"key\": \"minValueAgo\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"maxValue\", \"type\": \"float\", \"unit\": \"%\"},";
  keyDesc += "{\"key\": \"maxValueAgo\", \"type\": \"interval\"}";
  keyDesc += "]";

  json += "], \"name\": \"" + name + "\", \"keys\": " + keyDesc + "}";
  return json;
}
