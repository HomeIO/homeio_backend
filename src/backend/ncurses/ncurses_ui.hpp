#ifndef NCURSES_UI_HPP
#define	NCURSES_UI_HPP

#include <ncurses.h>
#include <menu.h>
#include <iostream>
#include <string>
#include <memory>

#include "ncurses_meas.hpp"
#include "ncurses_home.hpp"
#include "ncurses_log.hpp"
#include "ncurses_overseer.hpp"
#include "ncurses_action.hpp"
#include "ncurses_addon.hpp"
#include "ncurses_stats.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

class NcursesUI {
 public:
  NcursesUI();
  void start();
  void stop();
  void clearContent();
  void redrawWindow();

  unsigned int page;
  int interval; // int because of ncurses

  bool isRunning;
  bool ready;

  std::unique_ptr<NcursesHome> home;
  std::unique_ptr<NcursesMeas> meas;
  std::unique_ptr<NcursesLog> log;
  std::unique_ptr<NcursesOverseer> overseer;
  std::unique_ptr<NcursesAction> action;
  std::unique_ptr<NcursesAddon> addon;
  std::unique_ptr<NcursesStats> stats;

  WINDOW *local_win;

  ITEM **my_items;
  int c;
  MENU *menu;
  int n_choices;
  ITEM *cur_item;
};

#endif
