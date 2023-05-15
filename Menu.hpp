#ifndef Menu_hpp
#define Menu_hpp

#include <Arduino.h>

class Menu {
  public:
    Menu();

    enum MenuMode{MODE_NAV, MODE_EDIT};
    enum MenuPosition{POS_CHAN, POS_FREQ, POS_AMPL, POS_MODE, POS_ONOFF};

    void next();
    void prev();
    void toggleMode();

    MenuMode getMode();
    MenuPosition getPosition();
  private:
    MenuMode m_mode{MODE_NAV};
    MenuPosition m_position{POS_CHAN};
};

#endif
