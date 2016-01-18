#include "ncurses_log.hpp"

NcursesLog::NcursesLog() {
  logArray = new LogArray;
  page = 0;
}

void NcursesLog::render(WINDOW *w) {
  logArray->limit( (unsigned int) (LINES - 4) );

  mvwprintw(w, 1, 1, ("Page" + std::to_string(page)).c_str() );

  unsigned int i = 2;

  //for(std::vector<LogItem>::iterator it = logArray->logBuffer.begin(); it != logArray->logBuffer.end(); ++it) {
  for(std::vector<LogItem>::iterator it = logArray->logBuffer.end(); it != logArray->logBuffer.begin(); --it) {
    if (it->error) {
      wattron(w, NC_COLOR_PAIR_ERROR_SET);
    }
    mvwprintw(w, (int) i, 1, it->line().c_str() );
    if (it->error) {
      wattroff(w, NC_COLOR_PAIR_ERROR_SET);
    }

    i++;
  }

}
