#ifndef SAMPLE_ADDON_HPP
#define	SAMPLE_ADDON_HPP

#include "abstract_addon.hpp"

using namespace std;

class SampleAddon : public AbstractAddon {
 public:
  SampleAddon();
  void perform();
};

//#include "sample_addon.cpp"
#endif
