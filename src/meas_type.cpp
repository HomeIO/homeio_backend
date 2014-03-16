MeasType::MeasType() {
  bufferSize = 1000000;
}

unsigned int MeasType::fetch() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);
  
  cout << name << " - fetched raw " << raw << " (buffer size " << buffer.size() << ")" << endl;
  
  return raw;
}

unsigned int MeasType::assignBufferSize(unsigned int _bufferSize) {
  buffer.reserve(_bufferSize);
  
  cout << " size " << buffer.size();
  cout << " max_size " << buffer.max_size();
  cout << " size_of " << sizeof(buffer);
  cout << endl;
  
  return buffer.max_size();
}

unsigned int MeasType::addRaw(unsigned int _raw) {
  buffer.push_back(_raw);
  
  if (buffer.size() > bufferSize) {
    buffer.erase( buffer.begin() );
  }
  
  return 0;
}
