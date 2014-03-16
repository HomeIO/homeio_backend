//#include "meas_type.hpp"
//#include "io_proxy.hpp"

HomeIO::HomeIO() {
  ioProxy = new IoProxy();
  

}

int HomeIO::start() {
  ioProxy->prepareSocket();
  
  for(std::vector<MeasType>::iterator m = measTypes.begin(); m != measTypes.end(); ++m) {
    m->ioProxy = ioProxy;
  }  
  
  for(std::vector<MeasType>::iterator m = measTypes.begin(); m != measTypes.end(); ++m) {
    m->fetch();
  }  
}