#include <Arduino.h>
#include "Menu.hpp"

Menu::Menu() {}

void Menu::next() {
  switch(m_position) {
    case Menu::POS_CHAN:
      m_position = Menu::POS_FREQ;
      break;
    case Menu::POS_FREQ:
      m_position = Menu::POS_AMPL;
      break;
    case Menu::POS_AMPL:
      m_position = Menu::POS_MODE;
      break;
    case Menu::POS_MODE:
      m_position = Menu::POS_ONOFF;
      break;
    case Menu::POS_ONOFF:
      m_position = Menu::POS_CHAN;
      break;
  }
}

void Menu::prev() {
  switch(m_position) {
    case Menu::POS_CHAN:
      m_position = Menu::POS_ONOFF;
      break;
    case Menu::POS_ONOFF:
      m_position = Menu::POS_MODE;
      break;
    case Menu::POS_MODE:
      m_position = Menu::POS_AMPL;
      break;
    case Menu::POS_AMPL:
      m_position = Menu::POS_FREQ;
      break;
    case Menu::POS_FREQ:
      m_position = Menu::POS_CHAN;
      break;
  }
}

void Menu::toggleMode() {
  if (m_mode == Menu::MODE_NAV) {
    m_mode = Menu::MODE_EDIT;
  } else if (m_mode == Menu::MODE_EDIT) {
    m_mode = Menu::MODE_NAV;
  }
}

Menu::MenuMode Menu::getMode() {
  return m_mode;
}

Menu::MenuPosition Menu::getPosition() {
  return m_position;
}
