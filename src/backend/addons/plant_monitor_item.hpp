#ifndef PLANT_MONITOR_ITEM_HPP
#define	PLANT_MONITOR_ITEM_HPP

#include <string>
#include <memory>

#include "../utils/helper.hpp"
#include "../meas/meas_type.hpp"
#include "../log/log_array.hpp"


class PlantMonitorItem {
 public:
  PlantMonitorItem();
  std::string measName;
  std::shared_ptr<MeasType> measType;

  void process();

  unsigned long long msWateredAgo;
  double preWater, postWater;

  std::shared_ptr<LogArray> logArray;
};

#endif
