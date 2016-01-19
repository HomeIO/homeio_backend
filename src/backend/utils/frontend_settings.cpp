#include "frontend_settings.hpp"

FrontendSettings::FrontendSettings() {
  // 0 means not set
  intervalCurrent = 0;
  intervalHistory = 0;

  // some systems are fast (wind turbine) and some slow (home air temperature or soil moisture)
  // set lower than 1.0 for fast systems - it will multiply timeRange using this coefficient
  // set higher than 1.0 for slow systems
  currentCoeff = 1.0;
}
