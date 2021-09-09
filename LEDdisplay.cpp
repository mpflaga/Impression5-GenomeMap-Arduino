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

  IFDEBUG(_serial->printf("  regions[dd] =  \n"));
  for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    IFDEBUG(_serial->printf("      ..[%02d] = '%p'\n", idx, regions[idx]));
  }
  IFDEBUG(_serial->printf("Number of regions = %d\n", SIZE_OF_REGIONS));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void LEDdisplay::printSegs() {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s()\n", __func__));

  IFDEBUG(_serial->printf("  ledSegs[dd].startPos, endPos, buttonID =  \n"));
  for (int idx = 0; idx < SIZE_OF_LEDSEGS; idx++) {
    IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&ledSegs[idx].startPos), (int) pgm_read_word(&ledSegs[idx].endPos), ((int) pgm_read_word(&ledSegs[idx].buttonID)), regions[((int) pgm_read_word(&ledSegs[idx].buttonID))]));
  }
  IFDEBUG(_serial->printf("Number of ledSegs = %d\n", SIZE_OF_LEDSEGS));
  IFDEBUG(_serial->printf("Last endpos value = %d\n", (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos)));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void LEDdisplay::advanceCurrentLed(int stepPos, int incr) {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s(%d)\n", __func__, incr));

  IFDEBUG(_serial->printf("  ledSegs[dd].startPos, endPos, buttonID =  \n"));
  IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", stepPos, (int) pgm_read_word(&ledSegs[stepPos].startPos), (int) pgm_read_word(&ledSegs[stepPos].endPos), ((int) pgm_read_word(&ledSegs[stepPos].buttonID)), regions[((int) pgm_read_word(&ledSegs[stepPos].buttonID))]));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
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

  colorFillRange(color, (0 + 1), (Adafruit_NeoPixel::numPixels() + 1));

  IFDEBUG(_serial->printf("Ending  - LEDdisplay::%s()\n", __func__));
}

LedSegments LEDdisplay::findRegionsLedRange(Countries region) {
  IFDEBUG(_serial->printf("Running - LEDdisplay::%s(%d)\n", __func__, region));

  LedSegments response = { 0, 0, None };

  IFDEBUG(_serial->printf("  Scanning ledSegs[]...\n"));
  IFDEBUG_LED(_serial->printf("  ledSegs[dd].startPos, endPos, buttonID =  \n"));
  for (int idx = 0; idx < LENGTH_OF_ARRAY(ledSegs); idx++) {
    Countries ledSegOffset = ((int) pgm_read_word(&ledSegs[idx].buttonID));
    //IFDEBUG_LED(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&ledSegs[idx].startPos), (int) pgm_read_word(&ledSegs[idx].endPos), (int) pgm_read_word(&ledSegs[idx].buttonID), regions[ledSegOffset]));
    if (ledSegOffset == region) {
      IFDEBUG(_serial->printf("       ..[%02d] = %04d, %04d, %04d, '%p'\n", idx, (int) pgm_read_word(&ledSegs[idx].startPos), (int) pgm_read_word(&ledSegs[idx].endPos), (int) pgm_read_word(&ledSegs[idx].buttonID), regions[ledSegOffset]));
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

void LEDdisplay::testAllLEDs() {
  // Testing all Neopixels
  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);
  colorFillAll(Color( 255, 0, 0));
  delay(250);
  colorFillAll(Color( 0, 255, 0));
  delay(250);
  colorFillAll(Color( 0, 0, 255));
  delay(250);
  colorFillAll(Color( 0, 0,   0));
}

#define DIM_FACTOR 4
void LEDdisplay::toggleAllLedsWhite() {
  // Testing all Neopixels
  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);
  if (getPixelColor(lastLED - 1) > 0 ) {
    colorFillRange(Color( 0, 0, 0), 1, lastLED);
  } else {
    colorFillRange(Color( 255 / DIM_FACTOR, 255 / DIM_FACTOR, 255 / DIM_FACTOR), 1, lastLED);
  }
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void LEDdisplay::colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < Adafruit_NeoPixel::numPixels(); i++) { // For each pixel in strip...
    Adafruit_NeoPixel::setPixelColor(i, color);         //  Set pixel's color (in RAM)
    if (wait > 0) {
      Adafruit_NeoPixel::show();                          //  Update strip to match
      delay(wait);                           //  Pause for a moment
    }
  }
  if (wait < 1) {
    Adafruit_NeoPixel::show();                          //  Update strip to match
    delay(2000);
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void LEDdisplay::theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      Adafruit_NeoPixel::clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < Adafruit_NeoPixel::numPixels(); c += 3) {
        Adafruit_NeoPixel::setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      Adafruit_NeoPixel::show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void LEDdisplay::rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < Adafruit_NeoPixel::numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / Adafruit_NeoPixel::numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      Adafruit_NeoPixel::setPixelColor(i, Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue)));
    }
    Adafruit_NeoPixel::show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void LEDdisplay::theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      Adafruit_NeoPixel::clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < Adafruit_NeoPixel::numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (Adafruit_NeoPixel::numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / Adafruit_NeoPixel::numPixels();
        uint32_t color = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(hue)); // hue -> RGB
        Adafruit_NeoPixel::setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      Adafruit_NeoPixel::show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void LEDdisplay::showCaseMode() {
  while (1) {
    colorWipe(Color(255,   0,   0), 0); // Red
    if (Serial.available()) {
      break;
    }
    colorWipe(Color(  0, 255,   0), 0); // Green
    if (Serial.available()) {
      break;
    }
    colorWipe(Color(  0,   0, 255), 0); // Blue
    if (Serial.available()) {
      break;
    }
    //        colorWipe(Color(127, 127, 127), 0); // white
    //        if(Serial.available()) {
    //          break;
    //        }

    // Do a theater marquee effect in various colors...
    theaterChase(Color(127, 127, 127), 0); // White, half brightness
    if (Serial.available()) {
      break;
    }
    theaterChase(Color(127,   0,   0), 0); // Red, half brightness
    if (Serial.available()) {
      break;
    }
    theaterChase(Color(  0,   0, 127), 0); // Blue, half brightness
    if (Serial.available()) {
      break;
    }

    rainbow(10);             // Flowing rainbow cycle along the whole strip
    if (Serial.available()) {
      break;
    }
    theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
    if (Serial.available()) {
      break;
    }
  }
}
