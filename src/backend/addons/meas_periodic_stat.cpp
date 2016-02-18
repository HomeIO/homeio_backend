#include "meas_periodic_stat.hpp"

MeasPeriodicStat::MeasPeriodicStat() {
  time = 0;
  timeLength = 0;

  min = 0.0;
  avg = 0.0;
  max = 0.0;
}

std::string MeasPeriodicStat::toCsv() {
  std::string s = "";
  s += std::to_string(time) + "; ";
  s += std::to_string(min) + "; ";
  s += std::to_string(avg) + "; ";
  s += std::to_string(max) + "; ";

  return s;
}

void MeasPeriodicStat::fromCsv(std::string s) {
  std::istringstream ss(s);
  std::string token;

  std::getline(ss, token, ';');
  time = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  min = std::atof(token.c_str());

  std::getline(ss, token, ';');
  avg = std::atof(token.c_str());

  std::getline(ss, token, ';');
  max = std::atof(token.c_str());
}

std::string MeasPeriodicStat::toJson() {
  std::string s = "{";
  s += "\"time\": " + std::to_string(time);
  s += ", \"min\": " + std::to_string(min);
  s += ", \"avg\": " + std::to_string(avg);
  s += ", \"max\": " + std::to_string(max);
  s += "}";
  return s;
}
