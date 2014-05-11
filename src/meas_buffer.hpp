#ifndef MEAS_BUFFER
#define	MEAS_BUFFER

using namespace std;

class MeasBuffer {
public:
  MeasBuffer(unsigned long int _maxSize);
  
  unsigned long int add(unsigned int raw);
  unsigned int at(unsigned long int i);
  unsigned long int index(unsigned long int i);
  bool stored(unsigned long int i);
  string jsonArray(unsigned long int from, unsigned long int to);
  
  unsigned long int maxSize;
  unsigned long int offset;
  unsigned long int count;
  
private:
  std::vector < unsigned int > buffer;
  
};

#include "meas_buffer.cpp"
#endif
