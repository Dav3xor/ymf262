#include <cstdint>
#include "operator.h"

#define CH_NOTE_ON 32

#define CH_LEFT    16
#define CH_RIGHT   32
#define CH_C       64
#define CH_D       128
#define CH_SYN     1

class Channel : Component{
  public:
    Channel(uint8_t newid) : operators{0,1,2,3} {
      id = newid;
    }
      
    void set_frequency(uint16_t freq) {
      frequency = freq;
      note_info &= 0xfc;
      note_info |= (frequency >> 8);
    }

    void set_note_on(void) {
      note_info |= CH_NOTE_ON;
    }

    void set_note_off(void) {
      note_info &= ~(CH_NOTE_ON);
    }

    void set_block_number(uint8_t block) {
      note_info &= 0xe3; // abcdef
      note_info |= ((block & 0x07) << 2);
    }
    void set_feedback(uint8_t feedback) {
      note_info &= 0xf1;
      note_info |= ((feedback & 0x07) << 1);
    }
    void set_syn(void) {
      flags |= CH_SYN;
    }

  private:
    Operator operators[4];
    constexpr static uint8_t channel_ids [] = {0,1,2,9,10,11};

    uint8_t id;
    uint8_t frequency;  
    uint8_t note_info;
};
