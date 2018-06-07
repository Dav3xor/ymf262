#include <cstdint>

#define CH_NOTE_ON 32

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
      note_info &= 0x1c; // abcdef
      note_info |= (block << 2)
    }

  private:
    uint8_t frequency;  
    uint8_t note_info;
};
