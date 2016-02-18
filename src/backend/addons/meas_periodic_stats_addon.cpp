#include "meas_periodic_stats_addon.hpp"

MeasPeriodicStatsAddon::MeasPeriodicStatsAddon() {
  name = "meas_periodic_stats";
  lastTime = 0;
  lastStoreTime = 0;

  path = "stats";

  bufferMax = 60;
  calcInterval = 60000; // every minute
  interval = 24*3600*1000; // how often calculate, default - daily status

  storeDailyFiles = false;
  storeEnabled = true;
}

void MeasPeriodicStatsAddon::setup() {
  // create path at start, no wait
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

  if (storeEnabled) {
    restore();
  }

  meas = measTypeArray->byName(measName);
}

void MeasPeriodicStatsAddon::stop() {
  if (storeEnabled) {
    dump();
  }
}

std::string MeasPeriodicStatsAddon::backupFilename() {
  return path + "/addon_backup_" + name + ".txt";
}

std::string MeasPeriodicStatsAddon::storeFilename(std::shared_ptr<MeasPeriodicStat> s) {
  if (storeDailyFiles) {
    return path + "/" + name + "_" + Helper::currentDateSafe() + "_stats.csv";
  } else {
    return path + "/" + name + "_stats.csv";
  }
}


void MeasPeriodicStatsAddon::restore() {
  std::ifstream infile;
  infile.open(backupFilename(), std::ios_base::in);
  if (infile.good()) {
    int count;
    std::string line;

    getline(infile, line);
    count = std::atoi(line.c_str());
    getline(infile, line);
    lastStoreTime = std::stoull(line.c_str());

    logArray->log("MeasPeriodicStatsAddon", "load backup: " + std::to_string(count));

    getline(infile, line);
    getline(infile, line);
    getline(infile, line);

    for (int i = 0; i < count; i++) {
      std::shared_ptr<MeasPeriodicStat> s = std::make_shared<MeasPeriodicStat>();
      getline(infile, line);
      s->fromCsv(line);
      addToBuffer(s);
    }

  }
  infile.close();
}

void MeasPeriodicStatsAddon::dump() {
  std::ofstream outfile;
  outfile.open(backupFilename(), std::ios_base::out);
  outfile << bufferStat.size() << "\n";
  outfile << lastStoreTime << "\n";

  outfile << std::endl;
  outfile << std::endl;
  outfile << std::endl;

  for (unsigned int i = 0; i < bufferStat.size(); i++ ) {
    std::shared_ptr<MeasPeriodicStat> s = bufferStat[i];
    outfile << s->toCsv();
    outfile << std::endl;
  }

  outfile.close();
}

meas_time MeasPeriodicStatsAddon::normalizeTime(meas_time t) {
  return t - (t % interval);
}

void MeasPeriodicStatsAddon::perform() {
  if (lastTime > ( Helper::mTime() - interval) ) {
    return;
  }

  meas_time initialTime = normalizeTime( meas->buffer->earliestTime() );
  meas_time endTime = normalizeTime( Helper::mTime() ) + interval;
  meas_time t = initialTime;

  logArray->log("MeasPeriodicStatsAddon", "initialTime: " + std::to_string(initialTime));
  logArray->log("MeasPeriodicStatsAddon", "endTime:" + std::to_string(endTime));

  if (bufferStat.size() == 0) {
    // repopulate
    while (t < endTime) {
      logArray->log("MeasPeriodicStatsAddon", "repopulate: " + std::to_string(t));

      std::shared_ptr<MeasPeriodicStat> s = std::make_shared<MeasPeriodicStat>();
      s->time = t;
      updateStats(s);
      addToBuffer(s);

      t += interval;
    }

  } else {
    // take last stat from buffer and update
    std::shared_ptr<MeasPeriodicStat> s = bufferStat[ bufferStat.size() - 1];
    t = s->time;
    updateStats(s);

    while ( (t + interval) < endTime) {
      logArray->log("MeasPeriodicStatsAddon", "append fresh: " + std::to_string(t));

      std::shared_ptr<MeasPeriodicStat> s = std::make_shared<MeasPeriodicStat>();
      s->time = t;
      updateStats(s);
      addToBuffer(s);

      t += interval;
    }
  }



  // iterate and store
  if (storeEnabled) {
    for (unsigned int i = 0; i < bufferStat.size(); i++) {
      std::shared_ptr<MeasPeriodicStat> s = bufferStat[i];

      if (s->time > lastStoreTime) {
        store(s);
        lastStoreTime = s->time;
      }
    }
  }

  lastTime = Helper::mTime();
}

