#include "meas_type_array.hpp"

MeasTypeArray::MeasTypeArray() {
  isReady = false;
  i = 0;
}

unsigned int MeasTypeArray::add(MeasType *m) {
  m->index = i;
  m->logArray = logArray;
  m->buffer->logArray = logArray;

  measTypes.push_back(*m);
  i++;
  logArray->log("Meas", "[" + m->name + "] added: (" + std::to_string(measTypes.size()) + " total meas types)");
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
