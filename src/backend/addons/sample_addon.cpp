#include "sample_addon.hpp"

SampleAddon::SampleAddon() {
  name = "SampleAddon";
}

void SampleAddon::perform() {
  //  + to_string( measTypeArray->byName("int_temperature")->lastValue() )
  logInfo("It's sample addon ");
}