void MeasPeriodicStatsAddon::updateStats(std::shared_ptr<MeasPeriodicStat> s) {
  s->timeLength = interval;

  meas_buffer_index rawSize = 0;
  meas_buffer_index j = 0;
  double sumTmp = 0.0;
  long sumCount = 0;
  double doubleTmp = 0.0;

  double maxValue = 0.0;
  double minValue = 0.0;
  double avgValue = 0.0;


  std::vector < unsigned int > raw = meas->buffer->getFromBuffer(meas->timeToIndex(s->time), meas->timeToIndex(s->time + interval), 0);
  rawSize = raw.size();

  sumTmp = 0.0;
  sumCount = 0;

  if (rawSize > 0) {
    doubleTmp = meas->rawToValue( raw.at(0) );
    maxValue = doubleTmp;
    minValue = doubleTmp;

    for (j = 0; j < rawSize; j++ ) {
      // max battery voltage
      doubleTmp = meas->rawToValue( raw.at(j) );
      if (maxValue < doubleTmp) {
        maxValue = doubleTmp;
      }

      if (minValue > doubleTmp) {
        minValue = doubleTmp;
      }

      sumTmp += doubleTmp;
      sumCount++;
    }

    avgValue = sumTmp / ( (double) sumCount );
  }

  s->max = maxValue;
  s->min = minValue;
  s->avg = avgValue;
}

void MeasPeriodicStatsAddon::addToBuffer(std::shared_ptr<MeasPeriodicStat> wts) {
  if ( (unsigned char) bufferStat.size() >= (unsigned char) bufferMax ) {
    bufferStat.erase( bufferStat.begin() );
  }
  bufferStat.push_back(wts);
}



void MeasPeriodicStatsAddon::store(std::shared_ptr<MeasPeriodicStat> s) {
  std::ofstream outfile;

  logArray->log("MeasPeriodicStatsAddon", "store path " + storeFilename(s));

  outfile.open(storeFilename(s), std::ios_base::app);
  outfile << s->toCsv();
  outfile << std::endl;
  outfile.close();

  logArray->log("MeasPeriodicStatsAddon", "stored");
}

#define NC_S_ADDON_TIME 0
#define NC_S_ADDON_MIN NC_S_ADDON_TIME + 30
#define NC_S_ADDON_AVG NC_S_ADDON_MIN + 20
#define NC_S_ADDON_MAX NC_S_ADDON_AVG + 20

void MeasPeriodicStatsAddon::render() {
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 1, 1, ("Meas Periodic Stats Addon - " + measName + ", interval "+ Helper::intervalToString(interval)).c_str() );
  wattroff(window, NC_COLOR_PAIR_NAME_SET);

  int i = 3;

  // time
  wattron(window, NC_COLOR_PAIR_NAME_LESSER_SET);
  mvwprintw(window, i, 1 + NC_S_ADDON_TIME, "time" );
  mvwprintw(window, i, 1 + NC_S_ADDON_MIN, "min" );
  mvwprintw(window, i, 1 + NC_S_ADDON_AVG, "avg" );
  mvwprintw(window, i, 1 + NC_S_ADDON_MAX, "max" );
  wattroff(window, NC_COLOR_PAIR_NAME_LESSER_SET);

  if (bufferStat.size() == 0) {
    return;
  }

  for (int j = 0; j < (unsigned char) bufferStat.size(); j++) {
    int iRow = i + 1 + j;

    if (iRow >= (LINES - 3)) {
      break;
    }

    std::shared_ptr<MeasPeriodicStat> s = bufferStat.at(bufferStat.size() - 1 - j);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_S_ADDON_TIME, Helper::timeToDateTimeString(s->time).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_S_ADDON_MIN, meas->valueToFormatted(s->min).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_S_ADDON_AVG, meas->valueToFormatted(s->avg).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);

    wattron(window, NC_COLOR_PAIR_VALUE_SET);
    mvwprintw(window, iRow, 1 + NC_S_ADDON_MAX, meas->valueToFormatted(s->max).c_str() );
    wattroff(window, NC_COLOR_PAIR_VALUE_SET);
  }
}

std::string MeasPeriodicStatsAddon::toJson() {
  std::string json = "{\"array\": [";

  for (int j = 0; j < (unsigned char) bufferStat.size(); j++) {
    std::shared_ptr<MeasPeriodicStat> wst = bufferStat.at(bufferStat.size() - 1 - j);
    json += wst->toJson();
    json += ",";
  }

  // remove last coma
  if (json[json.size() - 1] == ',') {
    json.resize(json.size() - 1);
  }

  std::string keyDesc = "[";
  keyDesc += "{\"key\": \"time\", \"type\": \"time\"},";
  keyDesc += "{\"key\": \"min\", \"type\": \"float\", \"unit\": \"" + meas->unit + "\"},";
  keyDesc += "{\"key\": \"avg\", \"type\": \"float\", \"unit\": \"" + meas->unit + "\"},";
  keyDesc += "{\"key\": \"max\", \"type\": \"float\", \"unit\": \"" + meas->unit + "\"},";
  keyDesc += "]";

  json += "], \"name\": \"" + name + "\", \"keys\": " + keyDesc + "}";
  return json;
}
