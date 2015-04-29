class MeasTrend {
public: 
  static const int typeStable = 0;
  static const int typeAscend = 10;
  static const int typeDescend = 20;
  
  unsigned char type;
  
  double valueFrom;
  double valueTo;
  
  unsigned int rawFrom;
  unsigned int rawTo;
  
  unsigned long long timeFrom;
  unsigned long long timeTo;
  
  unsigned long int indexFrom;
  unsigned long int indexTo;
} ;