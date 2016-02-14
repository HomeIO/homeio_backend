#include "wind_turbine_stat.hpp"

WindTurbineStat::WindTurbineStat() {
  time = 0;
  timeLength = 0;

  work = 0.0;

  coilTime = 0;
  battCurrentTime = 0;
  resistorTime = 0;

  maxBattCurrent = 0.0;
  maxCoilVoltage = 0.0;
  maxBattVoltage = 0.0;
}

std::string WindTurbineStat::toCsv() {
  std::string s = "";
  s += std::to_string(time) + "; ";
  s += std::to_string(work) + "; ";
  s += std::to_string(timeLength) + "; ";
  s += std::to_string(coilTime) + "; ";
  s += std::to_string(battCurrentTime) + "; ";
  s += std::to_string(resistorTime) + "; ";
  s += std::to_string(maxBattCurrent) + "; ";
  s += std::to_string(maxBattVoltage) + "; ";
  s += std::to_string(maxCoilVoltage) + "; ";

  return s;
}

void WindTurbineStat::fromCsv(std::string s) {
  std::istringstream ss(s);
  std::string token;

  std::getline(ss, token, ';');
  time = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  work = std::atof(token.c_str());

  std::getline(ss, token, ';');
  timeLength = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  coilTime = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  battCurrentTime = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  resistorTime = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  maxBattCurrent = std::atof(token.c_str());

  std::getline(ss, token, ';');
  maxBattVoltage = std::atof(token.c_str());

  std::getline(ss, token, ';');
  maxCoilVoltage = std::atof(token.c_str());
}

std::string WindTurbineStat::toJson() {
  std::string s = "{";
  s += "\"time\": " + std::to_string(time);
  s += ", \"work\": " + std::to_string(work);
  s += ", \"timeLength\": " + std::to_string(timeLength);
  s += ", \"coilTime\": " + std::to_string(coilTime);
  s += ", \"battCurrentTime\": " + std::to_string(battCurrentTime);
  s += ", \"resistorTime\": " + std::to_string(resistorTime);
  s += ", \"maxBattCurrent\": " + std::to_string(maxBattCurrent);
  s += ", \"maxBattVoltage\": " + std::to_string(maxBattVoltage);
  s += ", \"maxCoilVoltage\": " + std::to_string(maxCoilVoltage);
  s += "}";
  return s;
}
