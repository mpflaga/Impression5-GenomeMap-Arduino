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
  for (int offset = 0; offset < 4; offset++) {
    if (!chips[offset].begin(offset + 0x5A)) {
      Serial.println("error setting up MPR121");
      switch (chips[offset].getError()) {
        case NO_ERROR:
          Serial.println("no error");
          break;
        case ADDRESS_UNKNOWN:
          Serial.println("incorrect address");
          break;
        case READBACK_FAIL:
          Serial.println("readback failure");
          break;
        case OVERCURRENT_FLAG:
          Serial.println("overcurrent on REXT pin");
          break;
        case OUT_OF_RANGE:
          Serial.println("electrode out of range");
          break;
        case NOT_INITED:
          Serial.println("not initialised");
          break;
        default:
          Serial.println("unknown error");
          break;
      }
      while (1);
    } else {
      Serial.print("MPR121 "); Serial.print(offset); Serial.println(" started");
    }
    chips[offset].setInterruptPin(interruptPin);
    Serial.print("Initializing groups MPR #0x");

    Serial.print((byte) pgm_read_word(&groups[offset].address), HEX);
    Serial.println();

    chips[offset].setFFI(FFI_10);
    chips[offset].setSFI(SFI_10);
    chips[offset].setGlobalCDT(CDT_4US);  // reasonable for larger capacitances
    delay(1000);
    chips[offset].autoSetElectrodes();  // autoset all electrode settings

  }
}

int tpad::scan() {
  int identifier = 0;

  for (int addr = 0; addr < 4; addr++) {
    chips[addr].updateAll();

    for (int pin = 0; pin < numElectrodes; pin++) {

      if (chips[addr].isNewTouch(pin)) {
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
