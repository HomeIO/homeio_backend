#include "src/meas_type.cpp"
#include "src/io_proxy.cpp"

int main()
{
  IoProxy *i = new IoProxy();
  i->address = "192.168.0.2";
  i->port = 2002;
  
  MeasType *mt = new MeasType("batt_u");
  mt->ioProxy = i;
  mt->command = '0';
  mt->responseSize = 2;
  
  mt->fetch();
  
  /*
  a->assignBufferSize(1000);
  a->addRaw(1);
  a->addRaw(1);
  a->addRaw(1);
  a->assignBufferSize(1000);
  */
  
  
  
  //i->test();
  
  return 0;
}
