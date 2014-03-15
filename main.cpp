#include "src/meas_type.cpp"
#include "src/io_proxy.cpp"

int main()
{
  MeasType *a = new MeasType("batt_u");
  a->assignBufferSize(1000);
  a->addRaw(1);
  a->addRaw(1);
  a->addRaw(1);
  a->assignBufferSize(1000);
  return 0;
}
