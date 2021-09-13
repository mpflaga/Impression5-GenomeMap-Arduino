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

}

/**
   \brief Destructortructor

   release alocated memory ,if any.
*/
LEDdisplay::~LEDdisplay() {
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

void LEDdisplay::colorFillRange(unsigned long color, int beginPos, int endPos) {

  for (int pos = (beginPos - 1); pos < endPos; pos++) { // For each pixel in strip...
    Adafruit_NeoPixel::setPixelColor(pos, color);             //  Set pixel's color (in RAM)
  }
  Adafruit_NeoPixel::show();                                  //  Update strip to match
}

void LEDdisplay::colorFillAll(unsigned long color) {

  colorFillRange(color, (0 + 1), (Adafruit_NeoPixel::numPixels() + 1));
}

LedSegments LEDdisplay::findRegionsLedRange(Countries region) {
  LedSegments response = { 0, 0, None };

  for (int idx = 0; idx < LENGTH_OF_ARRAY(ledSegs); idx++) {
    Countries ledSegOffset = ((int) pgm_read_word(&ledSegs[idx].buttonID));
    if (ledSegOffset == region) {
      response.startPos = (int) pgm_read_word(&ledSegs[idx].startPos);
      response.endPos = (int) pgm_read_word(&ledSegs[idx].endPos);
      response.buttonID = idx;
      break;
    }
  }

  if (response.buttonID == None) {
  }

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
