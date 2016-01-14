#include "storage_hash.hpp"

StorageHash::StorageHash(unsigned long long tF, unsigned long long tT, double v) {
  timeFrom = tF;
  timeTo = tT;
  value = v;
}

std::string StorageHash::toJson() {
  std::string s = "{";
  s += "\"f\":" + std::to_string(timeFrom) + ",";
  s += "\"t\":" + std::to_string(timeTo) + ",";
  s += "\"v\":" + std::to_string(value);
  s += "}";

  return s;
}
