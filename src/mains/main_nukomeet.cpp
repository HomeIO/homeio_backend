#include "../backend/homeio.hpp"

HomeIO *h;

void handleSignal(int s) {
  UNUSED(s);
  h->stop();
  exit(1);
}

int main()
{
  h = new HomeIO();
  signal(SIGINT, handleSignal);

  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  MeasType *m;

  m = new MeasType();
  m->name = "moisture_nukomeet";
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


  IoServer *io = new IoServer();
  io->port = "/dev/ttyACM0";
  h->ioServer = io;

  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 1000000; //50000;

  h->fileStorage->cycleInterval = 60*1000000; //3600*1000;

  // overseers intervals
  h->overseerArray->cycleInterval = 10000000;

  h->frontendSettings->intervalCurrent = 10000;
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 10.0;

  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = true;
  h->spy->cycleInterval = 5*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org.pl/";
  h->spy->siteName = "nukomeet";
  h->spy->enabled = true;

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 20*60*1000*1000; // 20 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (unsigned long long) 3*60*60*1000*1000; // 3 hours
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute

  h->start();

  return 0;
}
