#ifndef MEAS_TYPE_H
#define	MEAS_TYPE_H

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <iomanip>
#include <vector>
#include <time.h>

#include "../io/io_proxy.hpp"
#include "meas_buffer.hpp"
#include "meas_type_storage.hpp"
#include "meas_trend.hpp"
#include "../log/log_array.hpp"

class MeasType {
public:
  MeasType();
  void resizeBuffer(unsigned long int);
  unsigned int fetch();
  unsigned int addRaw(unsigned int);
  double rawToValue(unsigned int);
  double valueAt(unsigned long int i);
  unsigned int lastRaw();
  double lastValue();
  double lastValueFor(unsigned long int);
  std::string toJson();
  std::string rawForTimeJson(unsigned long long timeFrom, unsigned long long timeTo);
  std::string rawForIndexJson(unsigned long int indexFrom, unsigned long long indexTo);
  std::string rawHistoryForTimeJson(unsigned long long timeFrom, unsigned long long timeTo, unsigned long int responseMaxSize);

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
  std::string fetchString(unsigned int raw);

  MeasBuffer *buffer;

  std::string name; // name of measurements
  std::string unit; // value is presented in unit
  unsigned int index; // from 0 up
  unsigned char priority; // 0 - low priority, 1 - high (at this moment), used only in frontend
  unsigned char command; // at this moment only 1 byte commands
  unsigned char responseSize; // amount of bytes returned from uC, normally 1 or 2

  double coefficientLinear;
  int coefficientOffset;

  // extensions - not needed, but can be used by external tools, like frontend
  // it's okay to ignore "spikes" - when raw is unnaturally diferent for only 1 measurement
  // change it to raw before the "spike"
  bool extRemoveSpikes;
  // not its something to remove them in backend
  bool extBackendRemoveSpikes;

  bool started;

  IoProxy *ioProxy;
  MeasTypeStorage *measTypeStorage;
  LogArray *logArray;

  unsigned long int timeToIndex(unsigned long long t);
  std::vector < StorageHash > prepareStorage(unsigned long long timeFrom, unsigned long long timeTo);
  std::vector < StorageHash > storageArray(unsigned long long timeFrom, unsigned long long timeTo);
  std::string storageJson(unsigned long long timeFrom, unsigned long long timeTo);
  unsigned long long lastStored;

  // stats
  std::string statsJson(unsigned long long timeFrom, unsigned long long timeTo);
  std::vector < MeasTrend > getTrendsBetween(unsigned long long timeFrom, unsigned long long timeTo);

};

//#include "meas_type.cpp"
#endif
