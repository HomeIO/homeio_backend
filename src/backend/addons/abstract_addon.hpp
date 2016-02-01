#ifndef ABSTRACT_ADDON_HPP
#define	ABSTRACT_ADDON_HPP

#include <string>
#include <ncurses.h>

#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"
#include "../ncurses/ncurses_colors.hpp"

class AbstractAddon {
 public:
  virtual void setup();
  virtual void perform();
  virtual void render();

  std::shared_ptr<MeasTypeArray> measTypeArray;
  LogArray *logArray;

  std::string name;
  WINDOW *window;
};

//#include "abstract_addon.cpp"
#endif
