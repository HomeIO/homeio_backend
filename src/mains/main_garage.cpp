#include "../backend/main.hpp"

int main() {
  h = new HomeIO();
  signal(SIGINT, handleSignal);

  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  std::shared_ptr<MeasType> m;

  m = std::make_shared<MeasType>();
  m->name = "sync";
  m->unit = "x";
  m->command = 's';
  m->responseSize = 1;
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "usb_rasp_current";
  m->unit = "A";
  m->command = 'a';
  m->responseSize = 2;
  // direct: 512 -> 0A
  // multiplied 8192 -> 0A
  // multiplied 16384 -> 5A
  m->coefficientLinear = 0.0006103515625;
  m->coefficientOffset = -8192;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "battery_voltage";
  m->unit = "V";
  m->command = 'A';
  m->responseSize = 2;
  m->coefficientLinear = 0.2;
  m->coefficientOffset = -512;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  // m = std::make_shared<MeasType>();
  // m->name = "battery_charge_current";
  // m->unit = "A";
  // m->command = 'b';
  // m->responseSize = 2;
  // m->coefficientLinear = 0.0006103515625;
  // m->coefficientOffset = -8192;
  // m->minTimeDiffToStore = 5000;
  // m->maxTimeDiffToStore = 3600000;
  // m->valueDiffToStore = 0.5;
  // m->priority = 0;
  // h->measTypeArray->add(m);

  h->ioServer->port = "/dev/ttyACM0";

  h->measFetcher->betweenMeasInterval = 100;
  h->measFetcher->cycleInterval = 500000;

  h->fileStorage->cycleInterval = 60*1000000; //3600*1000;

  // overseers intervals
  h->overseerArray->cycleInterval = 10000000;

  h->frontendSettings->intervalCurrent = 10000;
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 5.0;

  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = false;
  h->spy->cycleInterval = 30*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org";
  h->spy->siteName = "garage";

  h->measGroup->addGroup("temperature", "temperature_garage");

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 20*60*1000*1000; // 20 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (unsigned long long) 3*60*60*1000*1000; // 3 hours
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute

  // ram buffer resize
  h->measFetcher->maxBufferSize = 2000000;

  // std::unique_ptr<FactoryMeasPeriodic> fmpd = std::make_unique<FactoryMeasPeriodic>();
  // fmpd->namePrefix = "stats_daily_";
  // fmpd->bufferMax = 365;
  // fmpd->calcInterval = 10*60*1000; // every 5 minutes
  // fmpd->interval = 24*60*60*1000; // daily
  // fmpd->makeItSo(h->measTypeArray, h->addonsArray);

  h->start();

  return 0;
}