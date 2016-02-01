#ifndef NCURSES_HOME_HPP
#define	NCURSES_HOME_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>


#include "../utils/helper.hpp"
#include "ncurses_colors.hpp"
#include "../log/log_array.hpp"

#include "../io/io_server.hpp"
#include "../meas/meas_fetcher.hpp"
#include "../overseer/overseer_array.hpp"
#include "../addons/addons_array.hpp"
#include "../utils/spy.hpp"
#include "../meas/meas_buffer_backup_storage.hpp"
#include "../meas/file_storage.hpp"
#include "../tcp/tcp_server.hpp"

class NcursesHome {
 public:
  NcursesHome();
  void render(WINDOW *w);
  void renderStatus(WINDOW *w, unsigned char row, bool status);

  std::shared_ptr<Spy> spy;
  std::shared_ptr<MeasFetcher> measFetcher;
  std::shared_ptr<TcpServer> tcpServer;
  std::shared_ptr<MeasBufferBackupStorage> measBufferBackupStorage;
  std::shared_ptr<IoServer> ioServer;
  std::shared_ptr<OverseerArray> overseerArray;

  FileStorage *fileStorage;
  AddonsArray *addonsArray;

  LogArray *logArray;
};

#endif
