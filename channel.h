#include <cstdint>

#define CH_NOTE_ON 32

class Channel {
  public:
    void set_frequency(uint8_t freq) {
      frequency = freq;
    }
    void set_note_on(void) {
      note_info |= CH_NOTE_ON;
    }
    void set_note_off(void) {
      note_info &= ~(CH_NOTE_ON);
    }

  private:
    uint8_t frequency;  
    uint8_t note_info;
};
