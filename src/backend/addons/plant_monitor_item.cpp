#include "plant_monitor_item.hpp"

PlantMonitorItem::PlantMonitorItem() {
  msWateredAgo = 0;
}

void PlantMonitorItem::process() {
  meas_buffer_index i = 0;
  double tmpValue = 0.0;
  double lastValue = 0.0;

  if (0 == measType->buffer->count) {
    return;
  }

  lastValue = measType->valueAt(0);

  for (i = 1; i < measType->buffer->count; i++) {
    tmpValue = measType->valueAt(i);

    if ((lastValue > 10.0) && (tmpValue > 10.0) && (lastValue > (tmpValue - 5.0))) {
      msWateredAgo = i * (meas_time) measType->buffer->calcInterval();
      break;
    }

    lastValue = tmpValue;
  }
}

// TODO define raw value, index and time
