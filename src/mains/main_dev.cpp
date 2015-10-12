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
  m->name = "light";
  m->unit = "%";
  m->command = '0';
  m->responseSize = 2;
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 1000;
  m->maxTimeDiffToStore = 5000; // 3600000
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "outputs";
  m->unit = "bit array";
  m->command = 'o';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  IoServer *io = new IoServer();
  io->port = "/dev/ttyUSB0";
  h->ioServer = io;

  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 50000;

  h->fileStorage->cycleInterval = 2*3600*1000;

  ActionType *a;
  a = new ActionType();
  a->name = "turn_led_on"; // led on pin 3 (count from 0)
  a->command = 'D';
  a->responseSize = 1;
  a->responseOkay = 13;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "turn_led_off"; // led on pin 3 (count from 0)
  a->command = 'd';
  a->responseSize = 1;
  a->responseOkay = 3;
  h->actionTypeArray->add(a);

  // overseers intervals
  h->overseerArray->cycleInterval = 2000000;

  // overseers
  Overseer *o;
  o = new Overseer();
  o->name = "turn_led_off_on_high_light";
  o->actionName = "turn_led_off";
  o->measName = "light";
  o->thresholdValue = 200.0; // TODO
  o->isMax = true;
  o->windowSize = 60;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_led_on_on_low_light";
  o->actionName = "turn_led_on";
  o->measName = "light";
  o->thresholdValue = 160.0; // TODO
  o->isMax = false;
  o->windowSize = 60;
  h->overseerArray->add(o);

  h->measGroup->addGroup("group1", "light");
  h->measGroup->addGroup("group2", "light,outputs");

  h->frontendSettings->intervalCurrent = 1000;
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 1.0;

  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = true;
  h->spy->cycleInterval = 10*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org.pl";
  h->spy->siteName = "dev";
  h->spy->enabled = true;

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = 60*1000*1000; // 1 minute
  h->measBufferBackupStorage->thresholdTimeRange = h->measBufferBackupStorage->cycleInterval * 5;
  h->measBufferBackupStorage->usDelay = h->measFetcher->cycleInterval * 20; // fast for dev

  h->start();

  return 0;
}
