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

  MeasType *mu = new MeasType("t_test");
  mu->ioProxy = ip;
  mu->command = 't';
  mu->responseSize = 2;
  
  MeasType *mv = new MeasType("s_test");
  mv->ioProxy = ip;
  mv->command = 's';
  mv->responseSize = 1;
  
  unsigned int i = 0;
  for (i=0; i< 100; i++) {
    cout << "fetch loop i = " << i << endl;
    
    mt->fetch();
    mu->fetch();
    mv->fetch();
  }
  
  return 0;
}
