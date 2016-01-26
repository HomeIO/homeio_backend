#include "ncurses_stats.hpp"

#define NC_STATS_START 0
#define NC_STATS_VM_USAGE NC_STATS_START + 1
#define NC_STATS_RESIDENT_SET NC_STATS_VM_USAGE + 1

#define NC_STATS_NAME 0
#define NC_STATS_VALUE NC_STATS_NAME + 30

NcursesStats::NcursesStats() {
  page = 0;
  perPage = 1;
  maxPage = 1;
}

unsigned int NcursesStats::setPage(unsigned int p) {
  //perPage = (unsigned int) (LINES - 8);
  //maxPage = (unsigned int) ( ceil( (float) (actionTypeArray->actionTypes.size()) / (float) (perPage) ) );

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  } else {
    page = p;
  }

  return page;
}

void NcursesStats::renderPage(WINDOW *w) {
  mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
}

void NcursesStats::render(WINDOW *w) {
  renderPage(w);

  double vmUsage;
  double residentSet;
  Helper::processMemUsage(vmUsage, residentSet);

  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, NC_STATS_VM_USAGE, 1 + NC_STATS_NAME, "Vm usage" );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  wattron(w, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(w, NC_STATS_VM_USAGE, 1 + NC_STATS_VALUE, std::to_string(vmUsage).c_str() );
  wattroff(w, NC_COLOR_PAIR_VALUE_SET);

  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, NC_STATS_RESIDENT_SET, 1 + NC_STATS_NAME, "Resident set" );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  wattron(w, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(w, NC_STATS_RESIDENT_SET, 1 + NC_STATS_VALUE, std::to_string(residentSet).c_str() );
  wattroff(w, NC_COLOR_PAIR_VALUE_SET);

}
