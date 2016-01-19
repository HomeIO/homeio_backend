#ifndef FRONTEND_SETTINGS_H
#define FRONTEND_SETTINGS_H

class FrontendSettings {
 public:
  FrontendSettings();

  unsigned long int intervalCurrent;
  unsigned long int intervalHistory;
  double currentCoeff;
};

//#include "frontend_settings.cpp"
#endif
