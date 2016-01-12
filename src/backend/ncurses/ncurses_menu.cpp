#include "ncurses_menu.hpp"

NcursesMenu::NcursesMenu() {
  cursor = 0;
}

void NcursesMenu::start() {
  //initscr();
  //menuWindow = newwin(10, 20, 0, 0);
  //box(menuWindow, 0 , 0);
  //wrefresh(menuWindow);
}

void NcursesMenu::stop() {
  //wrefresh(window);
  //delwin(window);
}
