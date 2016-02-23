#ifndef FACTORY_MEAS_PERIODIC_HPP
#define	FACTORY_MEAS_PERIODIC_HPP

#include <string>
#include <vector>

#include "../meas/meas_type_array.hpp"
#include "../meas/meas_definitions.hpp"
#include "addons_array.hpp"
#include "meas_periodic_stats_addon.hpp"

class FactoryMeasPeriodic {
 public:
  FactoryMeasPeriodic();
  void makeItSo(std::shared_ptr<MeasTypeArray> measTypeArray, std::shared_ptr<AddonsArray> addonArray);

  std::string namePrefix;
  unsigned int bufferMax;
  meas_time calcInterval;
  meas_time interval;

};

#endif
