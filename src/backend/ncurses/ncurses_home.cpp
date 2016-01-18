#include "ncurses_home.hpp"

NcursesHome::NcursesHome() {
}

void NcursesHome::render(WINDOW *w) {
  mvwprintw(w, 1, 1, "Hello, current time is:" );
  mvwprintw(w, 2, 1, Helper::detailCurrentTime().c_str() );
}
