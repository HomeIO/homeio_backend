#ifndef NCURSES_ACTION_HPP
#define	NCURSES_ACTION_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "../action/action_type_array.hpp"
#include "../action/action_type.hpp"
#include "ncurses_colors.hpp"

class NcursesAction {
 public:
  NcursesAction();
  void render(WINDOW *w);
  void renderPage(WINDOW *w);
  void renderAction(WINDOW *w, ActionType *a, int i);
  unsigned int setPage(unsigned int p);

  unsigned int page;
  unsigned int perPage;
  unsigned int maxPage;

  ActionTypeArray *actionTypeArray;
};

#endif
