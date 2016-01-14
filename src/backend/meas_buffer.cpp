#include "meas_buffer.hpp"

MeasBuffer::MeasBuffer() {
  maxSize = 1000000;
  elementSize = sizeof(meas_buffer_elemt);
  clearAndResize(maxSize);
  removeSpikes = false;
}

MeasBuffer::MeasBuffer(unsigned long int _maxSize) {
  maxSize = _maxSize;
  elementSize = sizeof(meas_buffer_elemt);
  clearAndResize(maxSize);
  removeSpikes = false;
}

void MeasBuffer::clearAndResize(unsigned long int _maxSize) {
  maxSize = _maxSize;

  offset = 0;
  count = 0;
  firstTime = 0;
  responseIndexInterval = 1;

  buffer.clear();
  buffer.reserve(maxSize);
  buffer.resize(maxSize);
  // http://www.cplusplus.com/reference/vector/vector/reserve/
}

unsigned long int MeasBuffer::add(unsigned int raw) {
  // special algorithm to remove spikes
  // update spike with raw before
  if ( (removeSpikes) && (wasSpike(raw)) ) {
    buffer[offset] = at(1);
  }

  // must be here because in other case first measurement is equal 0
  if (count == 0) {
    buffer[offset] = raw;
  }

  offset++;
  if (offset >= maxSize) {
    offset = 0;
  }
  if (offset > count) {
    count = offset;
    lastTimeForCount = Helper::mTime();
  }
  if (firstTime == 0) {
    firstTime = Helper::mTime();
  }
  lastTime = Helper::mTime();

  buffer[offset] = raw;

  return offset;
}



unsigned long int MeasBuffer::calcInterval() {
  if (offset == 0) {
    return 1; // not to fuck up all this shit somewhere
  } else {
    unsigned long long timeCount = lastTimeForCount - firstTime;
    return timeCount / count;
  }
}

unsigned long long MeasBuffer::earliestTime() {
  return lastTime - ((unsigned long long) calcInterval() * (unsigned long long) count);
}

// detect if b is spike near a and c
bool MeasBuffer::isSpike(unsigned int a, unsigned int b, unsigned int c) {
  int absA = abs( (int) a - (int) c );
  int absB = abs( (int) a - (int) b );

  if (absB > 10 * (absA + 1) ) {
    logArray->log("MeasBuffer", "found SPIKE " + std::to_string(a) + " - " + std::to_string(b) + " - " + std::to_string(c) );
    return true;
  } else {
    return false;
  }

}

bool MeasBuffer::wasSpike(unsigned int latestRaw) {
  // not enough data
  if (count <= 3) {
    return false;
  }

  return isSpike(at(1), at(0), latestRaw);
}

void MeasBuffer::filterStoredSpikes() {
  // not enough data
  if (count <= 3) {
    return;
  }

  unsigned long int i;

  // iterate buffer
  for (i = 1; i < (count - 1); i++) {
    // check if spike within closest raws
    if ( isSpike( at(i+1), at(i), at(i-1)) ) {
      // overwrite
      buffer[index(i)] = buffer[index(i+1)];
    }
  }
}

unsigned int MeasBuffer::at(unsigned long int i) {
  return buffer[index(i)];
}

unsigned int MeasBuffer::last() {
  return at(0);
}

unsigned long int MeasBuffer::index(unsigned long int i) {
  long int tmpI = (long int) offset - (long int) i;
  if (tmpI >= 0) {
    return (unsigned long int) tmpI;
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

unsigned long int MeasBuffer::calculateIndexInterval(unsigned long int lower, unsigned long int higher, unsigned long int responseMaxSize) {
  if (responseMaxSize == 0) {
    return 1;
  }

  unsigned long int distance = higher - lower;

  if (distance <= responseMaxSize) {
    return 1;
  } else {
    return (unsigned long int) ceil((double) distance / (double) responseMaxSize);
  }
}

std::vector < unsigned int > MeasBuffer::getFromBuffer(unsigned long int from, unsigned long int to, unsigned long int responseMaxSize) {
  unsigned long int i;
  std::vector < unsigned int > result;

  if (from <= to) {
    // safety
    if (to >= maxSize) {
      to = maxSize - 1;
    }

    logArray->log("MeasBuffer", "UP getFromBuffer(" + std::to_string(from) + ", " + std::to_string(to) + ", " + std::to_string(responseMaxSize) + ")");

    responseIndexInterval = calculateIndexInterval(from, to, responseMaxSize);
    for (i = from; i <= to; i += responseIndexInterval) {
      if (stored(i)) {
        result.push_back(at(i));
      }
    }
    return result;
  } else {
    // safety
    if (from >= maxSize) {
      from = maxSize - 1;
    }

    logArray->log("MeasBuffer", "DOWN getFromBuffer(" + std::to_string(from) + ", " + std::to_string(to) + ", " + std::to_string(responseMaxSize) + ")");

    responseIndexInterval = calculateIndexInterval(to, from, responseMaxSize);
    for (i = from; (i > to) && (i >= responseIndexInterval); i -= responseIndexInterval) {
      if (stored(i)) {
        result.push_back(at(i));
      }
    }

    return result;
  }
}

std::string MeasBuffer::jsonArray(unsigned long int from, unsigned long int to, unsigned long int responseMaxSize) {
  std::string s = "[";
  std::vector < unsigned int > tmp = getFromBuffer(from, to, responseMaxSize);
  for(std::vector<unsigned int>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    s += std::to_string(*it);
    s += ",";
  }

  // remove last coma
  if (s[s.size() - 1] == ',') {
    s.resize(s.size() - 1);
  }

  s += "]";

  return s;
}

std::string MeasBuffer::toJson() {
  std::string response;
  response = "{";
  response += "\"interval\":" + std::to_string(calcInterval()) + ",";
  response += "\"count\":" + std::to_string(count) + ",";
  response += "\"offset\":" + std::to_string(offset) + ",";
  response += "\"maxSize\":" + std::to_string(maxSize) + ",";
  response += "\"lastTime\":" + std::to_string(lastTime) + ",";
  response += "\"firstTime\":" + std::to_string(firstTime); //+ ",";
  //response += "\"earliestTime\":" + to_string(earliestTime());
  response += "}";
  return response;
}
