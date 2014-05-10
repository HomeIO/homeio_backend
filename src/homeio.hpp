#ifndef HOMEIO
#define	HOMEIO

#include <pthread.h>

#include "meas_type_array.hpp"
#include "meas_fetcher.hpp"
#include "io_proxy.hpp"
#include "tcp_server.hpp"

using namespace std;

class HomeIO {
public:
  HomeIO();
  unsigned char startFetch();
  unsigned char startServer();
  unsigned char start();
  
  MeasTypeArray *measTypeArray;
  MeasFetcher *measFetcher;
  IoProxy *ioProxy;
  TcpServer *tcpServer;
};

#include "homeio.cpp"
#endif
