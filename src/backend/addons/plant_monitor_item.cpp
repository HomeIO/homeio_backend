#include "plant_monitor_item.hpp"

PlantMonitorItem::PlantMonitorItem() {
  msWateredAgo = 0;
  preWater = 0.0;
  postWater = 0.0;

  minValue = 0.0;
  maxValue = 0.0;
}

void PlantMonitorItem::process() {
  meas_buffer_index i = 0;
  double oldValue = 0.0;
  double newValue = 0.0;
  meas_time measInterval = (meas_time) measType->buffer->calcInterval();

  if (0 == measType->buffer->count) {
    return;
  }

  oldValue = measType->valueAt(0);
  newValue = oldValue;
  minValue = oldValue;
  maxValue = oldValue;

  // last watered
  for (i = 1; i < measType->buffer->count; i++) {
    oldValue = measType->valueAt(i);

    if (wasWateredNow(oldValue, newValue)) {
      logArray->log("PlantMonitorItem", "int " + std::to_string( measType->buffer->calcInterval() ));
      logArray->log("PlantMonitorItem", "last " + std::to_string( measType->buffer->lastTimeForCount ));
      logArray->log("PlantMonitorItem", "firstTime " + std::to_string( measType->buffer->firstTime ));
      logArray->log("PlantMonitorItem", "count " + std::to_string( measType->buffer->count ));
      logArray->log("PlantMonitorItem", "offset " + std::to_string( measType->buffer->offset ));

      msWateredAgo = ((meas_time) i) * measInterval;
      preWater = oldValue;
      postWater = newValue;
      break;
    }

    newValue = oldValue;
  }

  // min and max value
  for (i = 1; i < measType->buffer->count; i++) {
    oldValue = measType->valueAt(i);

    if (maxValue < oldValue) {
      maxValue = oldValue;
      maxValueAgo = ((meas_time) i) * measInterval;
    }

    if (minValue < oldValue) {
      minValue = oldValue;
      minValueAgo = ((meas_time) i) * measInterval;
    }

    newValue = oldValue;
  }
}

bool PlantMonitorItem::wasWateredNow(double oldValue, double newValue) {
  if ((oldValue > 10.0) && (newValue > 10.0) && (oldValue < (newValue - 3.0))) {
    return true;
  } else {
    return false;
  }

}

// TODO define raw value, index and time
