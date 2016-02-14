#include "wind_turbine_daily_stats_addon.hpp"

WindTurbineDailyStatsAddon::WindTurbineDailyStatsAddon() {
  name = "wind_turbine_daily_stats";
  lastTime = 0;
  coilThresholdVoltage = 5.0;
  batteryThresholdCurrent = 0.5;

  path = "stats";

  bufferMax = 60;
  interval = 5000; // TODO
}

void WindTurbineDailyStatsAddon::setup() {
  // create path at start, no wait
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
  restore();

  u = measTypeArray->byName(measNameU);
  i = measTypeArray->byName(measNameI);
  coil = measTypeArray->byName(measCoil);
  resistor = measTypeArray->byName(measResistor);
}

void WindTurbineDailyStatsAddon::stop() {
  dump();
}

std::string WindTurbineDailyStatsAddon::backupFilename() {
  return "stats/addon_backup" + name + ".txt";
}

void WindTurbineDailyStatsAddon::restore() {
  std::ifstream infile;
  infile.open(backupFilename(), std::ios_base::in);
  if (infile.good()) {
    int count;
    std::string line;

    getline(infile, line);
    count = std::atoi(line.c_str());
    logArray->log("WindTurbineDailyStatsAddon", "load backup: " + std::to_string(count));

    getline(infile, line);

    for (int i = 0; i < count; i++) {
      std::shared_ptr<WindTurbineStat> s = std::make_shared<WindTurbineStat>();
      getline(infile, line);
      s->fromCsv(line);
      addToBuffer(s);
    }

  }
  infile.close();
}

void WindTurbineDailyStatsAddon::dump() {
  std::ofstream outfile;
  outfile.open(backupFilename(), std::ios_base::out);
  outfile << bufferStat.size() << "\n";
  outfile << std::endl;

  for (unsigned int i = 0; i < bufferStat.size(); i++ ) {
    std::shared_ptr<WindTurbineStat> s = bufferStat[i];
    outfile << s->toCsv();
    outfile << std::endl;
  }

  outfile.close();
}

meas_time WindTurbineDailyStatsAddon::normalizeTime(meas_time t) {
  return t - (t % day);
}

void WindTurbineDailyStatsAddon::perform() {
  if (lastTime > ( Helper::mTime() - interval) ) {
    return;
  }

  meas_time initialTime = normalizeTime( i->buffer->earliestTime() );
  meas_time endTime = normalizeTime( Helper::mTime() ) + day;
  meas_time t = initialTime;

  logArray->log("WindTurbineDailyStatsAddon", "initialTime: " + std::to_string(initialTime));
  logArray->log("WindTurbineDailyStatsAddon", "endTime:" + std::to_string(endTime));

  if (bufferStat.size() == 0) {
    // repopulate
    while (t < endTime) {
      logArray->log("WindTurbineDailyStatsAddon", "repopulate: " + std::to_string(t));

      std::shared_ptr<WindTurbineStat> s = std::make_shared<WindTurbineStat>();
      s->time = t;
      updateStats(s);
      addToBuffer(s);

      t += day;
    }

  } else {
    // take last stat from buffer and update
    std::shared_ptr<WindTurbineStat> s = bufferStat[ bufferStat.size() - 1];
    t = s->time;
    updateStats(s);

    while ( (t + day) < endTime) {
      logArray->log("WindTurbineDailyStatsAddon", "append fresh: " + std::to_string(t));

      std::shared_ptr<WindTurbineStat> s = std::make_shared<WindTurbineStat>();
      s->time = t;
      updateStats(s);
      addToBuffer(s);

      t += day;
    }

  }

  lastTime = Helper::mTime();
}

void WindTurbineDailyStatsAddon::updateStats(std::shared_ptr<WindTurbineStat> s) {
  s->timeLength = day;

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
  std::vector < unsigned int > uRaw = u->buffer->getFromBuffer(u->timeToIndex(s->time), u->timeToIndex(s->time + day), 0);
  std::vector < unsigned int > iRaw = i->buffer->getFromBuffer(i->timeToIndex(s->time), i->timeToIndex(s->time + day), 0);

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

  std::vector < unsigned int > coilRaw = coil->buffer->getFromBuffer(coil->timeToIndex(s->time), coil->timeToIndex(s->time + day), 0);
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

  std::vector < unsigned int > resistorRaw = resistor->buffer->getFromBuffer(resistor->timeToIndex(s->time), resistor->timeToIndex(s->time + day), 0);
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
}

void WindTurbineDailyStatsAddon::addToBuffer(std::shared_ptr<WindTurbineStat> wts) {
  if ( (unsigned char) bufferStat.size() >= (unsigned char) bufferMax ) {
    bufferStat.erase( bufferStat.begin() );
  }
  bufferStat.push_back(wts);
}



void WindTurbineDailyStatsAddon::store(std::shared_ptr<WindTurbineStat> s) {
  std::ofstream outfile;
  std::string currentDate = Helper::currentDateSafe();
  std::string filename = path + "/wind_turbine_daily_stats.csv";

  logArray->log("WindTurbineDailyStatsAddon", "store path " + filename);

  outfile.open(filename, std::ios_base::app);
  outfile << s->toCsv();
  outfile << std::endl;
  outfile.close();

  logArray->log("WindTurbineDailyStatsAddon", "stored");
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

void WindTurbineDailyStatsAddon::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, "Wind Turbine Daily Stats Addon" );
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
    mvwprintw(window, iRow, 1 + NC_WS_ADDON_TIME, Helper::timeToDateString(s->time).c_str() );
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

std::string WindTurbineDailyStatsAddon::toJson() {
  std::string json = "{\"array\": [";

  for (int j = 0; j < (unsigned char) bufferStat.size(); j++) {
    std::shared_ptr<WindTurbineStat> wst = bufferStat.at(bufferStat.size() - 1 - j);
    json += wst->toJson();
    json += ",";
  }

  // remove last coma
  if (json[json.size() - 1] == ',') {
    json.resize(json.size() - 1);
  }

  std::string keyDesc = "[";
  keyDesc += "{\"key\": \"time\", \"type\": \"time\"},";
  keyDesc += "{\"key\": \"work\", \"type\": \"float\", \"unit\": \"Wh\", \"coeff\": 0.0002777777777777778},";
  keyDesc += "{\"key\": \"timeLength\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"coilTime\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"battCurrentTime\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"resistorTime\", \"type\": \"interval\"},";
  keyDesc += "{\"key\": \"maxBattCurrent\", \"type\": \"float\", \"unit\": \"A\"},";
  keyDesc += "{\"key\": \"maxBattVoltage\", \"type\": \"float\", \"unit\": \"V\"},";
  keyDesc += "{\"key\": \"maxCoilVoltage\", \"type\": \"float\", \"unit\": \"V\"}";
  keyDesc += "]";

  json += "], \"name\": \"" + name + "\", \"keys\": " + keyDesc + "}";
  return json;
}
