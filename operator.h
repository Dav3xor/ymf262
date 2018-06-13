#ifndef OPERATOR_H
#define OPERATOR_H

#include <cstdint>
#include "component.h"

#define OP_TREMOLO 128
#define OP_VIBRATO 64
#define OP_SUSTAIN 32
#define OP_KSR     16


class Operator : Component {
  public:
    Operator(uint8_t newid) {
      id              = newid;
      levels          = 0;
      attack_decay    = 0;
      sustain_release = 0;
      waveform        = 0;
    }

    void set_multiplier(uint8_t multiplier) {
      flags &= 0xf0;
      flags |= (multiplier & 0x0f);
    }

    void set_keyscale(uint8_t keyscale) {
      levels &= 0x3f;
      levels |= ((keyscale & 0x03) << 6);
    }

    void set_level(uint8_t level) {
      levels &= 0xc0;
      levels |= (level & 0x3f);
    }
    
    void set_attack(uint8_t attack) {
      attack_decay &= 0x0f;
      attack_decay |= ((attack & 0x0f) << 4);
    }

    void set_decay(uint8_t decay) {
      attack_decay &= 0xf0;
      attack_decay |= (decay & 0x0f);
    }
    
    void set_sustain(uint8_t sustain) {
      sustain_release &= 0x0f;
      sustain_release |= ((sustain & 0x0f) << 4);
    }

    void set_release(uint8_t release) {
      sustain_release &= 0xf0;
      sustain_release |= (release & 0x0f);
    }

    void set_waveform(uint8_t wave) {
      waveform = wave & 0x07;
    }

  private:
    constexpr static uint8_t operator_ids [6][4] = { {0,  3,  6,  9},
                                                     {1,  4,  7,  10},
                                                     {2,  5,  8,  11},
                                                     {18, 21, 24, 27},
                                                     {19, 22, 25, 28},
                                                     {20, 23, 26, 29} };
    uint8_t id;
    uint8_t levels;
    uint8_t attack_decay;
    uint8_t sustain_release;
    uint8_t waveform;
};

#endif

