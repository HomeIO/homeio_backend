#include "meas_type.h"

MeasType::MeasType(string _name) {
  name = _name;
  
  cout << "Created " << name << endl;
}

unsigned int MeasType::fetch() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);
  
  cout << name << " - fetch raw " << raw << endl;
  
  return raw;
}

int MeasType::assignBufferSize(unsigned int _size) {
  buffer.reserve(_size);
  
  cout << " size " << buffer.size();
  cout << " max_size " << buffer.max_size();
  cout << " size_of " << sizeof(buffer);
  cout << endl;
  
  return buffer.max_size();
}

int MeasType::addRaw(unsigned int _raw) {
  buffer.push_back(_raw);
  cout << " added " << _raw;
  cout << " size " << buffer.size();
  cout << " max_size " << buffer.max_size();
  
  cout << endl;
  
  return 0;
}
