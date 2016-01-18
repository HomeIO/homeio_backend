#include "ncurses_log.hpp"

NcursesLog::NcursesLog() {
  logArray = new LogArray;
  page = 0;
  perPage = 1;
}

void NcursesLog::render(WINDOW *w) {
  perPage = (unsigned int) (LINES - 5);
  unsigned int maxPage = (unsigned int) ( ceil( (float) (logArray->logBuffer.size()) / (float) (perPage) ) );

  // disable clipping
  if (page >= maxPage) {
    page = maxPage - 1;
  }

  unsigned int i;
  long unsigned int j;

  mvwprintw(w, 1, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());

  for (i = 0; i < perPage; i++) {
    j = logArray->logBuffer.size() - 1 - (perPage * page) - i;
    if (j < logArray->logBuffer.size()) {
      LogItem li = logArray->logBuffer.at(j) ;

      if (li.error) {
        wattron(w, NC_COLOR_PAIR_ERROR_SET);
      }
      mvwprintw(w, (int) i + 3, 1, li.line().c_str() );
      if (li.error) {
        wattroff(w, NC_COLOR_PAIR_ERROR_SET);
      }

    }
  }



  /*
  unsigned int i = 2;

  //for(std::vector<LogItem>::iterator it = logArray->logBuffer.begin(); it != logArray->logBuffer.end(); ++it) {
  for(std::vector<LogItem>::iterator it = logArray->logBuffer.end(); it != logArray->logBuffer.begin(); --it) {

    i++;
  }
  */

}
