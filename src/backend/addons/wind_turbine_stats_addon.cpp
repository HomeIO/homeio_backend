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
    WindTurbineStat s = calculateStats(lastTime);
    store(s);
  }
}

unsigned long long WindTurbineStatsAddon::calculateTimeFrom() {
  unsigned long long t = mTime();
  t = t - hour; // calculate for previous hour only
  t = t - (t % hour);
  return t;
}

WindTurbineStat WindTurbineStatsAddon::calculateStats(unsigned long long t) {
  WindTurbineStat s;
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

  vector < unsigned int > uRaw = u->buffer->getFromBuffer(u->timeToIndex(t), u->timeToIndex(t + hour), 0);
  vector < unsigned int > iRaw = i->buffer->getFromBuffer(i->timeToIndex(t), i->timeToIndex(t + hour), 0);

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
  vector < unsigned int > coilRaw = coil->buffer->getFromBuffer(coil->timeToIndex(t), coil->timeToIndex(t + hour), 0);
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
  vector < unsigned int > resistorRaw = resistor->buffer->getFromBuffer(resistor->timeToIndex(t), resistor->timeToIndex(t + hour), 0);
  intervalInt = resistor->buffer->calcInterval();

  for (j = 0; j < resistorRaw.size(); j++ ) {
    if ( resistorRaw.at(j) > 0 ){
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

void WindTurbineStatsAddon::store(WindTurbineStat s) {
  ofstream outfile;
  string currentDate( currentDateSafe() );
  string filename = path + "/wind_turbine_stats_" + currentDate + ".csv";

  logInfo("WindTurbineStats store path " + filename);

  outfile.open(filename, ios_base::app);
  outfile << s.time << "; ";
  outfile << s.work << "; ";
  outfile << s.timeLength << "; ";
  outfile << s.coilTime << "; ";
  outfile << s.battCurrentTime << "; ";
  outfile << s.resistorTime << "; ";
  outfile << s.maxBattCurrent << "; ";
  outfile << endl;
  outfile.close();

  logInfo("WindTurbineStats stored");
}
