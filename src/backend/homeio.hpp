#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>
#include <signal.h>
#include <vector>
#include <memory>

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

  std::shared_ptr<Boot> boot;
  std::shared_ptr<Spy> spy;
  std::shared_ptr<MeasFetcher> measFetcher;
  std::shared_ptr<TcpServer> tcpServer;
  std::shared_ptr<TcpCommand> tcpCommand;
  std::shared_ptr<MeasBufferBackupStorage> measBufferBackupStorage;
  std::shared_ptr<MeasTypeArray> measTypeArray;
  std::shared_ptr<IoProxy> ioProxy;
  std::shared_ptr<IoServer> ioServer;
  std::shared_ptr<ActionTypeArray> actionTypeArray;
  std::shared_ptr<OverseerArray> overseerArray;

  FileStorage *fileStorage;
  FrontendSettings *frontendSettings;
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
