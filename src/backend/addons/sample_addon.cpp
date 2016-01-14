#include "sample_addon.hpp"

SampleAddon::SampleAddon() {
  name = "SampleAddon";
}

void SampleAddon::perform() {
  logArray->log("Addon", "It's sample addon ");
}
