#ifndef WIND_TURBINE_STATS_HPP
#define	WIND_TURBINE_STATS_HPP

#include <string>
#include "abstract_addon.hpp"

using namespace std;

class WindTurbineStats : public AbstractAddon {
public:
  WindTurbineStats();
  ~WindTurbineStats();

  void start();
  void stop();
  void perform();

  string measNameU;
  string measNameI;
};

#include "wind_turbine_stats.cpp"
#endif
