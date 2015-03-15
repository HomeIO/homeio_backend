MeasType::MeasType() {
  buffer = new MeasBuffer;
}

unsigned int MeasType::fetch() {
  unsigned int raw = ioProxy->fetch(command, responseSize);
  addRaw(raw);
  
  cout << "MeasBuffer [" << name << "] raw\t" << raw << "\t[offset=" << buffer->offset << ",count=" << buffer->count << ",memory=" << buffer->memorySize() << "]" << endl;
  
  return raw;
}


unsigned int MeasType::addRaw(unsigned int _raw) {
  buffer->add(_raw);
  return 0;
}
