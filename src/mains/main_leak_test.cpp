#include "../backend/main.hpp"

int main() {
  h = new HomeIO();
  signal(SIGINT, handleSignal);

  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  std::shared_ptr<MeasType> m;
  m = std::make_shared<MeasType>();
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

  m = std::make_shared<MeasType>();
  m->name = "batt_u";
  m->unit = "V";
  m->command = '3';
  m->responseSize = 2;
  m->coefficientLinear = 0.0777126099706744868;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 1000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 2.0;
  m->priority = 1;
  h->measTypeArray->add(m);

  h->ioServer->port = "/dev/ttyACM0";
  h->spy->enabled = false;

  h->boot->beginShutdown = true;

  //h->measFetcher->betweenMeasInterval = 10000;
  //h->measFetcher->cycleInterval = 100000;

  h->start();

  //delete h;

  return 0;
}
