#ifndef SYNTH_H
#define SYNTH_H

#include <cstdint>
#include "component.h"
#include "channel.h"

#define SYNTH_TREM_DEPTH 128
#define SYNTH_VIBR_DEPTH 64
#define NUM_CHANNELS 6
#define NUM_OPERATORS 4

class Synth : Component {
  public:
    Synth(): channels{0,1,2,3,4,5} {
      // init synth...
    }
  private:
    Channel channels[NUM_CHANNELS];
    Operator operators[NUM_OPERATORS];
};

#endif
