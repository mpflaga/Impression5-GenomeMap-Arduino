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
class LEDdisplay {

  public:

    LEDdisplay();
    ~LEDdisplay();
    int currentLedPos;
    int currentLedOffset;
    int displaySize;
    void begin(Adafruit_NeoPixel *strip);
    void begin(Adafruit_NeoPixel *strip, Stream &serial);
    void printRegions();
    void printSegs();
    void printRingSegs();
    void advanceCurrentLed(int stepPos, int incr = 1);
    void colorFillRange(unsigned long color, int beginPos, int endPos);
    void colorFill(unsigned long color);
    LedSegments findRegionsLedRange(Countries region);

  private:
    StreamEx* _serial;

    Adafruit_NeoPixel* _strip;
    unsigned long previousMillis = 0;        // will store last time LED was updated
    const long interval = 100;               // interval at which to blink (milliseconds)


};

#endif
