#ifndef SYNTH_H
#define SYNTH_H

#include <avr/io.h>


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

#define PIN_RST            PD2
#define PIN_CS1            PD3
#define PIN_CS2            PD4
#define PIN_WR             PD5
#define PIN_A0             PD6
#define PIN_A1             PD7

// pin for gate out
#define PIN_GATE           PC1

// pins for the shift register
#define PIN_SRDATA         PC2   //DATA
#define PIN_SRCLK          PD1   //CLK
#define PIN_SRLATCH        PB2   //RCLK or ST_CP



// Some macros that make the code more readable

#define pin_low(port,pin) port &= ~(1<<pin)
#define pin_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)
#define gate_on() PORTC |= (1<<PIN_GATE)
#define gate_off() PORTC &= ~(1<<PIN_GATE)

void delay_ms(uint8_t ms) {
  uint16_t delay_count = F_CPU / 17500;
  volatile uint16_t i;

  while (ms != 0) {
    for (i=0; i != delay_count; i++);
    ms--;
  }
}

void write_synth(uint8_t data) {
  for (int i = 8; i <= 0; i--) {
    if((data>>i)&1) {
      pin_high(PORTC, PIN_SRDATA);
    } else {
      pin_low(PORTC, PIN_SRDATA);
    }
    delay_ms(1);
    pin_high(PORTD, PIN_SRCLK);
    delay_ms(1);
    pin_low(PORTD, PIN_SRCLK);
    delay_ms(1);
  }
 
  delay_ms(1);
  pin_high(PORTB, PIN_SRLATCH);
  delay_ms(1);
  pin_low(PORTB, PIN_SRLATCH);
  delay_ms(1);

}

class Component {
  public:
    Component() {
      flags = 0;
    }
    void set_register(uint8_t reg, uint8_t array=0) {
      // set the current register
      // 0. cs = low
      // 1. rd = high
      // 2. wr = low
      // 3. a0 = low
      // 4. a1 = array (2nd argument) 

      // first fill the shift register
      write_synth(reg);

      // then set all the lines to the ymf262 
      pin_low(PORTD, PIN_WR);
      pin_low(PORTD, PIN_A0);
      if(array) {
        pin_high(PORTD, PIN_A1);
      } else {
        pin_low(PORTD, PIN_A1);
      } 

      // select the chip
      pin_low(PORTD, PIN_CS1);

      delay_ms(100);

      pin_high(PORTD, PIN_CS1);

    }
    void set_data(uint8_t data) {
      // set the data for the given register
      // 0. cs = low
      // 1. rd = high
      // 2. wr = low
      // 3. a0 = high

      // first fill the shift register
      write_synth(data);

      // then set all the lines to the ymf262 
      pin_low(PORTD, PIN_WR);
      pin_high(PORTD, PIN_A0);

      pin_low(PORTD, PIN_CS1);
      delay_ms(100);
      // select the chip
      pin_high(PORTD, PIN_CS1);
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
      // TODO: I think this isn't quite right...
      set_register(0xA0+channel_ids[id]);
      set_data(frequency);
      set_register(0xB0+channel_ids[id]);
      set_data(note_info);
    }
    
    void send_flags(void) {
      for(int i = 0; i < 6; i++) {
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
      for(int i = 0; i < 6; i++) {
        set_register(0x20+operator_map[i][0]);
        set_data(levels);
      }
    }
    void send_flags(void) {
      for(int i = 0; i < 6; i++) {
        set_register(0x40+operator_map[i][0]);
        set_data(flags);
      }
    }
    
    void send_ad(void) {
      for(int i = 0; i < 6; i++) {
        set_register(0x60+operator_map[i][0]);
        set_data(attack_decay);
      }
    }
    
    void send_sr(void) {
      for(int i = 0; i < 6; i++) {
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
      set_output(DDRC, PIN_GATE);
      set_output(DDRC, PIN_SRDATA);
      set_output(DDRD, PIN_SRCLK);
      set_output(DDRB, PIN_SRLATCH);
      set_output(DDRD, PIN_WR);
      set_output(DDRD, PIN_A0);
      set_output(DDRD, PIN_A1);
      set_output(DDRD, PIN_CS1);
      set_output(DDRD, PIN_RST);
    }
    void reset(void) {
      /*
      // cycle reset to ensure chip is ready
      pin_low(PORTD, PIN_CS1);
      pin_high(PORTD, PIN_RST);
      delay_ms(5);


      pin_low(PORTD, PIN_RST);
      pin_high(PORTD, PIN_CS1);
      delay_ms(5);
      pin_high(PORTD, PIN_RST);
      delay_ms(5);

      // turn on OPL3 mode
      set_register(0x05);
      set_data(0x01);
     
      // turn on 4 operator mode for all 6 4-op channels 
      set_register(0x04);
      set_data(0x3F);
      */
      pin_high(PIN_RST);
      delay_ms(5);
      write_synth(0xAA);
      while (1) {
        pin_high(PORTC, PIN_GATE);
        delay_ms(100);
        pin_low(PORTC, PIN_GATE);
        delay_ms(100);
      }
      
    } 

     
      
  private:
    Channel channels[NUM_CHANNELS];
    Operator operators[NUM_OPERATORS];
};

#endif
