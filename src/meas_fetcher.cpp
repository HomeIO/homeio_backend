MeasFetcher::MeasFetcher() {
}

void MeasFetcher::start() {
  ioProxy->prepareSocket();
  
  for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
    m->ioProxy = ioProxy;
  }
  
  unsigned int i;
  for (i=0; i<1; i++) {
    for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
      m->fetch();
    } 

    usleep(50000);
  }
}
