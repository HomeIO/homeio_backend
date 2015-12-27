#ifndef ABSTRACT_ADDON_HPP
#define	ABSTRACT_ADDON_HPP

#include <string>
#include "../meas_type_array.hpp"

using namespace std;

class AbstractAddon {
public:
  virtual void setup();
  virtual void perform();

  MeasTypeArray *measTypeArray;
  string name;
};

#include "abstract_addon.cpp"
#endif
