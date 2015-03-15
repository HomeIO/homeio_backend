#include "../backend/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
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
  m = new MeasType();
  // this must be unique and will be used by frontend and overseers
  m->name = "test_t";
  // ascii command to get measurement 
  m->command = 't';
  // number of bytes of response 
  m->responseSize = 2;
  // add this measuremt type to measurement which will be fetched by this server
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "test_s";
  m->command = 's';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  // place measurement here
    m = new MeasType();
  m->name = "batt_u";
  m->command = '3';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "i_gen_batt";
  m->command = '4';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "i_gen_resist";
  m->command = '5';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "i_inverters";
  m->command = '6';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "imp_per_min";
  m->command = 'l';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "coil_1_u";
  m->command = '0';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "coil_2_u";
  m->command = '1';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "coil_3_u";
  m->command = '2';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "res_pwm";
  m->command = 'p';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "res_pwm_avg";
  m->command = 'q';
  m->responseSize = 2;
  h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "outputs";
  m->command = 'o';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  // end measurements here
  
  IoServer *io = new IoServer();
  // port device connect to
  // example: "/dev/ttyUSB0", "/dev/ttyS0"
  io->port = "/dev/ttyUSB0";
  h->ioServer = io;
  
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
  
  // overseers
  Overseer *o;
  o = new Overseer();
  // this must be unique and can be used to access overseer parameters in future
  o->name = "brake_on_high_voltage";
  // string name of action
  o->actionName = "start_total_brake";
  // string name of measurement
  o->measName = "batt_u";
  // add this overseer type to list
  h->overseerArray->add(o);
  
  // Now you can run everything of parts of backend. 
  // TODO: way to disable IoServer
  h->start();
  
  return 0;
}
