#include "meas_type_array.hpp"

MeasTypeArray::MeasTypeArray() {
  isReady = false;
  i = 0;
}

unsigned int MeasTypeArray::add(std::shared_ptr<MeasType> m) {
  m->index = i;
  m->logArray = logArray;
  m->buffer->logArray = logArray;

  measTypes.push_back(m);
  i++;
  logArray->log("Meas", "[" + m->name + "] added: (" + std::to_string(measTypes.size()) + " total meas types)");
  return 0;
}

std::shared_ptr<MeasType> MeasTypeArray::byName(std::string s) {
  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypes.begin(); it != measTypes.end(); ++it) {
    std::shared_ptr<MeasType> measType = *it;
    if (measType->name == s) {
      return measType;
    }
  }
  return NULL;
}

void MeasTypeArray::delayTillReady() {
  while(isReady == false) {
    usleep(10000);
  }
}
