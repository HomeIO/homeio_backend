#include "meas_type_array.hpp"

MeasTypeArray::MeasTypeArray() {
  std::vector <MeasType> measTypes;
  isReady = false;
}

unsigned int MeasTypeArray::add(MeasType *m) {
  measTypes.push_back(*m);
  Helper::logInfo("MeasType added: '" + m->name + "' (" + std::to_string(measTypes.size()) + " total meas types)");
  return 0;
}

MeasType *MeasTypeArray::byName(std::string s) {
  for(std::vector<MeasType>::iterator it = measTypes.begin(); it != measTypes.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}

void MeasTypeArray::delayTillReady() {
  while(isReady == false) {
    usleep(100000);
  }
}
