#include "../backend/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  MeasType *m;
  m = new MeasType();
  m->name = "light";
  m->command = '0';
  m->responseSize = 2;
  m->coefficientLinear = -1.0;
  m->coefficientOffset = -1024;
  m->minTimeDiffToStore = 1000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 5;
  m->priority = 1;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "humidity";
  m->command = '1';
  m->responseSize = 2;
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 1000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 10;
  m->priority = 1;
  h->measTypeArray->add(m);
  
  IoServer *io = new IoServer();
  io->port = "/dev/ttyACM0";
  h->ioServer = io;
  
  h->measFetcher->betweenMeasInterval = 10000; // 10000;
  h->measFetcher->cycleInterval = 50000; //50000;
  
  h->fileStorage->cycleInterval = 2*3600*1000;
  
  // overseers intervals
  h->overseerArray->cycleInterval = 2000000;
  
  h->start();
  
  return 0;
}
