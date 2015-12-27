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

  // IoProxy connects to IoServer.
  // IoServer acts as bridge between uC via UART/RS/USB/hardware using TCP protocol.
  // It gets command, send to hardware, get hardware response and trasmit it via TCP.
  // If you run everything on same server use address 'localhost'. It's default way.
  // If you want to connect to another server where is hardware connected and IoServer is run
  // type IP of this server
  // example:
  // h->ioProxy->address = "192.168.0.20";
  // default:
  // h->ioProxy->address = "localhost";
  h->ioProxy->address = "localhost";
  // please use default port
  h->ioProxy->port = 2002;

  // Now please add type of measurements which you want backend to fetch and
  // store in RAM buffer.

  MeasType *m;

  // note: test meas is not needed
  //m = new MeasType();
  // this must be unique and will be used by frontend and overseers
  //m->name = "test_t";
  // ascii command to get measurement
  //m->command = 't';
  // number of bytes of response
  //m->responseSize = 2;
  // value = (raw + coefficientOffset)* coefficientLinear
  //m->coefficientLinear = 1.0;
  //m->coefficientOffset = 0;
  // do not store measurements if they are 'too short', in miliseconds
  //m->minTimeDiffToStore = 2000;
  // force to store measurements if too much time passed, in miliseconds
  //m->maxTimeDiffToStore = 3600000;
  // store measurement if value changed enough
  //m->valueDiffToStore = 1.0;

  // add this measuremt type to measurement which will be fetched by this server
  //h->measTypeArray->add(m);

  //m = new MeasType();
  //m->name = "test_s";
  //m->command = 's';
  //m->responseSize = 1;
  //h->measTypeArray->add(m);

  // place measurement here
  m = new MeasType();
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


  m = new MeasType();
  m->name = "i_gen_batt";
  m->unit = "A";
  m->command = '4';
  m->responseSize = 2;
  m->coefficientLinear = 0.191;
  m->coefficientOffset = -512;
  m->priority = 1;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "i_gen_resist";
  m->unit = "A";
  m->command = '5';
  m->responseSize = 2;
  m->coefficientLinear = 0.191;
  m->coefficientOffset = -512;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "i_inverters";
  m->unit = "A";
  m->command = '6';
  m->responseSize = 2;
  m->coefficientLinear = 0.191;
  m->coefficientOffset = -512;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "imp_per_min";
  m->unit = "imp/min";
  m->command = 'l';
  m->responseSize = 2;
  m->coefficientLinear = 60.0;
  m->coefficientOffset = 0;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "coil_1_u";
  m->unit = "V";
  m->command = '0';
  m->responseSize = 2;
  m->coefficientLinear = 0.0777126099706744868;
  m->coefficientOffset = 0;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "coil_2_u";
  m->unit = "V";
  m->command = '1';
  m->responseSize = 2;
  m->coefficientLinear = 0.0777126099706744868;
  m->coefficientOffset = 0;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "coil_3_u";
  m->unit = "V";
  m->command = '2';
  m->responseSize = 2;
  m->coefficientLinear = 0.0777126099706744868;
  m->coefficientOffset = 0;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "res_pwm";
  m->unit = "pwm";
  m->command = 'p';
  m->responseSize = 2;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "res_pwm_avg";
  m->unit = "pwm";
  m->command = 'q';
  m->responseSize = 2;
  h->measTypeArray->add(m);

  m = new MeasType();
  m->name = "outputs";
  m->unit = "bit array";
  m->command = 'o';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  // end measurements here

  IoServer *io = new IoServer();
  // port device connect to
  // example: "/dev/ttyUSB0", "/dev/ttyS0"
  io->port = "/dev/ttyUSB0";
  h->ioServer = io;

  // meas fetcher intervals
  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 50000;

  h->fileStorage->cycleInterval = 24*3600*1000;

  // actions
  ActionType *a;
  a = new ActionType();
  // this must be unique and will be used by frontend and overseers
  a->name = "start_total_brake";
  // ascii command to get measurement
  a->command = 'w';
  // number of bytes of response
  a->responseSize = 1;
  // "okay" response value
  a->responseOkay = 21;
  // add this measuremt type to measurement which will be fetched by this server
  h->actionTypeArray->add(a);

  a = new ActionType();
  // this must be unique and will be used by frontend and overseers
  a->name = "turn_off_brake";
  // ascii command to get measurement
  a->command = 'W';
  // number of bytes of response
  a->responseSize = 1;
  // "okay" response value
  a->responseOkay = 20;
  // add this measuremt type to measurement which will be fetched by this server
  h->actionTypeArray->add(a);


  a = new ActionType();
  a->name = "inverter_A_off"; // "output_2_off";
  a->command = 'b';
  a->responseSize = 1;
  a->responseOkay = 1;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "inverter_B_off"; // "output_3_off";
  a->command = 'c';
  a->responseSize = 1;
  a->responseOkay = 2;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "inverter_A_on"; // "output_2_on";
  a->command = 'B';
  a->responseSize = 1;
  a->responseOkay = 11;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "inverter_B_on"; // "output_3_on";
  a->command = 'C';
  a->responseSize = 1;
  a->responseOkay = 12;
  h->actionTypeArray->add(a);



  // overseers intervals
  h->overseerArray->cycleInterval = 2000000;

  // overseers
  Overseer *o;
  o = new Overseer();
  // this must be unique and can be used to access overseer parameters in future
  o->name = "brake_on_high_batt_voltage";
  // string name of action
  o->actionName = "start_total_brake";
  // string name of measurement
  o->measName = "batt_u";
  // critical value
  o->thresholdValue = 46.5;
  // check if value is higher
  o->isMax = true;
  // use average value using X raw measurements
  o->windowSize = 300;
  // add this overseer type to list
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "no_brake_on_low_batt_voltage";
  o->actionName = "turn_off_brake";
  o->measName = "batt_u";
  o->thresholdValue = 29.0;
  o->isMax = false;
  o->windowSize = 100;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_on_inverter_A_on_high_coil";
  o->actionName = "inverter_A_on";
  o->measName = "coil_1_u";
  o->thresholdValue = 30.0;
  o->isMax = true;
  o->windowSize = 100;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_on_inverter_A_on_high_battery";
  o->actionName = "inverter_A_on";
  o->measName = "batt_u";
  o->thresholdValue = 36.0;
  o->isMax = true;
  o->windowSize = 200;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_A_on_low_coil";
  o->actionName = "inverter_A_off";
  o->measName = "coil_1_u";
  o->thresholdValue = 2.0;
  o->isMax = false;
  o->windowSize = 50;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_B_on_low_coil";
  o->actionName = "inverter_B_off";
  o->measName = "coil_1_u";
  o->thresholdValue = 2.0;
  o->isMax = false;
  o->windowSize = 50;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_on_inverter_B_on_high_battery";
  o->actionName = "inverter_B_on";
  o->measName = "batt_u";
  o->thresholdValue = 30.0;
  o->isMax = true;
  o->windowSize = 1000;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_B_on_low_battery";
  o->actionName = "inverter_B_off";
  o->measName = "batt_u";
  o->thresholdValue = 23.0;
  o->isMax = false;
  o->windowSize = 100;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_A_on_low_battery";
  o->actionName = "inverter_A_off";
  o->measName = "batt_u";
  o->thresholdValue = 23.0;
  o->isMax = false;
  o->windowSize = 100;
  h->overseerArray->add(o);


  // settings which will only be usable in frontend
  h->frontendSettings->intervalCurrent = 1000;
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 0.4;

  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = true;
  h->spy->cycleInterval = 10*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org.pl";
  h->spy->siteName = "lakie_dev";
  h->spy->enabled = true;

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 10*60*1000*1000; // 10 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (long) 20*60*1000*1000; // 20 minutes
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute

  // ram buffer resize
  h->measFetcher->maxBufferSize = 2000000; // 4000000;

  // addons
  WindTurbineStatsAddon *wts = new WindTurbineStatsAddon;
  wts->measNameU = "batt_u";
  wts->measNameI = "i_gen_batt";
  wts->measCoil = "coil_1_u";
  wts->measResistor = "res_pwm_avg";
  h->addonsArray->addons.push_back(std::unique_ptr<AbstractAddon>(wts));

  // Now you can run everything of parts of backend.
  // TODO: way to disable IoServer
  h->start();

  return 0;
}
