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
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 1000;
  m->maxTimeDiffToStore = 5000; // 3600000
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "outputs";
  m->command = 'o';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  IoServer *io = new IoServer();
  io->port = "/dev/ttyUSB0";
  h->ioServer = io;
  
  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 50000;
  
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
  
  h->start();
  
  return 0;
}
