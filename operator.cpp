#include <cstdint>

#define OP_TREMOLO 128
#define OP_VIBRATO 64
#define OP_SUSTAIN 32
#define OP_KSR     16


class Operator {
  public:
    Operator() {
      options         = 0;
      levels          = 0;
      attack_decay    = 0;
      sustain_release = 0;
      waveform        = 0;
    }

    void set_option(uint8_t option) {
      options |= option;
    }

    void unset_option(uint8_t option) {
      options &= ~(1 << option);
    }

    void set_multiplier(uint8_t multiplier) {
      options &= 0xf0;
      options |= (multiplier & 0x0f);
    }
  private:
    uint8_t options;
    uint8_t levels;
    uint8_t attack_decay;
    uint8_t sustain_release;
    uint8_t waveform;
};

