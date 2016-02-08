#include "plant_monitor_item.hpp"

PlantMonitorItem::PlantMonitorItem() {
  msWateredAgo = 0;
  preWater = 0.0;
  postWater = 0.0;

  minValue = 0.0;
  maxValue = 0.0;

  drySpeed = 0.0;
  waterNeededIn = 0;

  waterAtValue = 50.0;
}

void PlantMonitorItem::process() {
  if (0 == measType->buffer->count) {
    return;
  }

  meas_buffer_index i = 0;
  double oldValue = 0.0;
  double newValue = 0.0;
  meas_time measInterval = (meas_time) measType->buffer->calcInterval();

  double tempDryValueDiff = 0.0;
  double tempDryTimeDiff = 0.0;
  double tempDrySpeedSum = 0.0;
  unsigned long int tempRegions = 0;
  meas_buffer_index tempLastWateredIndex = 0;

  oldValue = measType->valueAt(0);
  newValue = oldValue;
  minValue = oldValue;
  maxValue = oldValue;

  // min and max value
  for (i = 1; i < measType->buffer->count; i++) {
    oldValue = measType->valueAt(i);

    // max
    if (maxValue < oldValue) {
      maxValue = oldValue;
      maxValueAgo = ((meas_time) i) * measInterval;
    }

    // min
    if (minValue > oldValue) {
      minValue = oldValue;
      minValueAgo = ((meas_time) i) * measInterval;
    }

    // dry speed - unit per second
    if ( (newValue - oldValue) > 3.0 ) {
      tempDryValueDiff = measType->valueAt(tempLastWateredIndex) - measType->valueAt(i);
      tempDryTimeDiff = ( ( (double) (i) - (double) (tempLastWateredIndex) ) * (double) (measInterval) ) / 1000.0;

      // logArray->log("PlantMonitorItem", "tempDryValueDiff " + std::to_string( tempDryValueDiff ));
      // logArray->log("PlantMonitorItem", "tempDryTimeDiff " + std::to_string( tempDryTimeDiff ));

      if ( (tempDryTimeDiff > 100.0) && (tempDryValueDiff > 0.5) ) {
        tempDrySpeedSum += (tempDryValueDiff / tempDryTimeDiff);
        tempRegions++;
      }

      tempLastWateredIndex = i;
    }

    newValue = oldValue;
  }

  // logArray->log("PlantMonitorItem", "tempRegions " + std::to_string( tempRegions ));
  // logArray->log("PlantMonitorItem", "tempDryValueDiff " + std::to_string( tempDryValueDiff ));
  // logArray->log("PlantMonitorItem", "tempDryTimeDiff " + std::to_string( tempDryTimeDiff ));
  // logArray->log("PlantMonitorItem", "tempDrySpeedSum " + std::to_string( tempDrySpeedSum ));

  if (tempRegions > 0) {
    drySpeed = tempDrySpeedSum / (double) tempRegions;
  }


  // last watered
  for (i = 1; i < measType->buffer->count; i++) {
    oldValue = measType->valueAt(i);

    if (wasWateredNow(oldValue, newValue)) {
      // logArray->log("PlantMonitorItem", "int " + std::to_string( measType->buffer->calcInterval() ));
      // logArray->log("PlantMonitorItem", "last " + std::to_string( measType->buffer->lastTimeForCount ));
      // logArray->log("PlantMonitorItem", "firstTime " + std::to_string( measType->buffer->firstTime ));
      // logArray->log("PlantMonitorItem", "count " + std::to_string( measType->buffer->count ));
      // logArray->log("PlantMonitorItem", "offset " + std::to_string( measType->buffer->offset ));

      msWateredAgo = ((meas_time) i) * measInterval;
      preWater = oldValue;
      postWater = newValue;

      break;
    }

    newValue = oldValue;
  }

  // prediction
  oldValue = measType->valueAt(0);
  if (oldValue > waterAtValue) {
    double t = (((double) msWateredAgo) * (oldValue - waterAtValue) / (postWater - oldValue));
    waterNeededIn = (meas_time) t;

    logArray->log("PlantMonitorItem", "msWateredAgo " + std::to_string( msWateredAgo ));
    logArray->log("PlantMonitorItem", "postWater " + std::to_string( postWater ));
    logArray->log("PlantMonitorItem", "oldValue " + std::to_string( oldValue ));
    logArray->log("PlantMonitorItem", "waterAtValue " + std::to_string( waterAtValue ));
  }

}

bool PlantMonitorItem::wasWateredNow(double oldValue, double newValue) {
  if ((oldValue > 10.0) && (newValue > 10.0) && (oldValue < (newValue - 4.0))) {
    return true;
  } else {
    return false;
  }

}

// TODO define raw value, index and time
