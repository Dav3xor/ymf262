class Component {
  public:
    void set_register(uint8_t reg, uint8_t array=0) {
      // set the current register
      // 0. cs = low
      // 1. rd = high
      // 2. wr = low
      // 3. a0 = low
      // 4. a1 = array (2nd argument) 
    }
    void set_data(uint8_t data) {
      // set the data for the given register
      // 0. cs = low
      // 1. rd = high
      // 2. wr = low
      // 3. a0 = high
    }
    void set_flag(uint8_t flag) {
      flags |= flag;
    }

    void unset_flag(uint8_t flag) {
      flags &= ~(1 << flag);
    }
  protected:
    uint8_t flags;
};
