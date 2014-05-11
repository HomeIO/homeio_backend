MeasBuffer::MeasBuffer(unsigned long int _maxSize = 1000000) {
  maxSize = _maxSize;
  offset = 0;
  count = 0;
  
  buffer.resize(maxSize);
}

unsigned long int MeasBuffer::add(unsigned int raw) {
  offset++;
  if (offset >= maxSize) {
    offset = 0;
  }
  if (offset > count) {
    count = offset;
  }
  
  buffer[offset] = raw;
  return offset;
}

unsigned int MeasBuffer::at(unsigned long int i) {
  return buffer[index(i)];
}

unsigned long int MeasBuffer::index(unsigned long int i) {
  long int tmpI = offset - i;
  if (tmpI >= 0) {
    return tmpI;
  } else {
    return maxSize + offset - i;
  }
}

bool MeasBuffer::stored(unsigned long int i) {
  if (i>count) {
    return false;
  } else {
    return true;
  }
}

string MeasBuffer::jsonArray(unsigned long int from, unsigned long int to) {
  string s = "[";
  
//  cout << endl;
//  cout << "from " << from << endl;
//  cout << "to " << to << endl;
//  cout << "offset " << offset << endl;
//  cout << "count " << count << endl;
  
  unsigned long int i;
  for (i = from; i <= to; i++) {
    if (stored(i)) {
      s += to_string(at(i));
    } else {
      s += "null";
    }
    s += ",";
  }
  
  s += "null]";
  
  return s;
}