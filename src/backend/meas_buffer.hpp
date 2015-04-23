#ifndef MEAS_BUFFER
#define	MEAS_BUFFER

#include <math.h>

using namespace std;

class MeasBuffer {
public:
  MeasBuffer(unsigned long int _maxSize);
  
  unsigned long int add(unsigned int raw);
  unsigned int at(unsigned long int i);
  unsigned long int index(unsigned long int i);
  unsigned long int memorySize();
  bool stored(unsigned long int i);
  
  unsigned long int responseIndexInterval;
  unsigned long int calculateIndexInterval(unsigned long int lower, unsigned long int higher, unsigned long int responseMaxSize);
  vector < unsigned int > getFromBuffer(unsigned long int from, unsigned long int to, unsigned long int responseMaxSize);
  
  string jsonArray(unsigned long int from, unsigned long int to, unsigned long int responseMaxSize);
  string toJson();
  
  unsigned long int maxSize;
  unsigned long int offset;
  unsigned long int count;
  
  unsigned char elementSize;

  unsigned long int calcInterval();
  unsigned long long lastTime, firstTime, lastTimeForCount;
  
private:
  vector < unsigned int > buffer;
  
};

#include "meas_buffer.cpp"
#endif
