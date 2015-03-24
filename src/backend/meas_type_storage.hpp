#ifndef MEAS_TYPE_STORAGE_H
#define	MEAS_TYPE_STORAGE_H

#include <time.h>
#include <string>

#include "meas_buffer.hpp"

class MeasTypeStorage {
public:
  MeasTypeStorage();
  
  MeasBuffer *measBuffer;
};

#include "meas_type_storage.cpp"
#endif	
