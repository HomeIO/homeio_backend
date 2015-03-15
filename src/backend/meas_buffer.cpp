MeasBuffer::MeasBuffer(unsigned long int _maxSize = 1000000) {
  maxSize = _maxSize;
  offset = 0;
  count = 0;
  firstTime = 0;
  elementSize = sizeof(_maxSize);
  
  buffer.resize(maxSize);
}

unsigned long int MeasBuffer::add(unsigned int raw) {
  offset++;
  if (offset >= maxSize) {
    offset = 0;
  }
  if (offset > count) {
    count = offset;
    lastTimeForCount = time(NULL);
  }
  if (firstTime == 0) {
    firstTime = time(NULL);
  }
  lastTime = time(NULL);
  
  buffer[offset] = raw;
  
  return offset;
}

float MeasBuffer::calcInterval() {
  if (offset == 0) {
    return 1.0; // not to fuck up all this shit somewhere
  } else {
    unsigned long int timeCount = (unsigned long int) lastTimeForCount - (unsigned long int) firstTime;
    return (float) timeCount / (float) count;
  }
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

unsigned long int MeasBuffer::memorySize() {
  return buffer.capacity() * elementSize;
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