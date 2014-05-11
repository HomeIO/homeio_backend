MeasType::MeasType() {
  buffer = new MeasBuffer;
}

unsigned int MeasType::fetch() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);
  lastTime = time(NULL);
  
  cout << name << " - fetched raw " << raw << " (offset " << buffer->offset << ", count " << buffer->count << ")" << endl;
  
  return raw;
}


unsigned int MeasType::addRaw(unsigned int _raw) {
  buffer->add(_raw);
  return 0;
}
