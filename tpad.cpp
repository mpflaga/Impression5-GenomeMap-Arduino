#include "Arduino.h"
#include "tpad.h"
#include <avr/pgmspace.h>
#include "pgmStrToRAM.h"


tpad::tpad() {
}

tpad::~tpad() {
}

void tpad::begin(int IRQpin) {

  interruptPin = IRQpin;
  for (int offset = 0; offset < SIZE_OF_CHIPS; offset++) {
    if (!chips[offset].begin(offset + 0x5A)) {
      Serial.println(F("error setting up MPR121"));
      switch (chips[offset].getError()) {
        case NO_ERROR:
          Serial.println(F("no error"));
          break;
        case ADDRESS_UNKNOWN:
          Serial.println(F("incorrect address"));
          break;
        case READBACK_FAIL:
          Serial.println(F("readback failure"));
          break;
        case OVERCURRENT_FLAG:
          Serial.println(F("overcurrent on REXT pin"));
          break;
        case OUT_OF_RANGE:
          Serial.println(F("electrode out of range"));
          break;
        case NOT_INITED:
          Serial.println(F("not initialised"));
          break;
        default:
          Serial.println("unknown error");
          break;
      }
      while (1);
    } else {
      Serial.print(F("MPR121 ")); Serial.print(offset); Serial.println(F(" started"));
    }
    chips[offset].setInterruptPin(interruptPin);
    Serial.print(F("Initializing groups MPR #0x"));

    Serial.print((byte) pgm_read_word(&groups[offset].address), HEX);
    Serial.println();

    for (unsigned char channel = 0; channel < numElectrodes; channel++) {

    Serial.print(F("Channel ")); Serial.print(channel); Serial.print(F(" tthresh = ")); Serial.print((unsigned char) pgm_read_word(&groups[offset].tthresh[channel]));
      Serial.print(F(" rthreshold = ")); Serial.println((unsigned char) pgm_read_word(&groups[offset].rthresh[channel]));

      // this is the touch threshold - setting it low makes it more like a proximity trigger
      // default value is 40 for touch
      chips[offset].setTouchThreshold(channel, (unsigned char) pgm_read_word(&groups[offset].tthresh[channel]));

      // this is the release threshold - must ALWAYS be smaller than the touch threshold
      // default value is 20 for touch
      chips[offset].setReleaseThreshold(channel, (unsigned char) pgm_read_word(&groups[offset].rthresh[channel]));
    }

    chips[offset].setFFI(FFI_10);
    chips[offset].setSFI(SFI_10);
    chips[offset].setGlobalCDT(CDT_500NS);  // reasonable for larger capacitances
    delay(500);
    chips[offset].autoSetElectrodes();  // autoset all electrode settings
    delay(500);

    // initial data update
    chips[offset].updateTouchData();
  }
}

int tpad::scan() {
  int identifier = 0;
  int touchedButtons = 0;

  for (int addr = 0; addr < SIZE_OF_CHIPS; addr++) {
    chips[addr].updateAll();
    touchedButtons += chips[addr].getNumTouches();
  }

  for (int addr = 0; addr < SIZE_OF_CHIPS; addr++) {
    for (int pin = 0; pin < numElectrodes; pin++) {

      if ((chips[addr].isNewTouch(pin)) && (touchedButtons < 2)) {
        identifier = (int8_t) pgm_read_word(&groups[addr].identifier[pin]);
      } else if (chips[addr].isNewRelease(pin)) {
        identifier = -(int8_t) pgm_read_word(&groups[addr].identifier[pin]);
      } else {
        identifier = 0;
      }

      if (identifier != 0) {
        Serial.print(F("chip "));
        Serial.print(addr, DEC);
        Serial.print(F(" electrode "));
        Serial.print(pin, DEC);
        Serial.print(F(" identifier "));
        Serial.print(identifier, DEC);
        Serial.print(F(" '"));
        Serial.print(pgmStrToRAM(regions[abs(identifier)]));
        Serial.print(F("' "));
        if (identifier > 0) {
          Serial.println(F(" was just touched"));
        } else {
          Serial.println(F(" was just released"));
        }
        return identifier;
      }
    }
  }
  return identifier;
}
