class Component {
  public:
    void set_register(uint8_t reg) {
      // set the current register
      if(reg != cur_register) {
        // set here
        cur_register = reg;
      }
    }
    void set_data(uint8_t data) {
      // set the data for the given register
    }
    void set_flag(uint8_t flag) {
      flags |= flag;
    }

    void unset_flag(uint8_t flag) {
      flags &= ~(1 << flag);
    }
  protected:
    uint8_t flags;
    uint8_t cur_register;
};
