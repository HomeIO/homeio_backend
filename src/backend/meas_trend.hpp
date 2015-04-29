class MeasTrend {
public: 
  // 0 - stable
  // 10 - ascend
  // 20 - descend
  unsigned char type;
  
  double valueFrom;
  double valueTo;
  
  unsigned long long timeFrom;
  unsigned long long timeTo;
} ;