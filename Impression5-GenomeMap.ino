/**
  \file Impression5-GenomeMap.ino
  \brief Main Arduino sketch for Impression 5s Genome Migration Map Interactive Game.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@.flaga.net
*/

#include <Adafruit_NeoPixel.h>
#define LED_PIN    11
Adafruit_NeoPixel* strip;

#include "MigrationData.h"

#include "LEDdisplay.h"
LEDdisplay* ledDisplay;

#include "MigrationGame.h"
MigrationGame* m;

StreamEx serial = Serial;

void setup()
{
  //Serial.begin(115200);
  Serial.begin(500000);
  //while(!Serial);  // only needed if you want serial feedback with the
  // Arduino Leonardo or Bare Touch Board
  Serial.println();
  Serial.print(F("Running - ")); Serial.print(__func__); Serial.println("()");

  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);

  strip = new Adafruit_NeoPixel(lastLED, LED_PIN, NEO_GRB + NEO_KHZ800);
  strip->begin();
  strip->setBrightness(255 * .9);
  strip->clear();
  strip->show();
  Serial.print(F("Instanced ")); Serial.print(lastLED); Serial.println(F(" LED NeoPixel strip."));

  ledDisplay = new LEDdisplay();
  ledDisplay->begin(strip, Serial);

  m = new MigrationGame();
  m->begin(strip, ledDisplay, Serial);

#if 0  // print out all data structures (if debug is enabled).
  m->printPlantWithLED();
  ledDisplay->printRegions();
  ledDisplay->printSegs();
  ledDisplay->printRingSegs();
#endif

  // print build stat's.
  Serial.print(F("Build Date: ")); Serial.print(F(__DATE__)); Serial.print(F(" ")); Serial.println(F(__TIME__));
  serial.printf("m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);

  Serial.println(F("Enter in State Change?"));

  Serial.print(F("Ending  - ")); Serial.print(__func__); Serial.println("()");
}

void loop()
{

  String response = getConsole();
  if (response.length() > 0) {
    Serial.print(F("Console has recieved = ")); Serial.println(response);
    Serial.println(F("Enter in State Change?"));
  }

  m->checkGameStateMachine();
}

String getConsole() {
  String response;
  //  Serial.print(F("Running - "));Serial.print(__func__);Serial.println("()");

  if (Serial.available() > 0)
  {
    // get Serial Input
    String consoleInputStr = Serial.readStringUntil('\n');
    // clean up serial input
    consoleInputStr.trim();
    consoleInputStr.toUpperCase();
    Serial.print(F("You have entered = '")); Serial.print(consoleInputStr); Serial.println("'");

    // parse out commands
    if (consoleInputStr == "") {
      // Just print Linefeed
      Serial.println();

    } else if (consoleInputStr == "D") {
      // list out plants and regions
      m->printPlants();
      Serial.println();
      m->printRegions();

    } else if (consoleInputStr == "H") {
      // list out histories
      m->printRegionHistory();
      m->printPlantHistory();
      serial.printf("  hopPos  = %d\n", m->hopPos);
      serial.printf("  stepPos = %d\n", m->stepPos);
      serial.printf("  m->prv_gameState= '%p'(%d)\n", stateStr[m->prv_gameState], m->prv_gameState);
      m->printGameStateHistory();

    } else if (consoleInputStr == "G") {
      // list out history of gameState[]
      Serial.println("History of gameState[] :");
      serial.printf("  m->prv_gameState = '%p'(%d)\n", stateStr[m->prv_gameState], m->prv_gameState);
      for (int idx = 0; idx < LENGTH_OF_ARRAY(m->gameState); idx++) {
        serial.printf("  gameState[%d]  = '%p'(%d)\n", idx, stateStr[m->gameState[idx]], m->gameState[idx]);
      }
      serial.printf("  plant[%d] = %p(%d)\n", 0, plants[m->plant[0]].plantName, m->plant[0]);

    } else if (consoleInputStr == "R") {
      m->printCurrentDesiredRegion();

    } else if (consoleInputStr == "?") {
      // print help describing commands.
      Serial.println(F("Help..."));
      Serial.println(F("  D - print list of Plants and Regions"));
      Serial.println(F("  H - print history Plants and Regions"));
      Serial.println(F("  G - print current state of Game"));
      // Serial.println(F("  L - print current LED Segment Status"));
      // Serial.println(F("  R - print next regions."));
      Serial.println(F("  0..7 - select corresponding plant"));
      Serial.println(F("  A..Z - select matching region"));
      Serial.println();

    } else if ( isDigit(consoleInputStr.charAt(0)) ) {
      // cross reference 0..9 to find matching plant
      int nextPlantIdx = m->lookforPlant(consoleInputStr.toInt());
      if (nextPlantIdx >= 0) {
        // when positive value will be index of plant.
        if (m->updatePlant(nextPlantIdx)) {
          // when a different plant, update settings
          Serial.println(F("Plant Updated"));

        } else {
          // when same plant, do nothing
          Serial.println(F("No Change in Plant"));
        }
      }
    } else {
      // Check if text matches new Region (aka NEW HOP)!
      int nextRegionIdx = m->lookforRegion(&consoleInputStr);
      if (nextRegionIdx < 0) {
        Serial.print(F("No Change"));
      } else {
        m->updateRegion(nextRegionIdx);
        serial.printf("Accepted entered Region %p(%d)\n", regions[m->region[0]].name, m->region[0]);
      }
    }
    response = consoleInputStr;
  }

  //  Serial.print(F("Ending  - "));Serial.print(__func__);Serial.println("()");
  return response;
}
