#include <Arduino.h>
#include "RotaryEncoder.hpp"

RotaryEncoder::RotaryEncoder(int dt, int clk, int sw): m_dt(dt), m_clk(clk), m_sw(sw) {}

void RotaryEncoder::init() {
  pinMode(m_clk, INPUT);
  pinMode(m_dt, INPUT);
  pinMode(m_sw, INPUT_PULLUP);
  m_lastCLK = digitalRead(m_clk);
}

void RotaryEncoder::handle() {
  m_currentCLK = digitalRead(m_clk);

  m_cw = false;
  m_ccw = false;
  m_pressed = false;

  if (m_currentCLK != m_lastCLK && m_currentCLK == 1) {
    if (digitalRead(m_dt) != m_currentCLK) {
      m_cw = true;
    } else {
      m_ccw = true;
    }
  }

  m_lastCLK = m_currentCLK;

  if (digitalRead(m_sw) == HIGH) {
    if (millis() - m_lastPress > 50) {
      m_pressed = true;
    }
    m_lastPress = millis();
  }
}

bool RotaryEncoder::CW() {
  return m_cw;
};

bool RotaryEncoder::CCW() {
  return m_ccw;
};

bool RotaryEncoder::pressed() {
  return m_pressed;
}
