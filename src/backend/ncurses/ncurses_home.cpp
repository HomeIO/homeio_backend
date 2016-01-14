#include "ncurses_home.hpp"

NcursesHome::NcursesHome() {
  logArray = new LogArray;
}

void NcursesHome::render(WINDOW *w) {
  logArray->limit( (unsigned int) (LINES - 4) );

  unsigned int i = 3 + (unsigned int) logArray->logBuffer.size();

  mvwprintw(w, 1, 1, "Hello, current time is:" );
  mvwprintw(w, 2, 1, Helper::detailCurrentTime().c_str() );


  for(std::vector<LogItem>::iterator it = logArray->logBuffer.begin(); it != logArray->logBuffer.end(); ++it) {
    if (it->error) {
      wattron(w, NC_COLOR_PAIR_ERROR_SET);
    }
    mvwprintw(w, (int) i, 1, it->line().c_str() );
    if (it->error) {
      wattroff(w, NC_COLOR_PAIR_ERROR_SET);
    }

    i--;
  }

}
