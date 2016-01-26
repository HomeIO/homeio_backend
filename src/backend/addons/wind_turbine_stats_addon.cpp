#include "wind_turbine_stats_addon.hpp"

#define NC_WS_ADDON_NAME 0
#define NC_WS_ADDON_VALUE 23
#define NC_WS_ADDON_PREV 45

WindTurbineStatsAddon::WindTurbineStatsAddon() {
  name = "WindTurbineStats";
  lastTime = 0;
  coilThresholdVoltage = 5.0;
  batteryThresholdCurrent = 0.5;

  path = "stats";
}

void WindTurbineStatsAddon::setup() {
  // create path at start, no wait
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
}

void WindTurbineStatsAddon::perform() {
  // first run
  // so it won't calculate and write
  if (lastTime == 0) {
    lastTime = calculateTimeFrom();
  }

  if (calculateTimeFrom() > lastTime) {
    // calculate now

    lastTime = calculateTimeFrom();
    s = calculateStats(lastTime);
    store();
  }
}

unsigned long long WindTurbineStatsAddon::calculateTimeFrom() {
  unsigned long long t = Helper::Helper::mTime();
  t = t - hour; // calculate for previous hour only
  t = t - (t % hour);
  return t;
}

WindTurbineStat WindTurbineStatsAddon::calculateStats(unsigned long long t) {
  prevS = s;

  s.time = t;
  s.timeLength = hour;

  unsigned long j = 0;

  double w = 0.0;

  // temp
  double doubleTmp = 0.0;
  double intervalDouble = 0.0;
  unsigned long int intervalInt = 0;
  unsigned long int rawSize = 0;

  unsigned long int coilTime = 0;
  unsigned long int battCurrentTime = 0;
  unsigned long int resistorTime = 0;
  double maxBattCurrent = 0.0;
  double maxCoilVoltage = 0.0;
  double maxBattVoltage = 0.0;

  // work
  MeasType *u = measTypeArray->byName(measNameU);
  MeasType *i = measTypeArray->byName(measNameI);

  std::vector < unsigned int > uRaw = u->buffer->getFromBuffer(u->timeToIndex(t), u->timeToIndex(t + hour), 0);
  std::vector < unsigned int > iRaw = i->buffer->getFromBuffer(i->timeToIndex(t), i->timeToIndex(t + hour), 0);

  intervalDouble = (double) u->buffer->calcInterval();
  intervalInt = i->buffer->calcInterval();

  // take lesser size
  rawSize = uRaw.size();
  if (iRaw.size() < rawSize) {
    rawSize = iRaw.size();
  }

  for (j = 0; j < rawSize; j++ ) {
    // work
    // V*A*s
    // you need to divide 3600*1000 if you want to have kWh
    doubleTmp = ( u->rawToValue( uRaw.at(j) ) ) * ( i->rawToValue( iRaw.at(j) ) ) * intervalDouble / 1000.0;
    if (doubleTmp > 0.0) {
      w += doubleTmp;
    }

    // max battery voltage
    doubleTmp = u->rawToValue( uRaw.at(j) );
    if (maxBattVoltage < doubleTmp) {
      maxBattVoltage = doubleTmp;
    }

    // battery current time
    doubleTmp = i->rawToValue( iRaw.at(j) );
    if (doubleTmp >= batteryThresholdCurrent ) {
      battCurrentTime += intervalInt;
    }

    // max battery current
    if (maxBattCurrent < doubleTmp) {
      maxBattCurrent = doubleTmp;
    }
  }
  s.work = w;

  // coil time
  MeasType *coil = measTypeArray->byName(measCoil);
  std::vector < unsigned int > coilRaw = coil->buffer->getFromBuffer(coil->timeToIndex(t), coil->timeToIndex(t + hour), 0);
  intervalInt = coil->buffer->calcInterval();

  for (j = 0; j < coilRaw.size(); j++ ) {
    doubleTmp = coil->rawToValue( coilRaw.at(j) );

    // coil time
    if (doubleTmp >= coilThresholdVoltage ) {
      coilTime += intervalInt;
    }

    // max coil voltage
    if (maxCoilVoltage < doubleTmp) {
      maxCoilVoltage = doubleTmp;
    }
  }

  // resistor time
  MeasType *resistor = measTypeArray->byName(measResistor);
  std::vector < unsigned int > resistorRaw = resistor->buffer->getFromBuffer(resistor->timeToIndex(t), resistor->timeToIndex(t + hour), 0);
  intervalInt = resistor->buffer->calcInterval();

  for (j = 0; j < resistorRaw.size(); j++ ) {
    if ( resistorRaw.at(j) > 0 ) {
      resistorTime += intervalInt;
    }
  }

  s.coilTime = coilTime;
  s.battCurrentTime = battCurrentTime;
  s.resistorTime = resistorTime;
  s.maxBattCurrent = maxBattCurrent;
  s.maxCoilVoltage = maxCoilVoltage;
  s.maxBattVoltage = maxBattVoltage;

  return s;
}

void WindTurbineStatsAddon::store() {
  std::ofstream outfile;
  std::string currentDate = Helper::currentDateSafe();
  std::string filename = path + "/wind_turbine_stats_" + currentDate + ".csv";

  logArray->log("WindTurbineStats", "store path " + filename);

  outfile.open(filename, std::ios_base::app);
  outfile << s.time << "; ";
  outfile << s.work << "; ";
  outfile << s.timeLength << "; ";
  outfile << s.coilTime << "; ";
  outfile << s.battCurrentTime << "; ";
  outfile << s.resistorTime << "; ";
  outfile << s.maxBattCurrent << "; ";
  outfile << s.maxBattVoltage << "; ";
  outfile << s.maxCoilVoltage << "; ";
  outfile << std::endl;
  outfile.close();

  logArray->log("WindTurbineStats", "stored");
}

void WindTurbineStatsAddon::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, "Wind Turbine Addon" );
  wattroff(window, NC_COLOR_PAIR_NAME_SET);

  int i = 3;

  // time
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "time" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.time).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.time).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);

  i++;

  // work time
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "work" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.work).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.work).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // timeLength
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "timeLength" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.timeLength).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.timeLength).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // coilTime
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "coilTime" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.coilTime).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.coilTime).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // battCurrentTime
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "battCurrentTime" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.battCurrentTime).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.battCurrentTime).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // resistorTime
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "resistorTime" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.resistorTime).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.resistorTime).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // maxBattCurrent
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "maxBattCurrent" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.maxBattCurrent).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.maxBattCurrent).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // maxBattVoltage
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "maxBattVoltage" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.maxBattVoltage).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.maxBattVoltage).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


  i++;

  // maxBattVoltage
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_NAME, "maxCoilVoltage" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_VALUE, std::to_string(s.maxCoilVoltage).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_SET);

  wattron(window, NC_COLOR_PAIR_VALUE_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_PREV, std::to_string(prevS.maxCoilVoltage).c_str() );
  wattroff(window, NC_COLOR_PAIR_VALUE_LESSER_SET);


}
