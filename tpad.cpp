#include "Arduino.h"
#include "tpad.h"
#include <avr/pgmspace.h>

tpad::tpad() {
}

tpad::~tpad() {
}

void tpad::begin(Stream &serial, int IRQpin) {
  IFDEBUG(_serial = &serial);
  
  interruptPin = IRQpin;
  for (int offset = 0; offset < 4; offset++) {
    if (!chips[offset].begin(offset + 0x5A)) {
      IFDEBUG(_serial->println("error setting up MPR121"));
      switch (chips[offset].getError()) {
        case NO_ERROR:
          IFDEBUG(_serial->println("no error"));
          break;
        case ADDRESS_UNKNOWN:
          IFDEBUG(_serial->println("incorrect address"));
          break;
        case READBACK_FAIL:
          IFDEBUG(_serial->println("readback failure"));
          break;
        case OVERCURRENT_FLAG:
          IFDEBUG(_serial->println("overcurrent on REXT pin"));
          break;
        case OUT_OF_RANGE:
          IFDEBUG(_serial->println("electrode out of range"));
          break;
        case NOT_INITED:
          IFDEBUG(_serial->println("not initialised"));
          break;
        default:
          IFDEBUG(_serial->println("unknown error"));
          break;
      }
      while (1);
    } else {
      IFDEBUG(_serial->print("MPR121 ")); IFDEBUG(_serial->print(offset)); IFDEBUG(_serial->println(" started"));
    }
    chips[offset].setInterruptPin(interruptPin);
    IFDEBUG(_serial->print("Initializing groups MPR #0x"));
    //    IFDEBUG(_serial->print(offset));
    IFDEBUG(_serial->print((byte) pgm_read_word(&groups[offset].address), HEX));
    IFDEBUG(_serial->println());

    chips[offset].setFFI(FFI_10);
    chips[offset].setSFI(SFI_10);
    chips[offset].setGlobalCDT(CDT_4US);  // reasonable for larger capacitances
    delay(1000);
    chips[offset].autoSetElectrodes();  // autoset all electrode settings

  }
}

int tpad::scan() {
  uint8_t identifier;
  
  for (int addr = 0; addr < 4; addr++) {
    chips[addr].updateAll();
    for (int pin = 0; pin < 12; pin++) {
      if (chips[addr].isNewTouch(pin)) {
        IFDEBUG(_serial->print("chip "));
        IFDEBUG(_serial->print(addr, DEC));
        IFDEBUG(_serial->print(" electrode "));
        IFDEBUG(_serial->print(pin, DEC));
        IFDEBUG(_serial->print(" identifier "));
        identifier = (uint8_t) pgm_read_word(&groups[addr].identifier[pin]);
        IFDEBUG(_serial->print((uint8_t) identifier, DEC));
        IFDEBUG_LED(_serial->printf(" '%p' ", regions[identifier]));
        IFDEBUG(_serial->println(" was just touched"));
                
      } else if (chips[addr].isNewRelease(pin)) {
        IFDEBUG(_serial->print("chip "));
        IFDEBUG(_serial->print(addr, DEC));
        IFDEBUG(_serial->print(" electrode "));
        IFDEBUG(_serial->print(pin, DEC));
        IFDEBUG(_serial->print(" identifier "));
        IFDEBUG(_serial->print((uint8_t) pgm_read_word(&groups[addr].identifier[pin]), DEC));
        IFDEBUG_LED(_serial->printf(" '%p' ", regions[identifier]));
        IFDEBUG(_serial->println(" was just released"));
      }
    }
  }
  return 0;
}
