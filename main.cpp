#include "src/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
  h->ioProxy->address = "192.168.0.2";
  h->ioProxy->port = 2002;
  
  MeasType *m;
  m = new MeasType();
  m->name = "batt_u";
  m->command = '0';
  m->responseSize = 2;

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

  //cout << h->measTypeArray->byName("batt_u");
  
  //h->startFetch();
  //h->startServer();
  h->start();
  
  return 0;
}
