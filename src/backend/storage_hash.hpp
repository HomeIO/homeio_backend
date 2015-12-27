#ifndef STORAGE_HASH_H
#define	STORAGE_HASH_H

#include <string>

class StorageHash {
public:
  StorageHash(unsigned long long tF, unsigned long long tT, double v);

  unsigned long long timeFrom, timeTo;
  double value;

  string toJson();
};

//#include "storage_hash.cpp"
#endif
