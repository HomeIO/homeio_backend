#include "factory_meas_periodic.hpp"

FactoryMeasPeriodic::FactoryMeasPeriodic() {
  namePrefix = "stats_";
  bufferMax = 365;
  calcInterval = 5*60*1000;
  interval = 24*60*60*1000;
}

void FactoryMeasPeriodic::makeItSo(std::shared_ptr<MeasTypeArray> measTypeArray, std::shared_ptr<AddonsArray> addonsArray) {
  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> measType = *it;

    std::unique_ptr<MeasPeriodicStatsAddon> mps = std::make_unique<MeasPeriodicStatsAddon>();

    mps->name = namePrefix + measType->name;
    mps->bufferMax = bufferMax;
    mps->calcInterval = calcInterval;
    mps->interval = interval;
    mps->measName = measType->name;

    addonsArray->addons.push_back(std::move(mps));

  }
}
