#include "ncurses_meas.hpp"

NcursesMeas::NcursesMeas() {
}

WINDOW *NcursesMeas::render(WINDOW *w) {
  //mvwprintw(w, 10, 10, std::to_string(1).c_str() );
  //mvwprintw(w, 11, 11, "a" );

  return w;
}
