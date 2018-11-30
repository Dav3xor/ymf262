#include "stdint.h" 
#include "synth.h"

int main(void) {
  Synth synth;
  synth.reset();









  // TODO convert these to tests
  /*
  o.set_level(5);
  o.send_levels();
  o.set_keyscale(1);
  o.send_levels();
  o.set_level(4);
  o.send_levels();
  o.set_keyscale(255);
  o.send_levels();
  o.set_keyscale(2);
  o.set_level(255);
  o.send_levels();
  */
  
  /*
  o.set_flag(OP_TREMOLO);
  o.send_flags();
  o.set_multiplier(13); 
  o.send_flags();
  o.set_flag(OP_VIBRATO);
  o.send_flags();
  */

  /*
  o.set_attack(2);
  o.send_ad();
  o.set_decay(3);
  o.send_ad();
  o.set_attack(255);
  o.send_ad();
  */

  /*
  o.set_sustain(2);
  o.send_sr();
  o.set_release(3);
  o.send_sr();
  o.set_sustain(255);
  o.send_sr();
  */
 
  /* 
  c.set_frequency(255);
  c.send_frequency();
  c.set_frequency(256);
  c.send_frequency();
  c.set_frequency(1023);
  c.send_frequency();
  c.set_frequency(1024);
  c.send_frequency();
  c.set_frequency(550);
  c.set_note_on();
  c.send_frequency();
  c.set_note_off();
  c.send_frequency();
  c.set_block_number(1);
  c.send_frequency();
  c.set_block_number(7);
  c.send_frequency();
  c.set_block_number(100);
  c.send_frequency();
  */
 
  /*
  c.set_flag(CH_RIGHT);
  c.send_flags();
  c.set_feedback(3); 
  c.send_flags();
  c.set_feedback(255); 
  c.send_flags();
  c.unset_flag(CH_RIGHT);
  c.send_flags();
  */
   
}

constexpr uint8_t Operator::operator_map[6][4];
constexpr uint8_t Operator::register_map[36];
constexpr uint8_t Channel::channel_ids [];
constexpr uint8_t Channel::register_ids [];
