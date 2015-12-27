#include "storage_hash.hpp"

StorageHash::StorageHash(unsigned long long tF, unsigned long long tT, double v) {
  timeFrom = tF;
  timeTo = tT;
  value = v;
}

string StorageHash::toJson() {
  string s = "{";
  s += "\"f\":" + to_string(timeFrom) + ",";
  s += "\"t\":" + to_string(timeTo) + ",";
  s += "\"v\":" + to_string(value);
  s += "}";

  return s;
}
