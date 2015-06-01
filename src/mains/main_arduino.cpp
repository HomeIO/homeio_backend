#include "../backend/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  MeasType *m;
  m = new MeasType();
  m->name = "light";
  m->unit = "%";
  m->command = '0';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 3.0;
  m->priority = 1;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "moisture_shadow";
  m->unit = "%";
  m->command = '5';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 3.0;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "moisture_external";
  m->unit = "%";
  m->command = '4';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 3.0;
  m->priority = 1;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "moisture_top";
  m->unit = "%";
  m->command = '3';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 3.0;
  m->priority = 1;
  h->measTypeArray->add(m);

  
  /*
  // something not worky
  m = new MeasType();
  m->name = "temperature";
  m->command = '2';
  m->responseSize = 2;
  m->coefficientLinear = 0.48828125;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 1.0;
  m->priority = 1;
  h->measTypeArray->add(m);
  */
  
  IoServer *io = new IoServer();
  io->port = "/dev/ttyACM0";
  h->ioServer = io;
  
  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 10000000; //50000;
  //h->measFetcher->cycleInterval = 50000; //50000;
  
  h->fileStorage->cycleInterval = 60*1000000; //3600*1000;
  
  // overseers intervals
  h->overseerArray->cycleInterval = 10000000;
  
  h->frontendSettings->intervalCurrent = 10000;
  h->frontendSettings->intervalHistory = 3600*1000;
  
  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = true;
  h->spy->cycleInterval = 30*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org.pl/";
  h->spy->siteName = "poznan_dev";
  h->spy->enabled = true;
  
  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 20*60*1000*1000; // 20 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (unsigned long long) 3*60*60*1000*1000; // 3 hours
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute
  
  h->start();
  
  return 0;
}
