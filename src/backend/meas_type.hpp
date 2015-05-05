#ifndef MEAS_TYPE_H
#define	MEAS_TYPE_H

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <iomanip>
#include <vector>
#include <time.h>

#include "io_proxy.hpp"
#include "meas_buffer.hpp"
#include "meas_type_storage.hpp"
#include "meas_trend.hpp"

using namespace std;

class MeasType {
public:
  MeasType();
  unsigned int fetch();
  unsigned int addRaw(unsigned int);
  double rawToValue(unsigned int);
  double valueAt(unsigned long int i);
  unsigned int lastRaw();
  double lastValue();
  double lastValueFor(unsigned long int);
  string toJson();
  string rawForTimeJson(unsigned long long timeFrom, unsigned long long timeTo);
  string rawForIndexJson(unsigned long int indexFrom, unsigned long long indexTo);
  string rawHistoryForTimeJson(unsigned long long timeFrom, unsigned long long timeTo, unsigned long int responseMaxSize);
  void logInfo(string log); // overwritten color
  
  unsigned long int minTimeDiffToStore;
  unsigned long int maxTimeDiffToStore;
  double valueDiffToStore;

  void recalculateAvg(); 
  double currentAvgValue(); // recalculate if needed and get stored
  double avgValue; // calculated periodically to show value deviation from standard value
  unsigned long long lastAvgTime; // when last was execute
  unsigned long long intervalAvg; // how often refresh
  unsigned long int windowAvg; // how many meas use to calculate avg

  
  void prepareFetch();
  string fetchString(unsigned int raw);

  MeasBuffer *buffer;
  
  string name; // name of measurements
  string unit; // value is presented in unit
  unsigned char priority; // 0 - low priority, 1 - high (at this moment), used only in frontend
  char command; // at this moment only 1 byte commands
  char responseSize; // amount of bytes returned from uC, normally 1 or 2
  
  double coefficientLinear;
  int coefficientOffset;
  
  bool started;
  string logPrefix;
  
  IoProxy *ioProxy;
  MeasTypeStorage *measTypeStorage;

  unsigned long int timeToIndex(unsigned long long t);
  vector < StorageHash > prepareStorage(unsigned long long timeFrom, unsigned long long timeTo);
  vector < StorageHash > storageArray(unsigned long long timeFrom, unsigned long long timeTo);
  string storageJson(unsigned long long timeFrom, unsigned long long timeTo);
  unsigned long long lastStored;

  // stats
  string statsJson(unsigned long long timeFrom, unsigned long long timeTo);
  vector < MeasTrend > getTrendsBetween(unsigned long long timeFrom, unsigned long long timeTo);
	
};

#include "meas_type.cpp"
#include "meas_type_stats.cpp"
#endif	
