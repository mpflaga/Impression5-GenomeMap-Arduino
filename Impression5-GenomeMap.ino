/**
  \file Impression5-GenomeMap.ino
  \brief Main Arduino sketch for Impression 5s Genome Migration Map Interactive Game.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@.flaga.net
*/

#include <Adafruit_NeoPixel.h>
#define LED_PIN    11

#include "MigrationData.h"

#include "LEDdisplay.h"
LEDdisplay* led;

#include "MigrationGame.h"
MigrationGame* game;

StreamEx serial = Serial;

#include "tpad.h"
const uint8_t MPR121_INT = 12;  // pin 4 is the MPR121 interrupt on the Bare Touch Board

tpad tpad;

void setup()
{
  //Serial.begin(115200);
  Serial.begin(500000);
  //while(!Serial);  // only needed if you want serial feedback with the
  // Arduino Leonardo or Bare Touch Board
  Serial.println();
  Serial.print(F("Running - ")); Serial.print(__func__); Serial.println("()");

  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);

  Serial.print(F("Instanced ")); Serial.print(lastLED); Serial.println(F(" LED NeoPixel strip."));

  led = new LEDdisplay(lastLED, LED_PIN, NEO_GRB + NEO_KHZ800);
  led->begin(Serial);

  led->testAllLEDs();

  game = new MigrationGame();
  game->begin(led, Serial);

#if 0  // print out all data structures (if debug is enabled).
  game->printPlantsWithLED();
  led->printRegions();
  led->printSegs();
  led->printRingSegs();
#endif

  tpad.begin(Serial, MPR121_INT);

  // print build stat's.
  Serial.print(F("Build Date: ")); Serial.print(F(__DATE__)); Serial.print(F(" ")); Serial.println(F(__TIME__));
  serial.printf("game->gameState[0] = '%d'(%p)\n", game->gameState[0], stateStr[game->gameState[0]]);

  Serial.println(F("Enter in State Change?"));

  Serial.print(F("Ending  - ")); Serial.print(__func__); Serial.println("()");
}

void loop()
{

  int value = tpad.scan();

  String response = getConsole();
  if (response.length() > 0) {
    Serial.print(F("Console has recieved = ")); Serial.println(response);
    Serial.println(F("Enter in State Change?"));
  }

  game->checkGameStateMachine();
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
      game->printPlants();
      Serial.println();
      game->printRegions();

    } else if (consoleInputStr == "T") {
      // Test Pattern of the LEDs
      led->testAllLEDs();

    } else if (consoleInputStr == "W") {
      // Toggle the LEDs all White
      led->toggleAllLedsWhite();

    } else if (consoleInputStr == "S") {
      // Toggle the LEDs all White
      led->showCaseMode();

    } else if (consoleInputStr == "H") {
      // list out histories
      game->printRegionHistory();
      game->printPlantHistory();
      serial.printf("  hopPos  = %d\n", game->hopPos);
      serial.printf("  stepPos = %d\n", game->stepPos);
      serial.printf("  game->prv_gameState= '%p'(%d)\n", stateStr[game->prv_gameState], game->prv_gameState);
      game->printGameStateHistory();

    } else if (consoleInputStr == "G") {
      // list out history of gameState[]
      Serial.println("History of gameState[] :");
      serial.printf("  game->prv_gameState = '%p'(%d)\n", stateStr[game->prv_gameState], game->prv_gameState);
      for (int idx = 0; idx < LENGTH_OF_ARRAY(game->gameState); idx++) {
        serial.printf("  gameState[%d]  = '%p'(%d)\n", idx, stateStr[game->gameState[idx]], game->gameState[idx]);
      }
      serial.printf("  plant[%d] = %p(%d)\n", 0, plants[game->plant[0]].plantName, game->plant[0]);

    } else if (consoleInputStr == "R") {
      game->printCurrentDesiredRegion();

    } else if (consoleInputStr == "?") {
      // print help describing commands.
      Serial.println(F("Help..."));
      Serial.println(F("  D - print list of Plants and Regions"));
      Serial.println(F("  H - print history Plants and Regions"));
      Serial.println(F("  G - print current state of Game"));
      Serial.println(F("  T - Test the RGB of the LEDs"));
      Serial.println(F("  S - Show Case a Demo Sequence of LEDs"));
      Serial.println(F("  W - Toggle all the LEDs White or Off"));
      // Serial.println(F("  L - print current LED Segment Status"));
      // Serial.println(F("  R - print next regions."));
      Serial.println(F("  0..7 - select corresponding plant"));
      Serial.println(F("  A..Z - select matching region"));
      Serial.println();

    } else if ( isDigit(consoleInputStr.charAt(0)) ) {
      // cross reference 0..9 to find matching plant
      int nextPlantIdx = game->lookforPlant(consoleInputStr.toInt());
      if (nextPlantIdx >= 0) {
        // when positive value will be index of plant.
        if (game->updatePlant(nextPlantIdx)) {
          // when a different plant, update settings
          Serial.println(F("Plant Updated"));

        } else {
          // when same plant, do nothing
          Serial.println(F("No Change in Plant"));
        }
      }
    } else {
      // Check if text matches new Region (aka NEW HOP)!
      int nextRegionIdx = game->lookforRegion(&consoleInputStr);
      if (nextRegionIdx < 0) {
        Serial.print(F("No Change"));
      } else {
        game->updateRegion(nextRegionIdx);
        serial.printf("Accepted entered Region %p(%d)\n", regions[game->region[0]], game->region[0]);
      }
    }
    response = consoleInputStr;
  }

  //  Serial.print(F("Ending  - "));Serial.print(__func__);Serial.println("()");
  return response;
}
