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
#define NC_THREADS_STATUS_CHANGING "CHANGING"
#define NC_THREADS_STATUS_WORK "WORK"

#define NC_THREADS_STATUS_POS 30
#define NC_THREADS_CHANING_POS NC_THREADS_STATUS_POS + 12
#define NC_THREADS_WORK_POS NC_THREADS_CHANING_POS + 12
#define NC_THREADS_TEXT_POS NC_THREADS_WORK_POS + 12

void NcursesHome::renderStatus(WINDOW *w, unsigned char row, bool status, bool changing, bool working, std::string statusText) {
  if (status) {
    wattron(w, NC_COLOR_PAIR_NAME_SET);
    mvwprintw(w, row, NC_THREADS_STATUS_POS, NC_THREADS_STATUS_RUN );
    wattroff(w, NC_COLOR_PAIR_NAME_SET);
  } else {
    wattron(w, NC_COLOR_PAIR_ERROR_SET);
    mvwprintw(w, row, NC_THREADS_STATUS_POS, NC_THREADS_STATUS_STOP );
    wattroff(w, NC_COLOR_PAIR_ERROR_SET);
  }

  if (changing) {
    wattron(w, NC_COLOR_PAIR_SYMBOL_SET);
    mvwprintw(w, row, NC_THREADS_CHANING_POS, NC_THREADS_STATUS_CHANGING );
    wattroff(w, NC_COLOR_PAIR_SYMBOL_SET);
  }

  if (working) {
    wattron(w, NC_COLOR_PAIR_NAME_LESSER_SET);
    mvwprintw(w, row, NC_THREADS_WORK_POS, NC_THREADS_STATUS_WORK );
    wattroff(w, NC_COLOR_PAIR_NAME_LESSER_SET);
  }

  if (statusText != "") {
    wattron(w, NC_COLOR_PAIR_TEXT_SET);
    mvwprintw(w, row, NC_THREADS_TEXT_POS, statusText.c_str() );
    wattroff(w, NC_COLOR_PAIR_TEXT_SET);
  }
}

void NcursesHome::render(WINDOW *w) {
  mvwprintw(w, 1, 1, "Hello, current time is:" );
  mvwprintw(w, 2, 1, Helper::detailCurrentTime().c_str() );

  mvwprintw(w, NC_THREADS_BEGIN, 1, "UI" );
  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, NC_THREADS_BEGIN, NC_THREADS_STATUS_POS, NC_THREADS_STATUS_RUN );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  mvwprintw(w, NC_THREADS_IO_SERVER, 1, "IO" );
  renderStatus(w, NC_THREADS_IO_SERVER, boot->ioServer->ready, boot->ioServer->changing, boot->ioServer->work, "" );

  mvwprintw(w, NC_THREADS_MEAS, 1, "Meas" );
  renderStatus(w, NC_THREADS_MEAS, boot->measFetcher->ready, boot->measFetcher->changing, boot->measFetcher->work, "");

  mvwprintw(w, NC_THREADS_OVERSEER, 1, "Overseer" );
  renderStatus(w, NC_THREADS_OVERSEER, boot->overseerArray->ready, boot->overseerArray->changing, boot->overseerArray->work, boot->overseerArray->statusText() );

  mvwprintw(w, NC_THREADS_TCP_SERVER, 1, "TCP" );
  renderStatus(w, NC_THREADS_TCP_SERVER, boot->tcpServer->ready, boot->tcpServer->changing, boot->tcpServer->work, "");

  mvwprintw(w, NC_THREADS_FILE_STORAGE, 1, "Meas storage" );
  renderStatus(w, NC_THREADS_FILE_STORAGE, boot->fileStorage->ready, boot->fileStorage->changing, boot->fileStorage->work, boot->fileStorage->statusText());

  mvwprintw(w, NC_THREADS_BUFFER_BACKUP, 1, "Meas buffer backup" );
  renderStatus(w, NC_THREADS_BUFFER_BACKUP, boot->measBufferBackupStorage->ready, boot->measBufferBackupStorage->changing, boot->measBufferBackupStorage->work, boot->measBufferBackupStorage->statusText());

  mvwprintw(w, NC_THREADS_ADDONS, 1, "Addons" );
  renderStatus(w, NC_THREADS_ADDONS, boot->addonsArray->ready, boot->addonsArray->changing, boot->addonsArray->work, "");

  mvwprintw(w, NC_THREADS_SPY, 1, "Spy" );
  renderStatus(w, NC_THREADS_SPY, boot->spy->ready, boot->spy->changing, boot->spy->work, "");

  //mvwprintw(w, NC_THREADS_SHUTDOWN, 1, "shutdownWatchThread" );
  //mvwprintw(w, NC_THREADS_SHUTDOWN, NC_THREADS_STATUS, std::to_string(ioServer->ready).c_str() );
  //renderStatus(w, NC_THREADS_MEAS, measFetcher->ready);
  //h->ncursesUI->beginShutdown


}
