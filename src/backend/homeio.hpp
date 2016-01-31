#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>
#include <signal.h>
#include <vector>

#include "meas/meas_type_array.hpp"
#include "meas/meas_fetcher.hpp"
#include "meas/meas_buffer_backup_storage.hpp"
#include "meas/meas_group.hpp"
#include "meas/file_storage.hpp"

#include "action/action_type_array.hpp"

#include "overseer/overseer_array.hpp"

#include "tcp/tcp_server.hpp"

#include "utils/helper.hpp"
#include "utils/frontend_settings.hpp"
#include "utils/spy.hpp"
#include "utils/boot.hpp"

#include "io/io_proxy.hpp"
#include "io/io_server.hpp"

#include "addons/addons_array.hpp"
#include "addons/all_addons.hpp"

#include "log/log_array.hpp"

#include "ncurses/ncurses_ui.hpp"

class HomeIO {
 public:
  HomeIO();
  ~HomeIO();

  void prepareDirectories();
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

  Boot *boot;
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
};

void* measStartThread(void*);
void* tcpServerThread(void*);
void* ioServerThread(void*);
void* overseerThread(void*);
void* fileStorageThread(void*);
void* fileBufferBackupThread(void*);
void* spyThread(void*);
void* addonsThread(void*);
void* ncursesThread(void*);
void* shutdownWatchThread(void*);

#endif
