#include "ncurses_home.hpp"

NcursesHome::NcursesHome() {
}

#define NC_THREADS_BEGIN 5
#define NC_THREADS_NCURSES NC_THREADS_BEGIN + 1
#define NC_THREADS_IO_SERVER NC_THREADS_BEGIN + 1
#define NC_THREADS_MEAS NC_THREADS_IO_SERVER + 1
#define NC_THREADS_OVERSEER NC_THREADS_MEAS + 1
#define NC_THREADS_TCP_SERVER NC_THREADS_OVERSEER + 1
#define NC_THREADS_FILE_STORAGE NC_THREADS_TCP_SERVER + 1
#define NC_THREADS_BUFFER_BACKUP NC_THREADS_FILE_STORAGE + 1
#define NC_THREADS_ADDONS NC_THREADS_BUFFER_BACKUP + 1
#define NC_THREADS_SPY NC_THREADS_ADDONS + 1
#define NC_THREADS_SHUTDOWN NC_THREADS_SPY + 1

#define NC_THREADS_STATUS_RUN "RUN"
#define NC_THREADS_STATUS_STOP "STOP"

#define NC_THREADS_STATUS 30

void NcursesHome::renderStatus(WINDOW *w, unsigned char row, bool status) {
  if (status) {
    wattron(w, NC_COLOR_PAIR_NAME_SET);
    mvwprintw(w, row, NC_THREADS_STATUS, NC_THREADS_STATUS_RUN );
    wattroff(w, NC_COLOR_PAIR_NAME_SET);
  } else {
    wattron(w, NC_COLOR_PAIR_ERROR_SET);
    mvwprintw(w, row, NC_THREADS_STATUS, NC_THREADS_STATUS_STOP );
    wattroff(w, NC_COLOR_PAIR_ERROR_SET);
  }
}

void NcursesHome::render(WINDOW *w) {
  mvwprintw(w, 1, 1, "Hello, current time is:" );
  mvwprintw(w, 2, 1, Helper::detailCurrentTime().c_str() );

  mvwprintw(w, NC_THREADS_BEGIN, 1, "UI" );
  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, NC_THREADS_BEGIN, NC_THREADS_STATUS, NC_THREADS_STATUS_RUN );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  mvwprintw(w, NC_THREADS_IO_SERVER, 1, "IO" );
  renderStatus(w, NC_THREADS_IO_SERVER, ioServer->ready);

  mvwprintw(w, NC_THREADS_MEAS, 1, "Meas" );
  renderStatus(w, NC_THREADS_MEAS, measFetcher->ready);

  mvwprintw(w, NC_THREADS_OVERSEER, 1, "Overseer" );
  renderStatus(w, NC_THREADS_OVERSEER, overseerArray->ready);

  mvwprintw(w, NC_THREADS_TCP_SERVER, 1, "TCP" );
  renderStatus(w, NC_THREADS_TCP_SERVER, tcpServer->ready);

  mvwprintw(w, NC_THREADS_FILE_STORAGE, 1, "Meas storage" );
  renderStatus(w, NC_THREADS_FILE_STORAGE, fileStorage->ready);

  mvwprintw(w, NC_THREADS_BUFFER_BACKUP, 1, "Meas buffer backup" );
  renderStatus(w, NC_THREADS_BUFFER_BACKUP, measBufferBackupStorage->ready);

  mvwprintw(w, NC_THREADS_ADDONS, 1, "Addons" );
  renderStatus(w, NC_THREADS_ADDONS, addonsArray->ready);

  mvwprintw(w, NC_THREADS_SPY, 1, "Spy" );
  renderStatus(w, NC_THREADS_SPY, spy->ready);

  //mvwprintw(w, NC_THREADS_SHUTDOWN, 1, "shutdownWatchThread" );
  //mvwprintw(w, NC_THREADS_SHUTDOWN, NC_THREADS_STATUS, std::to_string(ioServer->ready).c_str() );
  //renderStatus(w, NC_THREADS_MEAS, measFetcher->ready);
  //h->ncursesUI->beginShutdown


}
