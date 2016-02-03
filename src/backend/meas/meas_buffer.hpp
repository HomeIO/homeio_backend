#ifndef MEAS_BUFFER
#define	MEAS_BUFFER

#include <math.h>
#include <vector>
#include <string>
#include <memory>

#include "meas_definitions.hpp"
#include "../utils/helper.hpp"
#include "../log/log_array.hpp"

class MeasBuffer {
 private:
  meas_buffer_index index(meas_buffer_index i);
  meas_buffer_index responseIndexInterval;
  std::vector < meas_buffer_element > buffer;

 public:
  MeasBuffer();
  MeasBuffer(meas_buffer_index _maxSize);
  void clearAndResize(meas_buffer_index _maxSize);
  meas_buffer_index add(meas_buffer_element raw);
  meas_buffer_element at(meas_buffer_index i);
  meas_buffer_element last();

  unsigned long int memorySize();
  bool stored(meas_buffer_index i);

  // extension - remove one time spikes
  bool removeSpikes;
  // detect if b is spike near a and c
  bool isSpike(meas_buffer_element a, meas_buffer_element b, meas_buffer_element c);
  // detect if last stored measuremt is spike
  // it can only be done one measurement after that
  bool wasSpike(meas_buffer_element latestRaw);
  // iterate buffer and filter spikes
  void filterStoredSpikes();

  meas_buffer_index tempResponseIndexInterval();
  meas_buffer_index calculateIndexInterval(meas_buffer_index lower, meas_buffer_index higher, meas_buffer_index responseMaxSize);
  std::vector < unsigned int > getFromBuffer(meas_buffer_index from, meas_buffer_index to, meas_buffer_index responseMaxSize);

  std::string jsonArray(meas_buffer_index from, meas_buffer_index to, meas_buffer_index responseMaxSize);
  std::string toJson();

  meas_buffer_index maxSize;
  // position of current raw
  meas_buffer_index offset;
  // how many raws were added
  meas_buffer_index count;

  unsigned char elementSize;

  meas_buffer_index calcInterval();
  meas_time lastTime, firstTime, lastTimeForCount;
  // it's not very accurate
  meas_time earliestTime();

  std::shared_ptr<LogArray> logArray;

};

#endif
