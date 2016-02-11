#include "wind_turbine_stats_addon.hpp"

WindTurbineStatsAddon::WindTurbineStatsAddon() {
  name = "wind_turbine_stats";
  lastTime = 0;
  coilThresholdVoltage = 5.0;
  batteryThresholdCurrent = 0.5;

  path = "stats";

  bufferMax = 40;
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
    repopulateFromBuffer();
  }

  if (calculateTimeFrom() > lastTime) {
    // calculate now

    lastTime = calculateTimeFrom();
    std::shared_ptr<WindTurbineStat> wts = calculateStats(lastTime);
    addToBuffer(wts);
    store(wts);
  }
}

void WindTurbineStatsAddon::repopulateFromBuffer() {
  meas_time t = lastTime - (meas_time)(bufferMax) * hour;

  while (t < lastTime) {
    std::shared_ptr<WindTurbineStat> wts = calculateStats(t);
    addToBuffer(wts);

    t += hour;
  }
}

void WindTurbineStatsAddon::addToBuffer(std::shared_ptr<WindTurbineStat> wts) {
  if ( (unsigned char) bufferStat.size() >= (unsigned char) bufferMax ) {
    bufferStat.erase( bufferStat.begin() );
  }
  bufferStat.push_back(wts);
}

unsigned long long WindTurbineStatsAddon::calculateTimeFrom() {
  unsigned long long t = Helper::Helper::mTime();
  t = t - hour; // calculate for previous hour only
  t = t - (t % hour);
  return t;
}

std::shared_ptr<WindTurbineStat> WindTurbineStatsAddon::calculateStats(unsigned long long t) {
  std::shared_ptr<WindTurbineStat> s = std::make_shared<WindTurbineStat>();

  s->time = t;
  s->timeLength = hour;

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
  std::shared_ptr<MeasType> u = measTypeArray->byName(measNameU);
  std::shared_ptr<MeasType> i = measTypeArray->byName(measNameI);

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
  s->work = w;

  // coil time
  std::shared_ptr<MeasType> coil = measTypeArray->byName(measCoil);
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
  std::shared_ptr<MeasType> resistor = measTypeArray->byName(measResistor);
  std::vector < unsigned int > resistorRaw = resistor->buffer->getFromBuffer(resistor->timeToIndex(t), resistor->timeToIndex(t + hour), 0);
  intervalInt = resistor->buffer->calcInterval();

  for (j = 0; j < resistorRaw.size(); j++ ) {
    if ( resistorRaw.at(j) > 0 ) {
      resistorTime += intervalInt;
    }
  }

  s->coilTime = coilTime;
  s->battCurrentTime = battCurrentTime;
  s->resistorTime = resistorTime;
  s->maxBattCurrent = maxBattCurrent;
  s->maxCoilVoltage = maxCoilVoltage;
  s->maxBattVoltage = maxBattVoltage;

  return s;
}

void WindTurbineStatsAddon::store(std::shared_ptr<WindTurbineStat> s) {
  std::ofstream outfile;
  std::string currentDate = Helper::currentDateSafe();
  std::string filename = path + "/wind_turbine_stats_" + currentDate + ".csv";

  logArray->log("WindTurbineStats", "store path " + filename);

  outfile.open(filename, std::ios_base::app);
  outfile << s->time << "; ";
  outfile << s->work << "; ";
  outfile << s->timeLength << "; ";
  outfile << s->coilTime << "; ";
  outfile << s->battCurrentTime << "; ";
  outfile << s->resistorTime << "; ";
  outfile << s->maxBattCurrent << "; ";
  outfile << s->maxBattVoltage << "; ";
  outfile << s->maxCoilVoltage << "; ";
  outfile << std::endl;
  outfile.close();

  logArray->log("WindTurbineStats", "stored");
}

#define NC_WS_ADDON_TIME 0
#define NC_WS_ADDON_WORK NC_WS_ADDON_TIME + 11
#define NC_WS_ADDON_COIL_TIME NC_WS_ADDON_WORK + 13
#define NC_WS_ADDON_BATT_TIME NC_WS_ADDON_COIL_TIME + 11
#define NC_WS_ADDON_RES_TIME NC_WS_ADDON_BATT_TIME + 11
#define NC_WS_ADDON_MAX_BATT_CURRENT NC_WS_ADDON_RES_TIME + 11
#define NC_WS_ADDON_MAX_BATT_VOLTAGE NC_WS_ADDON_MAX_BATT_CURRENT + 12
#define NC_WS_ADDON_MAX_COIL_VOLTAGE NC_WS_ADDON_MAX_BATT_VOLTAGE + 12

#define NS_WS_ADDON_COLOR_HIGH NC_COLOR_PAIR_ERROR_SET
#define NS_WS_ADDON_COLOR_MED NC_COLOR_PAIR_NAME_SET
#define NS_WS_ADDON_COLOR_LOW NC_COLOR_PAIR_VALUE_SET

