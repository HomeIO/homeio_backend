#include "ncurses_home.hpp"

NcursesHome::NcursesHome() {
  maxHistory = 40;
}

void NcursesHome::render(WINDOW *w) {
  // TODO maybe move it somewhere else
  if (maxHistory > (unsigned int)(LINES - 4)) {
    maxHistory = (unsigned int)(LINES - 4);
  }

  unsigned int i = 3 + (unsigned int) logBuffer.size();

  mvwprintw(w, 1, 1, "Hello, current time is:" );
  mvwprintw(w, 2, 1, Helper::detailCurrentTime().c_str() );

  for(std::vector<LogItem>::iterator it = logBuffer.begin(); it != logBuffer.end(); ++it) {
    if (it->error) {
      wattron(w, NC_COLOR_PAIR_ERROR_SET);
    }
    mvwprintw(w, i, 1, it->line().c_str() );
    if (it->error) {
      wattroff(w, NC_COLOR_PAIR_ERROR_SET);
    }

    i--;
  }
}

void NcursesHome::log(std::string c) {
  LogItem *li = new LogItem(c);
  addLogItem(*li);
}

void NcursesHome::logError(std::string c) {
  LogItem *li = new LogItem(c);
  li->error = true;
  addLogItem(*li);
}

void NcursesHome::addLogItem(LogItem li) {
  if (logBuffer.size() >= maxHistory) {
    logBuffer.erase( logBuffer.begin() );
  }

  logBuffer.push_back(li);
}
