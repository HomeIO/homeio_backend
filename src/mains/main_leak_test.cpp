#include "../backend/main.hpp"

int main() {
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
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  ActionType *a;
  a = new ActionType();
  a->name = "inverter_A_off"; // "output_2_off";
  a->command = 'b';
  a->responseSize = 1;
  a->responseOkay = 1;
  h->actionTypeArray->add(a);

  // overseers
  Overseer *o;
  o = new Overseer();
  o->name = "brake_on_high_batt_voltage";
  o->actionName = "inverter_A_off";
  o->measName = "light";
  o->thresholdValue = 46.5;
  o->isMax = true;
  o->windowSize = 300;
  h->overseerArray->add(o);

  h->ioServer->port = "/dev/ttyACM0";
  h->spy->enabled = false;

  h->boot->beginShutdown = true;

  h->start();

  delete h;

  return 0;

  // TODO add action, overseer and disable all objects before deletions in proper way
}
