/**
  \file LEDdisplay.h
  \brief LED Driver of NeoPixels in 7 segment arrangement.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@flaga.net
*/

#ifndef PRINTEX_VERSION
#include <PrintEx.h>
#endif

#ifndef IFDEBUG
#define IFDEBUG(...) ((void)((DEBUG_LEVEL) && (__VA_ARGS__, 0)))
#define DEBUG_LEVEL 1 // set to 1 to compile in Serial Debug prints
#endif

#ifndef IFDEBUG_LED
#define IFDEBUG_LED(...) ((void)((DEBUG_LEVEL_LED) && (__VA_ARGS__, 0)))
#define DEBUG_LEVEL_LED 1 // set to 1 to compile in Serial Debug prints
#endif

#ifndef LEDdisplay_h
#define LEDdisplay_h

#include <Adafruit_NeoPixel.h>
#include "MigrationData.h"

/**
   \class LEDdisplay
   \brief Interface Driver to the f NeoPixels in 7 segment arrangement.
*/
class LEDdisplay : public Adafruit_NeoPixel {

  public:

    LEDdisplay(uint16_t n, uint8_t p = 6, neoPixelType t = NEO_GRB + NEO_KHZ800);
    ~LEDdisplay();
    int currentLedPos;
    int currentLedOffset;
    int displaySize;
    void begin();
    void begin(Stream &serial);
    void printRegions();
    void printSegs();
    void advanceCurrentLed(int stepPos, int incr = 1);
    void colorFillRange(unsigned long color, int beginPos, int endPos);
    void colorFillAll(unsigned long color);
    LedSegments findRegionsLedRange(Countries region);
    void testAllLEDs();
    void toggleAllLedsWhite();
    void colorWipe(uint32_t color, int wait);
    void theaterChase(uint32_t color, int wait);
    void rainbow(int wait);
    void theaterChaseRainbow(int wait);
    void showCaseMode();

  private:
    StreamEx* _serial;

    unsigned long previousMillis = 0;        // will store last time LED was updated
    const long interval = 100;               // interval at which to blink (milliseconds)


};

#endif
