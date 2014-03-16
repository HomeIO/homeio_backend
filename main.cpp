#include "src/homeio.hpp"

int main()
{
  HomeIO *h = new HomeIO();
  
  h->ioProxy->address = "192.168.0.2";
  h->ioProxy->port = 2002;
  
  MeasType *m;
  m = new MeasType();
  m->name = "batt_u";
  m->command = '0';
  m->responseSize = 2;
  
  h->measTypes.push_back(*m);
  
  m = new MeasType();
  m->name = "test_t";
  m->command = 't';
  m->responseSize = 2;
  
  h->measTypes.push_back(*m);
  
  m = new MeasType();
  m->name = "test_s";
  m->command = 's';
  m->responseSize = 1;
  
  h->measTypes.push_back(*m);
  
  h->start();
  
  return 0;
}
