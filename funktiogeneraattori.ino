#include <MD_AD9833.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Menu.hpp"
#include "RotaryEncoder.hpp"

#define DATA  11
#define CLK   13
#define FSYNC 10

MD_AD9833	AD(FSYNC);
// MD_AD9833	AD(DATA, CLK, FSYNC); // Arbitrary SPI pins
LiquidCrystal_I2C lcd(0x27,20,4);
RotaryEncoder rotaryEncoder(7, 4, 5);
Menu menu;

MD_AD9833::mode_t modes[] = {
  MD_AD9833::MODE_SINE,
  MD_AD9833::MODE_TRIANGLE,
  MD_AD9833::MODE_SQUARE1,
  MD_AD9833::MODE_SQUARE2
};

String modeTitles[] = {
  "SIN",
  "TRI",
  "SQ1",
  "SQ2"
};

struct Channel {
  String name;
  int freq;
  int ampl;
  int mode;
  bool isOn;
};

Channel ch1;
Channel ch2;
Channel* selectedChannel = &ch1;

void setup(void)
{
  ch1.name = "CH1";
  ch1.freq = 100;
  ch1.ampl = 5;
  ch1.mode = 0;
  ch1.isOn = false;

  ch2.name = "CH2";
  ch2.freq = 100;
  ch2.ampl = 5;
  ch2.mode = 0;
  ch2.isOn = false;

  AD.begin();
  setADFreq();
  setADMode();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print(":            /");
  lcd.setCursor(0,1);
  lcd.print("Hz:");
  lcd.setCursor(0,2);
  lcd.print("Vpp:");
  lcd.setCursor(0,3);
  lcd.print("Mode:");

  displayChannel();
  displayFreq();
  displayAmpl();
  displayMode();
  displayCursor();

  rotaryEncoder.init();

  Serial.begin(9600);
}

void loop(void)
{
  rotaryEncoder.handle();

  if (rotaryEncoder.CW()) {
    if (menu.getMode() == Menu::MODE_NAV) {
      menu.next();
      displayCursor();
    } else if (menu.getMode() == Menu::MODE_EDIT) {
      increaseValue();
    }
  }

  if (rotaryEncoder.CCW()) {
    if (menu.getMode() == Menu::MODE_NAV) {
      menu.prev();
      displayCursor();
    } else if (menu.getMode() == Menu::MODE_EDIT) {
      decreaseValue();
    }
  }

  if (rotaryEncoder.pressed()) {
    menu.toggleMode();
    displayCursor();
    if (menu.getMode() == Menu::MODE_NAV) {
      setADFreq();
      setADMode();
    }
  }

  // Serial.println(analogRead(A0));
  delay(1);
}

void setADFreq() {
  AD.setFrequency(MD_AD9833::CHAN_0, selectedChannel->freq);
}

void setADMode() {
  if (selectedChannel->isOn) {
    AD.setMode(modes[selectedChannel->mode]);
  } else {
    AD.setMode(MD_AD9833::MODE_OFF);
  }
}

void switchChannel() {
  if (selectedChannel == &ch1) {
    selectedChannel = &ch2;
  } else {
    selectedChannel = &ch1;
  }
}

void increaseValue() {
  switch(menu.getPosition()) {
    case Menu::POS_CHAN:
      switchChannel();
      displayChannel();
      displayFreq();
      displayAmpl();
      displayMode();
      break;
    case Menu::POS_FREQ:
      if (selectedChannel->freq < 1000000) {
        selectedChannel->freq++;
      }
      displayFreq();
      break;
    case Menu::POS_AMPL:
      displayAmpl();
      break;
    case Menu::POS_MODE:
      if (selectedChannel->mode < 3) {
        selectedChannel->mode++;
      } else {
        selectedChannel->mode = 0;
      }
      displayMode();
      break;
    case Menu::POS_ONOFF:
      if (ch1.isOn && ch2.isOn) {
        ch1.isOn = false;
        ch2.isOn = false;
      } else if (!ch1.isOn && !ch2.isOn) {
        ch1.isOn = true;
      } else if (ch1.isOn) {
        ch1.isOn = false;
        ch2.isOn = true;
      } else {
        ch1.isOn = true;
        ch2.isOn = true;
      }
      displayOnOff();
      break;
  }
}

void decreaseValue() {
  switch(menu.getPosition()) {
    case Menu::POS_CHAN:
      switchChannel();
      displayChannel();
      displayFreq();
      displayAmpl();
      displayMode();
      break;
    case Menu::POS_FREQ:
      if (selectedChannel->freq > 0) {
        selectedChannel->freq--;
      }
      displayFreq();
      break;
    case Menu::POS_AMPL:
      displayAmpl();
      break;
    case Menu::POS_MODE:
      if (selectedChannel->mode > 0) {
        selectedChannel->mode--;
      } else {
        selectedChannel->mode = 3;
      }
      displayMode();
      break;
    case Menu::POS_ONOFF:
      if (ch1.isOn && ch2.isOn) {
        ch1.isOn = false;
      } else if (!ch1.isOn && ch2.isOn) {
        ch1.isOn = true;
        ch2.isOn = false;
      } else if (ch1.isOn) {
        ch1.isOn = false;
        ch2.isOn = false;
      } else {
        ch1.isOn = true;
        ch2.isOn = true;
      }
      displayOnOff();
      break;
  }
}

void displayChannel() {
  lcd.setCursor(0,0);
  lcd.print(selectedChannel->name);
}

void displayFreq() {
  Serial.println(selectedChannel->freq);
  lcd.setCursor(6,1);
  lcd.print("              ");
  lcd.setCursor(6,1);
  lcd.print(selectedChannel->freq);
}

void displayAmpl() {
  Serial.println(selectedChannel->ampl);
  lcd.setCursor(6,2);
  lcd.print("              ");
  lcd.setCursor(6,2);
  lcd.print(selectedChannel->ampl);
}

void displayMode() {
  lcd.setCursor(6,3);
  lcd.print(modeTitles[selectedChannel->mode]);
}

void displayOnOff() {
  lcd.setCursor(13,0);
  lcd.print(ch1.isOn ? ch1.name : "   ");
  lcd.setCursor(17,0);
  lcd.print(ch2.isOn ? ch2.name : "   ");
}

void displayCursor() {
  String cursor;
  if (menu.getMode() == Menu::MODE_NAV) {
    cursor = "*";
  } else {
    cursor = ">";
  }

  lcd.setCursor(3,0);
  lcd.print(" ");
  lcd.setCursor(3,1);
  lcd.print(" ");
  lcd.setCursor(4,2);
  lcd.print(" ");
  lcd.setCursor(5,3);
  lcd.print(" ");
  lcd.setCursor(12,0);
  lcd.print(" ");

  switch(menu.getPosition()) {
    case Menu::POS_CHAN:
      lcd.setCursor(3,0);
      lcd.print(cursor);
      break;
    case Menu::POS_FREQ:
      lcd.setCursor(3,1);
      lcd.print(cursor);
      break;
    case Menu::POS_AMPL:
      lcd.setCursor(4,2);
      lcd.print(cursor);
      break;
    case Menu::POS_MODE:
      lcd.setCursor(5,3);
      lcd.print(cursor);
      break;
    case Menu::POS_ONOFF:
      lcd.setCursor(12,0);
      lcd.print(cursor);
      break;
  }
}
