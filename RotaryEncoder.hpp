#ifndef RotaryEncoder_hpp
#define RotaryEncoder_hpp

#include <Arduino.h>

class RotaryEncoder {
  public:
    RotaryEncoder(int dt, int clk, int sw);

    void init();
    void handle();

    bool CW();
    bool CCW();
    bool pressed();
  private:
    int m_dt;
    int m_clk;
    int m_sw;

    int m_currentCLK;
    int m_lastCLK;
    unsigned long m_lastPress{0};
    unsigned long m_debounce{0};

    bool m_cw{false};
    bool m_ccw{false};
    bool m_pressed{false};
};

#endif
