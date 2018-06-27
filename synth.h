#ifndef SYNTH_H
#define SYNTH_H

#include <cstdint>
#include <cstdio>
#define SYNTH_TREM_DEPTH 128
#define SYNTH_VIBR_DEPTH 64
#define NUM_CHANNELS 6
#define NUM_OPERATORS 4

#define CH_NOTE_ON 32

#define CH_LEFT    16
#define CH_RIGHT   32
#define CH_C       64
#define CH_D       128
#define CH_SYN     1

#define OP_TREMOLO 128
#define OP_VIBRATO 64
#define OP_SUSTAIN 32
#define OP_KSR     16

#define PIN_CS1    1
#define PIN_CS2    2
#define PIN_RD     3
#define PIN_WR     4
#define PIN_A0     5
#define PIN_A1     6

// pins for the shift register
#define PIN_SRDATA   7
#define PIN_SRCLK    8
#define PIN_SRLATCH  9 // RCLK or ST_CP
 
class Component {
  public:
    Component() {
      flags = 0;
    }
    void set_register(uint8_t reg, uint8_t array=0) {
      printf(" - register %d array %d\n", reg, array);
      // set the current register
      // 0. cs = low
      // 1. rd = high
      // 2. wr = low
      // 3. a0 = low
      // 4. a1 = array (2nd argument) 

      // first fill the shift register
      digitalWrite(PIN_SRLATCH, LOW);
      shiftOut(PIN_SRDATA, PIN_SRCLK, MSBFIRST, reg);
      digitalWrite(PIN_SRLATCH, HIGH);

      // then set all the lines to the ymf262 
      digitalWrite(PIN_RD,  HIGH);
      digitalWrite(PIN_WR,  LOW);
      digitalWrite(PIN_A0,  LOW);
      digitalWrite(PIN_A1,  array);

      // select the chip
      digitalWrite(PIN_CS1, LOW);

      delay(100);

      digitalWrite(PIN_CS1, HIGH);

    }
    void set_data(uint8_t data) {
      printf(" - data     %d\n", data);
      // set the data for the given register
      // 0. cs = low
      // 1. rd = high
      // 2. wr = low
      // 3. a0 = high

      // first fill the shift register
      digitalWrite(PIN_SRLATCH, LOW);
      shiftOut(PIN_SRDATA, PIN_SRCLK, MSBFIRST, data);
      digitalWrite(PIN_SRLATCH, HIGH);

      // then set all the lines to the ymf262 
      digitalWrite(PIN_RD,  HIGH);
      digitalWrite(PIN_WR,  LOW);
      digitalWrite(PIN_A0,  HIGH);

      // select the chip
      digitalWrite(PIN_CS1, LOW);

      delay(100);

      digitalWrite(PIN_CS1, HIGH);
    }
    void set_flag(uint8_t flag) {
      flags |= flag;
    }

    void unset_flag(uint8_t flag) {
      flags &= ~(flag);
    }

  protected:
    uint8_t flags;
};



class Channel : public Component{
  public:
    Channel(uint8_t newid) {
      id = newid;
      frequency = 0;
      note_info = 0;
    }
      
    void send_frequency(void) {
      printf("set ch freq -------\n"); 
      printf("channel:    %d\n", channel_ids[id]);
      // TODO: I think this isn't quite right...
      set_register(0xA0+channel_ids[id]);
      set_data(frequency);
      set_register(0xB0+channel_ids[id]);
      set_data(note_info);
    }
    
    void send_flags(void) {
      printf("set ch flags -------\n"); 
      for(int i = 0; i < 6; i++) {
        printf("channel:    %d\n",i);
        set_register(0xC0+channel_ids[id]);
        set_data(flags);
      }
    }

    void set_frequency(uint16_t freq) {
      frequency = freq;
      note_info &= 0xfc;
      note_info |= ((freq >> 8) & 0x03);
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
      flags &= 0xf1;
      flags |= ((feedback & 0x07) << 1);
    }
    void set_syn(void) {
      flags |= CH_SYN;
    }

  private:
    constexpr static uint8_t channel_ids [] = {0,1,2,9,10,11};

    uint8_t id;
    uint8_t frequency;  
    uint8_t note_info;
};


class Operator : public Component {
  public:
    Operator(uint8_t newid) {
      id              = newid;
      levels          = 0;
      attack_decay    = 0;
      sustain_release = 0;
      waveform        = 0;
    }

    void send_levels(void) {
      printf("set level -------\n"); 
      for(int i = 0; i < 6; i++) {
        printf("channel:    %d\n",i);
        set_register(0x20+operator_map[i][0]);
        set_data(levels);
      }
    }
    void send_flags(void) {
      printf("set op flags -------\n"); 
      for(int i = 0; i < 6; i++) {
        printf("channel:    %d\n",i);
        set_register(0x40+operator_map[i][0]);
        set_data(flags);
      }
    }
    
    void send_ad(void) {
      printf("set attack/decay -------\n"); 
      for(int i = 0; i < 6; i++) {
        printf("channel:    %d\n",i);
        set_register(0x60+operator_map[i][0]);
        set_data(attack_decay);
      }
    }
    
    void send_sr(void) {
      printf("set sustain/release -------\n"); 
      for(int i = 0; i < 6; i++) {
        printf("channel:    %d\n",i);
        set_register(0x80+operator_map[i][0]);
        set_data(sustain_release);
      }
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
    constexpr static uint8_t operator_map [6][4] = { {0,  3,  6,  9},
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


class Synth : Component {
  public:
    Synth(): channels{0,1,2,3,4,5}, operators{0,1,2,3} {
      // init synth...
    }
  private:
    Channel channels[NUM_CHANNELS];
    Operator operators[NUM_OPERATORS];
};

#endif
