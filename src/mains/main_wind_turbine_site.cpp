#include "../backend/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
  h->ioProxy->address = "localhost";
  h->ioProxy->port = 2002;

  MeasType *m;
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
  //m = new MeasType();
  //m->name = "test_t";
  //m->command = 't';
  //m->responseSize = 2;
  //h->measTypeArray->add(m);
  
  m = new MeasType();
  m->name = "test_s";
  m->command = 's';
  m->responseSize = 1;
  h->measTypeArray->add(m);

  //cout << h->measTypeArray->byName("batt_u");
  
  //h->startFetch();
  //h->startServer();
  h->start();
  
  return 0;
}
