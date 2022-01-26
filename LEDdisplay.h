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
#define DEBUG_LEVEL_LED 0 // set to 1 to compile in Serial Debug prints
#endif

#ifndef LEDdisplay_h
#define LEDdisplay_h

#include <Adafruit_NeoPixel.h>
#include "MigrationData.h"

#define RED     255,   0,   0
#define ORANGE  255,  34,   0
#define YELLOW  255, 255,   0
#define GREEN     0, 255,   0
#define CYAN      0, 255, 255
#define BLUE      0,   0, 255
#define VIOLET  153,   0, 255
#define MAGENTA 255,   0,  51
#define PINK    255,  51, 119
#define AQUA     85, 125, 255
#define WHITE   255, 255, 255
#define OFF       0,   0,   0

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
    void begin(int maxBrightness_ = 255);
    void colorFillRange(unsigned long color, int beginPos, int endPos);
    void colorFillAll(unsigned long color);
    LedSegments findRegionsLedRange(Countries region);
    void testAllLEDs(int wait = 250);
    void toggleAllLedsWhite();
    void colorWipe(uint32_t color, int wait);
    void theaterChase(uint32_t color, int wait);
    void rainbow(int wait);
    void theaterChaseRainbow(int wait);
    void showCaseMode();

  private:
    unsigned long previousMillis = 0;        // will store last time LED was updated
    const long interval = 100;               // interval at which to blink (milliseconds)


};

#endif
