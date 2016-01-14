#ifndef MEAS_BUFFER
#define	MEAS_BUFFER

#define meas_buffer_elemt unsigned int

#include <math.h>
#include <vector>
#include <string>
#include "../utils/helper.hpp"
#include "../log/log_array.hpp"

class MeasBuffer {
public:
  MeasBuffer();
  MeasBuffer(unsigned long int _maxSize);

  void clearAndResize(unsigned long int _maxSize);
  unsigned long int add(unsigned int raw);
  unsigned int at(unsigned long int i);
  unsigned int last();
  unsigned long int index(unsigned long int i);
  unsigned long int memorySize();
  bool stored(unsigned long int i);

  // extension - remove one time spikes
  bool removeSpikes;
  // detect if b is spike near a and c
  bool isSpike(unsigned int a, unsigned int b, unsigned int c);
  // detect if last stored measuremt is spike
  // it can only be done one measurement after that
  bool wasSpike(unsigned int latestRaw);
  // iterate buffer and filter spikes
  void filterStoredSpikes();



  unsigned long int responseIndexInterval;
  unsigned long int calculateIndexInterval(unsigned long int lower, unsigned long int higher, unsigned long int responseMaxSize);
  std::vector < unsigned int > getFromBuffer(unsigned long int from, unsigned long int to, unsigned long int responseMaxSize);

  std::string jsonArray(unsigned long int from, unsigned long int to, unsigned long int responseMaxSize);
  std::string toJson();

  unsigned long int maxSize;
  // position of current raw
  unsigned long int offset;
  // how many raws were added
  unsigned long int count;

  unsigned char elementSize;

  unsigned long int calcInterval();
  unsigned long long lastTime, firstTime, lastTimeForCount;
  // it's not very accurate
  unsigned long long earliestTime();

  LogArray *logArray;

private:
  std::vector < meas_buffer_elemt > buffer;

};

//#include "meas_buffer.cpp"
#endif
