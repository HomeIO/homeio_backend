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
  m->name = "moisture_shadow";
  m->unit = "%";
  m->command = '5';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "moisture_external";
  m->unit = "%";
  m->command = '4';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "moisture_top";
  m->unit = "%";
  m->command = '3';
  m->responseSize = 2;
  m->coefficientLinear = -0.09765625;
  m->coefficientOffset = -1023;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  /*
  // temporary not used
  m = std::make_shared<MeasType>();
  m->name = "lm35_temperature";
  m->unit = "C";
  m->command = '2';
  m->responseSize = 2;
  m->coefficientLinear = 0.48828125;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);
  */

  m = std::make_shared<MeasType>();
  m->name = "int_temperature";
  m->unit = "C";
  m->command = 'd';
  m->responseSize = 2;
  m->coefficientLinear = 0.1;
  m->coefficientOffset = -500;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.3;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "int_humidity";
  m->unit = "%";
  m->command = 'h';
  m->responseSize = 2;
  m->coefficientLinear = 0.1;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.3;
  m->priority = 0;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "ext_temperature";
  m->unit = "C";
  m->command = 'D';
  m->responseSize = 2;
  m->coefficientLinear = 0.1;
  m->coefficientOffset = -500;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 1;
  m->extRemoveSpikes = true;
  m->extBackendRemoveSpikes = true;
  h->measTypeArray->add(m);

  m = std::make_shared<MeasType>();
  m->name = "ext_humidity";
  m->unit = "%";
  m->command = 'H';
  m->responseSize = 2;
  m->coefficientLinear = 0.1;
  m->coefficientOffset = 0;
  m->minTimeDiffToStore = 5000;
  m->maxTimeDiffToStore = 3600000;
  m->valueDiffToStore = 0.5;
  m->priority = 0;
  h->measTypeArray->add(m);

  h->ioServer->port = "/dev/ttyACM0";

  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 5000000;
  //h->measFetcher->cycleInterval = 50000;

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
  h->spy->hiveHost = "http://hive.homeio.org.pl";
  h->spy->siteName = "poznan_dev";

  h->measGroup->addGroup("soil_moisture", "moisture_shadow,moisture_external,moisture_top");
  h->measGroup->addGroup("temperature", "int_temperature,ext_temperature");
  h->measGroup->addGroup("humidity", "int_humidity,ext_humidity");
  h->measGroup->addGroup("temp_and_sun", "int_temperature,ext_temperature,light");

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 20*60*1000*1000; // 20 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (unsigned long long) 3*60*60*1000*1000; // 3 hours
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute

  // ram buffer resize
  h->measFetcher->maxBufferSize = 2000000;

  //addons
  SunStatsAddon *ssa = new SunStatsAddon;
  //ssa->name = "sun_stats";
  ssa->lightMeasName = "light";
  h->addonsArray->addons.push_back(std::unique_ptr<AbstractAddon>(ssa));

  PlantMonitorAddon *pma = new PlantMonitorAddon;
  pma->plantMeasNames.push_back("moisture_shadow");
  pma->plantMeasNames.push_back("moisture_external");
  pma->plantMeasNames.push_back("moisture_top");
  h->addonsArray->addons.push_back(std::unique_ptr<AbstractAddon>(pma));

  std::unique_ptr<FactoryMeasPeriodic> fmpd = std::make_unique<FactoryMeasPeriodic>();
  fmpd->namePrefix = "stats_daily_";
  fmpd->bufferMax = 365;
  fmpd->calcInterval = 10*60*1000; // every 5 minutes
  fmpd->interval = 24*60*60*1000; // daily
  fmpd->makeItSo(h->measTypeArray, h->addonsArray);

  std::unique_ptr<FactoryMeasPeriodic> fmph = std::make_unique<FactoryMeasPeriodic>();
  fmph->namePrefix = "stats_hourly_";
  fmph->bufferMax = 60*24;
  fmph->calcInterval = 5*60*1000; // every 5 minutes
  fmph->interval = 60*60*1000; // daily
  fmph->makeItSo(h->measTypeArray, h->addonsArray);


  h->start();

  return 0;
}
