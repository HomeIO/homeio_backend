#include "wind_turbine_super_overseer.hpp"

WindTurbineSuperOverseer::WindTurbineSuperOverseer() {
  name = "wind_turbine_super_overseer";
}

void WindTurbineSuperOverseer::setup() {
  outputsMeas = measTypeArray->byName("outputs");
  resPwmAvgMeas = measTypeArray->byName("res_pwm_avg");

  turnOnInverterAOverseer1 = overseerArray->byName("turn_on_inverter_A_on_high_coil");
  turnOnInverterAOverseer2 = overseerArray->byName("turn_on_inverter_A_on_high_battery");

  turnOffInverterAOverseer1 = overseerArray->byName("turn_off_inverter_A_on_low_coil");
  turnOffInverterAOverseer2 = overseerArray->byName("turn_off_inverter_A_on_low_battery");

  turnOnInverterBOverseer1 = overseerArray->byName("turn_on_inverter_B_on_high_battery");

  turnOffInverterBOverseer1 = overseerArray->byName("turn_off_inverter_B_on_low_coil");
  turnOffInverterBOverseer2 = overseerArray->byName("turn_off_inverter_B_on_low_battery");

  turnOnBrake = overseerArray->byName("brake_on_high_batt_voltage");
  turnOffBrake = overseerArray->byName("no_brake_on_low_batt_voltage");
}

bool WindTurbineSuperOverseer::isInterterAOn() {
  if (outputsMeas->lastRaw() & 2 == 2) {
    return true;
  } else {
    return false;
  }
}

bool WindTurbineSuperOverseer::isInterterBOn() {
  if (outputsMeas->lastRaw() & 4 == 4) {
    return true;
  } else {
    return false;
  }
}

bool WindTurbineSuperOverseer::isBreaking() {
  if (resPwmAvgMeas->lastValue() > 40.0) {
    return true;
  } else {
    return false;
  }
}


void WindTurbineSuperOverseer::perform() {
  if (isInterterAOn()) {
    turnOnInverterAOverseer1->enabled = false;
    turnOnInverterAOverseer2->enabled = false;

    turnOffInverterAOverseer1->enabled = true;
    turnOffInverterAOverseer2->enabled = true;
  } else {
    turnOnInverterAOverseer1->enabled = true;
    turnOnInverterAOverseer2->enabled = true;

    turnOffInverterAOverseer1->enabled = false;
    turnOffInverterAOverseer2->enabled = false;
  }

  if (isInterterBOn()) {
    turnOnInverterBOverseer1->enabled = false;

    turnOffInverterBOverseer1->enabled = true;
    turnOffInverterBOverseer2->enabled = true;
  } else {
    turnOnInverterBOverseer1->enabled = true;

    turnOffInverterBOverseer1->enabled = false;
    turnOffInverterBOverseer2->enabled = false;
  }

  if (isBreaking()) {
    turnOnBrake->enabled = false;
    turnOffBrake->enabled = true;
    turnOffInverterBOverseer2->enabled = true;
  } else {
    turnOnBrake->enabled = true;
    turnOffBrake->enabled = false;
  }
}

void WindTurbineSuperOverseer::stop() {
}

void WindTurbineSuperOverseer::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, ("Wind Turbine Super Overseer"));
  wattroff(window, NC_COLOR_PAIR_NAME_SET);

  // time
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, 3, 1, "Inverter A" );
  mvwprintw(window, 4, 1, "Inverter B" );
  mvwprintw(window, 5, 1, "Breaking" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, 3, 20, std::to_string(isInterterAOn()).c_str() );
  mvwprintw(window, 4, 20, std::to_string(isInterterBOn()).c_str() );
  mvwprintw(window, 5, 20, std::to_string(isBreaking()).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);
}


std::string WindTurbineSuperOverseer::toJson() {
  std::string json = "{\"hash\": {\"inwerterA\": " + std::to_string(isInterterAOn()) + ", \"inwerterB\": " + std::to_string(isInterterBOn()) + ", \"breaking\": " + std::to_string(isBreaking()) + "}";
  json += ", \"name\": \"" + name + "\"}";
  return json;
}
