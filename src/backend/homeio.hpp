#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>
#include <signal.h>
#include <vector>

#include "helper.hpp"

#include "meas/meas_type_array.hpp"
#include "meas/meas_fetcher.hpp"
#include "meas/meas_buffer_backup_storage.hpp"
#include "meas/meas_group.hpp"
#include "meas/file_storage.hpp"

#include "tcp_server.hpp"
#include "action_type_array.hpp"
#include "overseer_array.hpp"
#include "frontend_settings.hpp"
#include "spy.hpp"
#include "addons_array.hpp"

#include "io/io_proxy.hpp"
#include "io/io_server.hpp"

#include "addons/all_addons.hpp"

#include "log/log_array.hpp"
#include "ncurses/ncurses_ui.hpp"

class HomeIO {
public:
  HomeIO();
  unsigned char startFetch();
  unsigned char startServer();
  unsigned char startIoServer();
  unsigned char startOverseer();
  unsigned char startFileStorage();
  unsigned char startBufferBackupStorage();
  unsigned char startSpy();
  unsigned char startAddons();
  unsigned char startNcurses();
  unsigned char start();

  unsigned char stop();
  bool isSafeToShutdown();

  void copyInternalDelays();

  MeasTypeArray *measTypeArray;
  MeasFetcher *measFetcher;
  IoProxy *ioProxy;
  TcpServer *tcpServer;
  TcpCommand *tcpCommand;
  IoServer *ioServer;
  ActionTypeArray *actionTypeArray;
  OverseerArray *overseerArray;
  FileStorage *fileStorage;
  MeasBufferBackupStorage *measBufferBackupStorage;
  FrontendSettings *frontendSettings;
  Spy *spy;
  MeasGroup *measGroup;
  AddonsArray *addonsArray;
  NcursesUI *ncursesUI;
  LogArray *logArray;

  bool ioServerReady;
};

//#include "homeio.cpp"
#endif
