#include "src/meas_type.cpp"
#include "src/io_proxy.cpp"

int main()
{
  IoProxy *ip = new IoProxy();
  ip->address = "192.168.0.2";
  ip->port = 2002;
  ip->prepareSocket();
  
  MeasType *mt = new MeasType("batt_u");
  mt->ioProxy = ip;
  mt->command = '0';
  mt->responseSize = 2;
  
  unsigned int i = 0;
  for (i=0; i< 100; i++) {
    cout << endl << endl;
    cout << i;
    
    mt->fetch();
  }
  
  
  
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
