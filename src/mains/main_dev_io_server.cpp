#include "../backend/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
  h->ioProxy->address = "192.168.0.2";
  h->ioProxy->port = 2002;

  MeasType *m;
  m = new MeasType();
  m->name = "test_t";
  m->command = 't';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "test_s";
  m->command = 's';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  IoServer *io = new IoServer();
  h->ioServer = io;
  
  //h->startFetch();
  //h->startServer();
  //h->start();
  h->startIoServer();
  
  return 0;
}
