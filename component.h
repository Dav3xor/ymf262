class Component {
  public:
    void set_flag(uint8_t flag) {
      flags |= flag;
    }

    void unset_flag(uint8_t flag) {
      flags &= ~(1 << flag);
    }
  protected:
    uint8_t flags;
};
