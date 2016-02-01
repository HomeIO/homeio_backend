#ifndef MEAS_TYPE_ARRAY
#define	MEAS_TYPE_ARRAY

#include <iostream>
#include <vector>
#include <memory>

#include "meas_type.hpp"
#include "../io/io_proxy.hpp"
#include "../log/log_array.hpp"

class MeasTypeArray {
 public:
  MeasTypeArray();
  unsigned int add(std::shared_ptr<MeasType>);
  std::shared_ptr<MeasType> byName(std::string s);

  std::vector <std::shared_ptr<MeasType>> measTypes;
  IoProxy *ioProxy;
  std::shared_ptr<LogArray> logArray;
  unsigned int i;

  // used by other utils like overseers, storage, to
  // delay them
  bool isReady;
  void delayTillReady();
};

//#include "meas_type_array.cpp"
#endif
