#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>
#include <signal.h>

#include "helpers.hpp"
#include "meas_type_array.hpp"
#include "meas_fetcher.hpp"
#include "io_proxy.hpp"
#include "tcp_server.hpp"
#include "action_type_array.hpp"
#include "overseer_array.hpp"
#include "file_storage.hpp"
#include "meas_buffer_backup_storage.hpp"
#include "frontend_settings.hpp"
#include "spy.hpp"

#include "io_server/io_server.hpp"

using namespace std;

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
  
  bool ioServerReady;
};

#include "homeio.cpp"
#endif
