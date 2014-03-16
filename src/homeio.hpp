#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>

#include "meas_type.hpp"
#include "io_proxy.hpp"
#include "tcp_server.hpp"

using namespace std;

class HomeIO {
public:
  HomeIO();
  int start();
  
  IoProxy *ioProxy;
  TcpServer *tcpServer;
  std::vector <MeasType> measTypes;
};

#include "homeio.cpp"
#endif
