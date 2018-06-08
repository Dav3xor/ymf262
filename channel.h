#include <cstdint>

#define CH_NOTE_ON 32

#define CH_LEFT    16
#define CH_RIGHT   32
#define CH_C       64
#define CH_D       128


class Channel {
  public:
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
  private:
    uint8_t frequency;  
    uint8_t note_info;
    uint8_t flags;
};