#include "ncurses_meas.hpp"

#define NC_MEAS_NAME 0
#define NC_MEAS_VALUE NC_MEAS_NAME + 30
#define NC_MEAS_RAW NC_MEAS_VALUE + 28
#define NC_MEAS_OFFSET NC_MEAS_RAW + 20

NcursesMeas::NcursesMeas() {
  page = 0;
  perPage = 1;
  maxPage = 1;
}

unsigned int NcursesMeas::setPage(unsigned int p) {
  perPage = (unsigned int) (LINES - 8);
  maxPage = (unsigned int) ( ceil( (float) (measTypeArray->measTypes.size()) / (float) (perPage) ) );

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  } else {
    page = p;
  }

  return page;
}

void NcursesMeas::renderPage(WINDOW *w) {
  mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
}

void NcursesMeas::render(WINDOW *w) {
  renderPage(w);

  mvwprintw(w, 1, 1 + NC_MEAS_NAME, "Name" );
  mvwprintw(w, 1, 1 + NC_MEAS_VALUE, "Value" );
  mvwprintw(w, 1, 1 + NC_MEAS_RAW, "Raw" );
  mvwprintw(w, 1, 1 + NC_MEAS_OFFSET, "Offset" );

  unsigned int i;
  long unsigned int j;
  MeasType *element;

  for (i = 0; i < perPage; i++) {
    j = (perPage * page) + i;
    if (j < measTypeArray->measTypes.size()) {
      element = &measTypeArray->measTypes.at(j) ;
      renderMeas(w, element, (int) (i + 3) );
    }
  }
}

void NcursesMeas::renderMeas(WINDOW *w, MeasType *m, int i) {
  std::string valueString = "";

  std::ostringstream os;
  os << std::setprecision(5) << m->lastValue();
  valueString += os.str();;
  valueString += " ";
  valueString += m->unit;

  std::string rawString = "";
  rawString += to_string(m->lastRaw());

  string offsetString = "";
  offsetString += to_string(m->buffer->offset);

  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, i, 1 + NC_MEAS_NAME, m->name.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  wattron(w, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(w, i, 1 + NC_MEAS_VALUE, valueString.c_str());
  wattroff(w, NC_COLOR_PAIR_VALUE_SET);

  wattron(w, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(w, i, 1 + NC_MEAS_RAW, rawString.c_str());
  wattroff(w, NC_COLOR_PAIR_VALUE_LESSER_SET);

  wattron(w, NC_COLOR_PAIR_LESSER_SET);
  mvwprintw(w, i, 1 + NC_MEAS_OFFSET, offsetString.c_str());
  wattroff(w, NC_COLOR_PAIR_LESSER_SET);
}
