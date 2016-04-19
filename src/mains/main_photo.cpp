#include "../backend/main.hpp"

int main() {
  h = new HomeIO();
  signal(SIGINT, handleSignal);

  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  std::shared_ptr<MeasType> m;
  m = std::make_shared<MeasType>();
  m->name = "photo_voltage";
  m->unit = "V";
  m->command = '0';
  m->responseSize = 2;
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "photo_current";
  m->unit = "A";
  m->command = '1';
  m->responseSize = 2;
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  //
  m = std::make_shared<MeasType>();
  m->name = "photo_voltage2";
  m->unit = "V";
  m->command = 'a';
  m->responseSize = 2;
  m->coefficientLinear = 0.001;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "photo_current2";
  m->unit = "A";
  m->command = 'b';
  m->responseSize = 2;
  m->coefficientLinear = 0.001;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "photo_power2";
  m->unit = "W";
  m->command = 'p';
  m->responseSize = 2;
  m->coefficientLinear = 0.001;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  h->ioServer->port = "/dev/ttyACM0";

  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 50000;

  h->fileStorage->cycleInterval = 60*1000000;

  // overseers intervals
  h->overseerArray->cycleInterval = 10000000;

  h->frontendSettings->intervalCurrent = 1000;
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 5.0;

  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = false;
  h->spy->cycleInterval = 30*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org.pl";
  h->spy->siteName = "poznan_photo";

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 10*60*1000*1000; // 10 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (unsigned long long) 3*60*60*1000*1000; // 3 hours
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute

  // ram buffer resize
  h->measFetcher->maxBufferSize = 2000000;

  h->start();

  return 0;
}