void WindTurbineStatsAddon::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, "Wind Turbine Stats Addon" );
  wattroff(window, NC_COLOR_PAIR_NAME_SET);

  int i = 3;

  // time
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_WS_ADDON_TIME, "time" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_WORK, "work" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_COIL_TIME, "coil time" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_BATT_TIME, "batt time" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_RES_TIME, "res time" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_MAX_BATT_CURRENT, "max b cur" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_MAX_BATT_VOLTAGE, "max b v" );
  mvwprintw(window, i, 1 + NC_WS_ADDON_MAX_COIL_VOLTAGE, "max coil v" );

  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  if (bufferStat.size() == 0) {
    return;
  }

  for (int j = 0; j < (unsigned char) bufferStat.size(); j++) {
    int iRow = i + 1 + j;

    if (iRow >= (LINES - 3)) {
      break;
    }

    std::shared_ptr<WindTurbineStat> s = bufferStat.at(bufferStat.size() - 1 - j);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_TIME, Helper::timeToTimeString(s->time).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    float w = s->work / 3600.0;
    if (w > 100.0) {
      wattron(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (w > 30.0) {
      wattron(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattron(window, NS_WS_ADDON_COLOR_LOW);
    }
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_WORK, MeasType::formattedValue(w, "Wh").c_str() );
    if (w > 100.0) {
      wattroff(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (w > 30.0) {
      wattron(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattroff(window, NS_WS_ADDON_COLOR_LOW);
    }


    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_COIL_TIME, Helper::intervalToString(s->coilTime).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_BATT_TIME, Helper::intervalToString(s->battCurrentTime).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_RES_TIME, Helper::intervalToString(s->resistorTime).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    if (s->maxBattCurrent > 5.0) {
      wattron(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (s->maxBattCurrent > 2.0) {
      wattron(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattron(window, NS_WS_ADDON_COLOR_LOW);
    }
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_MAX_BATT_CURRENT, MeasType::formattedValue(s->maxBattCurrent, "A").c_str() );
    if (s->maxBattCurrent > 5.0) {
      wattroff(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (s->maxBattCurrent > 2.0) {
      wattroff(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattroff(window, NS_WS_ADDON_COLOR_LOW);
    }

    if (s->maxBattVoltage > 37.0) {
      wattron(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (s->maxBattVoltage > 34.0) {
      wattron(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattron(window, NS_WS_ADDON_COLOR_LOW);
    }
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_MAX_BATT_VOLTAGE, MeasType::formattedValue(s->maxBattVoltage, "V").c_str() );
    if (s->maxBattVoltage > 37.0) {
      wattroff(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (s->maxBattVoltage > 34.0) {
      wattroff(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattroff(window, NS_WS_ADDON_COLOR_LOW);
    }

    if (s->maxCoilVoltage > 28.0) {
      wattron(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (s->maxCoilVoltage > 24.0) {
      wattron(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattron(window, NS_WS_ADDON_COLOR_LOW);
    }
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_MAX_COIL_VOLTAGE, MeasType::formattedValue(s->maxCoilVoltage, "V").c_str() );
    if (s->maxCoilVoltage > 28.0) {
      wattroff(window, NS_WS_ADDON_COLOR_HIGH);
    } else if (s->maxCoilVoltage > 24.0) {
      wattroff(window, NS_WS_ADDON_COLOR_MED);
    } else {
      wattroff(window, NS_WS_ADDON_COLOR_LOW);
    }
  }

}

std::string WindTurbineStatsAddon::toJson() {
  std::string json = "{\"data\": [";
  std::string itemJson;

  for (int j = 0; j < (unsigned char) bufferStat.size(); j++) {
    std::shared_ptr<WindTurbineStat> wst = bufferStat.at(bufferStat.size() - 1 - j);

    itemJson = "{";
    itemJson += "\"time\": " + std::to_string(wst->time);
    itemJson += ", \"work\": " + std::to_string(wst->work);
    itemJson += ", \"timeLength\": " + std::to_string(wst->timeLength);
    itemJson += ", \"coilTime\": " + std::to_string(wst->coilTime);
    itemJson += ", \"battCurrentTime\": " + std::to_string(wst->battCurrentTime);
    itemJson += ", \"resistorTime\": " + std::to_string(wst->resistorTime);
    itemJson += ", \"maxBattCurrent\": " + std::to_string(wst->maxBattCurrent);
    itemJson += ", \"maxBattVoltage\": " + std::to_string(wst->maxBattVoltage);
    itemJson += ", \"maxCoilVoltage\": " + std::to_string(wst->maxCoilVoltage);
    itemJson += ", \"work\": " + std::to_string(wst->work);

    itemJson += "}";
    itemJson += ",";

    json += itemJson;
  }

  // remove last coma
  if (json[json.size() - 1] == ',') {
    json.resize(json.size() - 1);
  }

  json += "]}";
  return json;
}
