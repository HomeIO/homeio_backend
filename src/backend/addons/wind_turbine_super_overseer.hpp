#ifndef WIND_TURBINE_SUPER_OVERSEER_HPP
#define	WIND_TURBINE_SUPER_OVERSEER_HPP

#include <string>
#include <vector>
#include <memory>

#include "abstract_addon.hpp"
#include "../utils/helper.hpp"
#include "../meas/meas_definitions.hpp"

class WindTurbineSuperOverseer : public AbstractAddon {
 public:
  WindTurbineSuperOverseer();
  ~WindTurbineSuperOverseer();

  void setup();
  void perform();
  void render();
  void stop();
  std::string toJson();

  std::shared_ptr<MeasType> outputsMeas;
  std::shared_ptr<MeasType> resPwmAvgMeas;

  std::shared_ptr<Overseer> turnOnInverterAOverseer1;
  std::shared_ptr<Overseer> turnOnInverterAOverseer2;
  std::shared_ptr<Overseer> turnOffInverterAOverseer1;
  std::shared_ptr<Overseer> turnOffInverterAOverseer2;
  std::shared_ptr<Overseer> turnOnInverterBOverseer1;
  std::shared_ptr<Overseer> turnOffInverterBOverseer1;
  std::shared_ptr<Overseer> turnOffInverterBOverseer2;

  std::shared_ptr<Overseer> turnOnBrake;
  std::shared_ptr<Overseer> turnOffBrake;

  bool isInterterAOn();
  bool isInterterBOn();
  bool isBreaking();
};

#endif
