#ifndef PLANT_MONITOR_ITEM_HPP
#define	PLANT_MONITOR_ITEM_HPP

#include <string>
#include <memory>

#include "../utils/helper.hpp"
#include "../meas/meas_type.hpp"
#include "../log/log_array.hpp"
#include "../meas/meas_definitions.hpp"

class PlantMonitorItem {
 public:
  PlantMonitorItem();
  std::string measName;
  std::shared_ptr<MeasType> measType;

  void process();
  bool wasWateredNow(double oldValue, double newValue);

  unsigned long long msWateredAgo;
  double preWater, postWater;
  double drySpeed;
  meas_time waterNeededIn;
  double waterAtValue;

  meas_value minValue;
  meas_time minValueAgo;

  meas_value maxValue;
  meas_time maxValueAgo;

  std::shared_ptr<LogArray> logArray;
};

#endif
