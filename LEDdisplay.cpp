/**
  \file LEDdisplay.cpp
  \brief LED Driver of NeoPixels in 7 segment arrangement.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@flaga.net
*/

#include "Arduino.h"
#include "LEDdisplay.h"
#include "MigrationData.h"

/**
   \brief Constructor
   \param[in] int Number of Digits in the Display.

   reserve memory
*/
LEDdisplay::LEDdisplay(uint16_t n, uint8_t p, neoPixelType t) : Adafruit_NeoPixel(n, p, t) {
#if DEBUG_LEVEL==1
  Serial.print(F("Constructed - LEDdisplay::")); Serial.print(__func__); Serial.println();
#endif

}

/**
   \brief Destructortructor

   release alocated memory ,if any.
*/
LEDdisplay::~LEDdisplay() {
  IFDEBUG(_serial->printf("Destructed - LEDdisplay::%s()\n", __func__));
}

/**
   \brief Initialize the LEDdisplay

   initialize LEDdisplay along with default private values and pointers.
*/
void LEDdisplay::begin() {
  currentLedPos = 0;
  currentLedOffset = 0;
  Adafruit_NeoPixel::begin();

}

/**
   \brief Initialize the LEDdisplay

   initialize LEDdisplay with a none default Serial Stream for debug.
*/
void LEDdisplay::begin(Stream &serial) {
  begin();
  IFDEBUG(_serial = &serial);
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s()\n", __func__));

  IFDEBUG(_serial->printf("Last LED is ...\n"));
  IFDEBUG(_serial->printf("  ledSegs[%d].endPos == %d\n", LENGTH_OF_ARRAY(ledSegs) - 1, (int) pgm_read_word(&ledSegs[LENGTH_OF_ARRAY(ledSegs) - 1].endPos)));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
}

void LEDdisplay::printRegions() {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s()\n", __func__));

  IFDEBUG(_serial->printf("  regions[dd].chipID, channel, LEDseg, name =  \n"));
  for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    IFDEBUG(_serial->printf("      ..[%02d] = 0x%02x, %02d, %02d, '%p'\n", idx, (int) pgm_read_word(&regions[idx].chipID), (int) pgm_read_word(&regions[idx].channel), (int) pgm_read_word(&regions[idx].LEDseg), regions[idx].name));
  }
  IFDEBUG(_serial->printf("Number of regions = %d\n", SIZE_OF_REGIONS));
  IFDEBUG(_serial->printf("Last chipID value = 0x%02x\n", (int) pgm_read_word(&regions[SIZE_OF_REGIONS - 1].chipID)));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void LEDdisplay::printSegs() {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s()\n", __func__));

  IFDEBUG(_serial->printf("  ledSegs[dd].startPos, endPos, buttonID =  \n"));
  for (int idx = 0; idx < SIZE_OF_LEDSEGS; idx++) {
    IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&ledSegs[idx].startPos), (int) pgm_read_word(&ledSegs[idx].endPos), ((int) pgm_read_word(&ledSegs[idx].buttonID)), regions[((int) pgm_read_word(&ledSegs[idx].buttonID))].name));
  }
  IFDEBUG(_serial->printf("Number of ledSegs = %d\n", SIZE_OF_LEDSEGS));
  IFDEBUG(_serial->printf("Last endpos value = %d\n", (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos)));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void LEDdisplay::advanceCurrentLed(int stepPos, int incr) {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s(%d)\n", __func__, incr));

  IFDEBUG(_serial->printf("  ledSegs[dd].startPos, endPos, buttonID =  \n"));
  IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", stepPos, (int) pgm_read_word(&ledSegs[stepPos].startPos), (int) pgm_read_word(&ledSegs[stepPos].endPos), ((int) pgm_read_word(&ledSegs[stepPos].buttonID)), regions[((int) pgm_read_word(&ledSegs[stepPos].buttonID))].name));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
}

void LEDdisplay::printRingSegs() {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s()\n", __func__));

  IFDEBUG(_serial->printf("  RingSegs[dd].startPos, endPos, buttonID =  \n"));
  for (int idx = 0; idx < SIZE_OF_RINGSEGS; idx++) {
    IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&RingSegs[idx].startPos), (int) pgm_read_word(&RingSegs[idx].endPos), ((int) pgm_read_word(&RingSegs[idx].buttonID)), regions[((int) pgm_read_word(&RingSegs[idx].buttonID))].name));
  }

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void LEDdisplay::colorFillRange(unsigned long color, int beginPos, int endPos) {

  #ifdef IFDEBUG
    //IFDEBUG(_serial->printf("Running - LEDdisplay::%s(begin=%d, end=%d, color=0x%06x)\n", __func__, beginPos, endPos, color)); // has error in unsigned long
    _serial->print("Running - LEDdisplay::"); _serial->print(__func__);
      _serial->print("(0x"); _serial->print(color, HEX);
      _serial->print(", "); _serial->print(beginPos);
      _serial->print(", "); _serial->print(endPos);
      _serial->println(")");
  #endif

  IFDEBUG_LED(_serial->printf("  setting LEDs: "));
  for (int pos = (beginPos - 1); pos < endPos; pos++) { // For each pixel in strip...
    IFDEBUG_LED(_serial->printf("%d, ", pos));
    Adafruit_NeoPixel::setPixelColor(pos, color);             //  Set pixel's color (in RAM)
  }
  IFDEBUG_LED(_serial->printf("\n"));
  Adafruit_NeoPixel::show();                                  //  Update strip to match

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
}

void LEDdisplay::colorFillAll(unsigned long color) {
  //IFDEBUG(IFDEBUG(_serial->printf("Running - LEDdisplay::%s(0x%06x)\n", __func__, color)); // has error in unsigned long
#ifdef IFDEBUG
  _serial->print("Running - LEDdisplay::"); _serial->print(__func__);
  _serial->print("(0x"); _serial->print(color, HEX);
  _serial->println(")");
#endif

  colorFillRange(color, 0, Adafruit_NeoPixel::numPixels());

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
}

LedSegments LEDdisplay::findRegionsLedRange(Countries region) {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s(%d)\n", __func__, region));

  LedSegments response = { 0, 0, None };

  IFDEBUG(_serial->printf("  Scanning ledSegs[]...\n"));
  IFDEBUG_LED(_serial->printf("  ledSegs[dd].startPos, endPos, buttonID =  \n"));
  for (int idx = 0; idx < LENGTH_OF_ARRAY(ledSegs); idx++) {
    Countries ledSegOffset = ((int) pgm_read_word(&ledSegs[idx].buttonID));
    //IFDEBUG_LED(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&ledSegs[idx].startPos), (int) pgm_read_word(&ledSegs[idx].endPos), (int) pgm_read_word(&ledSegs[idx].buttonID), regions[ledSegOffset].name));
    if (ledSegOffset == region) {
      IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&ledSegs[idx].startPos), (int) pgm_read_word(&ledSegs[idx].endPos), (int) pgm_read_word(&ledSegs[idx].buttonID), regions[ledSegOffset].name));
      response.startPos = (int) pgm_read_word(&ledSegs[idx].startPos);
      response.endPos = (int) pgm_read_word(&ledSegs[idx].endPos);
      response.buttonID = idx;
      IFDEBUG(_serial->printf("  length of segment(inclusive) = %d \n", (response.endPos - response.startPos + 1)));
      break;
    }
  }

  if (response.buttonID == None) {
    IFDEBUG(_serial->printf("No match for region was found\n"));
  }

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
  return response;
}
