#ifndef MEAS_TREND_H
#define MEAS_TREND_H

using namespace std;

class MeasTrend {
public:
  static const int typeStable = 0;
  static const int typeAscend = 10;
  static const int typeDescend = 20;

  unsigned char tmpType;

  double valueFrom;
  double valueTo;

  unsigned int rawFrom;
  unsigned int rawTo;

  unsigned long long timeFrom;
  unsigned long long timeTo;

  unsigned long int indexFrom;
  unsigned long int indexTo;

  MeasTrend();
  unsigned long long timeDiff();
  double valueDiff();
  double trend();
  unsigned char type();
};

//#include "meas_trend.cpp"
#endif
