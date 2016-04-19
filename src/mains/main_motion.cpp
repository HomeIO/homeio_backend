#include "../backend/main.hpp"

int main() {
  h = new HomeIO();
  signal(SIGINT, handleSignal); // start safe exit after ctrl+c

  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002; // you can write your own tools using provided proxy

/*
  m = std::make_shared<MeasType>();
  m->name = "motion"; // should be unique
  m->unit = ""; // example: V, A, %
  m->command = 'm'; // byte command sent to arduino
  m->responseSize = 2; // atmega ADC uses 2 bytes
  m->coefficientLinear = 1.0; // value = (raw + offeset) * linear
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000; // not store/archive if last store was consist of last 5s
  m->maxTimeDiffToStore = 3600000; // in case of no measurement change, perform store after 1h
  m->valueDiffToStore = 0.5; // perform store if abs(value - last store value) > 0.5
  m->priority = 1; // only used in frontend
  h->measTypeArray->add(m);
*/

  std::shared_ptr<MeasType> m; // shared pointers is lazy, but good, way to fight memory leaks and segfaults
  m = std::make_shared<MeasType>();
  m->name = "new_counter";
  m->unit = "";
  m->command = 'c';
  m->responseSize = 2;
  m->coefficientLinear = 1.0;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  h->ioServer->port = "/dev/ttyACM0"; // where is arduino connected

  h->measFetcher->betweenMeasInterval = 50000;  // how much sleep between measurement type and next one, microseconds
  h->measFetcher->cycleInterval = 100000; // how much sleep between all measurements, microseconds

  h->fileStorage->cycleInterval = 60*1000000; // run storage/archive and check measurement buffers every 1 minute, microseconds

  // overseers intervals
  h->overseerArray->cycleInterval = 10000000; // run overseer check every 10 seconds

  h->frontendSettings->intervalCurrent = 1000; // theese values are used to calculate proper autorefresh, default time range size
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 5.0;

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 10*60*1000*1000; // store every 10 minutes
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // first run 1 minute after startup

  // ram buffer size
  // how many measurements per type store in memory
  h->measFetcher->maxBufferSize = 2000000;

  h->start();

  return 0;
}
