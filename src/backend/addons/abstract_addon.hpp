#ifndef ABSTRACT_ADDON_HPP
#define	ABSTRACT_ADDON_HPP

#include <string>
#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"

class AbstractAddon {
 public:
  virtual void setup();
  virtual void perform();

  MeasTypeArray *measTypeArray;
  LogArray *logArray;
  std::string name;
};

//#include "abstract_addon.cpp"
#endif
