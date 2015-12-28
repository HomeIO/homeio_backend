#include "sample_addon.hpp"

SampleAddon::SampleAddon() {
  name = "SampleAddon";
}

void SampleAddon::perform() {
  //  + std::to_string( measTypeArray->byName("int_temperature")->lastValue() )
  Helper::logInfo("It's sample addon ");
}
