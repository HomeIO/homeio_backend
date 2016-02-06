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

    if (lastValue > (tmpValue - 5.0)) {
      msWateredAgo = i * (meas_time) measType->buffer->calcInterval();
      break;
    }
  }
}

// TODO define raw value, index and time
