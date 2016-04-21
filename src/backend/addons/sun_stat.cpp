#include "sun_stat.hpp"

SunStat::SunStat() {
  time = 0;
  timeLength = 0;

  foundSunriseTime2 = false;
  sunriseTime2 = 0;

  foundSunriseTime8 = false;
  sunriseTime8 = 0;

  foundSunTime40 = false;
  sunTime40 = 0;
  sunInterval40 = 0;

  sunIntegrated = 0.0;
}

std::string SunStat::toCsv() {
  std::string s = "";
  s += std::to_string(time) + "; ";
  s += std::to_string(foundSunriseTime2) + "; ";
  s += std::to_string(sunriseTime2) + "; ";
  s += std::to_string(foundSunriseTime8) + "; ";
  s += std::to_string(sunriseTime8) + "; ";
  s += std::to_string(foundSunTime40) + "; ";
  s += std::to_string(sunTime40) + "; ";
  s += std::to_string(sunInterval40) + "; ";
  s += std::to_string(sunIntegrated) + "; ";

  return s;
}

void SunStat::fromCsv(std::string s) {
  std::istringstream ss(s);
  std::string token;

  std::getline(ss, token, ';');
  time = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  foundSunriseTime2 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  sunriseTime2 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  foundSunriseTime8 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  sunriseTime8 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  foundSunTime40 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  sunTime40 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  sunInterval40 = std::stoull(token.c_str());

  std::getline(ss, token, ';');
  sunIntegrated = std::atof(token.c_str());
}

std::string SunStat::toJson() {
  std::string s = "{";
  s += "\"time\": " + std::to_string(time);
  s += ", \"foundSunriseTime2\": " + std::to_string(foundSunriseTime2);
  s += ", \"sunriseTime2\": " + std::to_string(sunriseTime2);
  s += ", \"foundSunriseTime8\": " + std::to_string(foundSunriseTime8);
  s += ", \"sunriseTime8\": " + std::to_string(sunriseTime8);
  s += ", \"foundSunTime40\": " + std::to_string(foundSunTime40);
  s += ", \"sunTime40\": " + std::to_string(sunTime40);
  s += ", \"sunInterval40\": " + std::to_string(sunInterval40);
  s += ", \"sunIntegrated\": " + std::to_string(sunIntegrated);
  s += "}";
  return s;
}
