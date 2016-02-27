#include "ncurses_overseer.hpp"

#define NC_OVERSEER_1 0
#define NC_OVERSEER_1_NAME 0

#define NC_OVERSEER_2 1
#define NC_OVERSEER_2_MEAS_NAME 0
#define NC_OVERSEER_2_MEAS_VALUE NC_OVERSEER_2_MEAS_NAME + 22
#define NC_OVERSEER_2_MEAS_IS_MAX NC_OVERSEER_2_MEAS_VALUE + 20
#define NC_OVERSEER_2_MEAS_THRESHOLD NC_OVERSEER_2_MEAS_IS_MAX + 2
#define NC_OVERSEER_2_MEAS_RESULT NC_OVERSEER_2_MEAS_THRESHOLD + 16

#define NC_OVERSEER_3 2
#define NC_OVERSEER_3_ACTION_NAME 0
#define NC_OVERSEER_3_ACTION_SECONDS_AGO NC_OVERSEER_3_ACTION_NAME + 22
#define NC_OVERSEER_3_ACTION_HITS NC_OVERSEER_3_ACTION_SECONDS_AGO + 20 + 2

#define NC_OVERSEER_LINE_SIZE 4

NcursesOverseer::NcursesOverseer() {
  page = 0;
  perPage = 1;
  maxPage = 1;
}

bool NcursesOverseer::ready() {
  return overseerArray->ready;
}

unsigned int NcursesOverseer::setPage(unsigned int p) {
  perPage = (unsigned int) ( (LINES - 8) / NC_OVERSEER_LINE_SIZE );
  maxPage = (unsigned int) ( ceil( (float) (overseerArray->overseers.size()) / (float) (perPage) ) );

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  } else {
    page = p;
  }

  return page;
}

void NcursesOverseer::renderPage(WINDOW *w) {
  mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
}

void NcursesOverseer::render(WINDOW *w) {
  if (ready() == false) {
    mvwprintw(w, 1, 1, "Initializing..." );
    return;
  }

  renderPage(w);

  mvwprintw(w, 1, 1 + NC_OVERSEER_1_NAME, "Name" );

  unsigned int i;
  long unsigned int j;
  std::shared_ptr<Overseer> element;

  for (i = 0; i < perPage; i++) {
    j = (perPage * page) + i;
    if (j < overseerArray->overseers.size()) {
      element = overseerArray->overseers.at(j) ;
      renderOverseer(w, element, (int) ((i * NC_OVERSEER_LINE_SIZE) + 3) );
    }
  }
}

void NcursesOverseer::renderOverseer(WINDOW *w, std::shared_ptr<Overseer> o, int i) {
  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, i + NC_OVERSEER_1, 1 + NC_OVERSEER_1_NAME, o->name.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  // meas
  wattron(w, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_NAME, o->measName.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_LESSER_SET);

  std::string valueString = o->tempValueFormatted();

  wattron(w, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_VALUE, valueString.c_str() );
  wattroff(w, NC_COLOR_PAIR_VALUE_SET);

  wattron(w, NC_COLOR_PAIR_SYMBOL_SET);
  if (o->isMax) {
    mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_IS_MAX, ">" );
  } else {
    mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_IS_MAX, "<" );
  }
  wattroff(w, NC_COLOR_PAIR_SYMBOL_SET);

  valueString = o->thresholdValueFormatted();

  wattron(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_THRESHOLD, valueString.c_str() );
  wattroff(w, NC_COLOR_PAIR_VALUE_LESSER_SET);

  wattron(w, NC_COLOR_PAIR_SYMBOL_SET);
  if (o->enabled == false) {
    mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_RESULT, "DISABLED" );
  } else if (o->tempResult) {
    mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_RESULT, "HIT" );
  } else {
    mvwprintw(w, i + NC_OVERSEER_2, 1 + NC_OVERSEER_2_MEAS_RESULT, "-" );
  }
  wattroff(w, NC_COLOR_PAIR_SYMBOL_SET);

  // action
  wattron(w, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(w, i + NC_OVERSEER_3, 1 + NC_OVERSEER_3_ACTION_NAME, o->actionName.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_LESSER_SET);

  if (o->wasExecutedAtLeastOnce()) {
    wattron(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(w, i + NC_OVERSEER_3, 1 + NC_OVERSEER_3_ACTION_SECONDS_AGO, std::to_string( o->secondsTillLastExec() ).c_str() );
    wattroff(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
  } else {
    wattron(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
    mvwprintw(w, i + NC_OVERSEER_3, 1 + NC_OVERSEER_3_ACTION_SECONDS_AGO, "-" );
    wattroff(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
  }

  wattron(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(w, i + NC_OVERSEER_3, 1 + NC_OVERSEER_3_ACTION_HITS, std::to_string( o->hitCount ).c_str() );
  wattroff(w, NC_COLOR_PAIR_VALUE_LESSER_SET);

}
