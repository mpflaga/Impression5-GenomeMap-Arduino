/**
  \file Impression5-GenomeMap.ino
  \brief Main Arduino sketch for Impression 5s Genome Migration Map Interactive Game.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@.flaga.net
*/

#include "MigrationData.h"

#include "LEDdisplay.h"
LEDdisplay* led;
#define NEOPIXEL_PIN    11

#include "MigrationGame.h"
MigrationGame* game;

StreamEx serial = Serial;

#include "tpad.h"
const uint8_t MPR121_INT = 12;  // pin 4 is the MPR121 interrupt on the Bare Touch Board

tpad tpad;

#include "PhotoCellNumber.h"
Pins ldrPins[] = {
  // {Signal Pin, GND Pin, Threshold}
  // Note - GND = 0 assumes LDR is connected to real ground.
  {A0, A1, 110},
  {A2, A3, 110},
  {A4, A5, 110}
};

// instanciate Driver to monitor array of photocells
PhotoCellNumber PhotoCellNumber(ldrPins, sizeof(ldrPins) / sizeof(ldrPins[0]), 10);
int value_prv = 0; // value to monitor change in photo cells composite number


void setup()
{
  //Serial.begin(115200);
  Serial.begin(500000);
  //while(!Serial);  // only needed if you want serial feedback with the
  // Arduino Leonardo or Bare Touch Board
  Serial.println();
  Serial.print(F("Running - ")); Serial.print(__func__); Serial.println(F("()"));

  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);

  Serial.print(F("Instanced ")); Serial.print(lastLED); Serial.println(F(" LED NeoPixel strip."));

Serial.print(F("before LEDs (FR=")); Serial.print(freeMemory()); Serial.println(F(") "));
  led = new LEDdisplay(lastLED, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Serial.print(F("after LEDs new (FR=")); Serial.print(freeMemory()); Serial.println(F(") "));
  led->begin();
Serial.print(F("after LEDs begin (FR=")); Serial.print(freeMemory()); Serial.println(F(") "));

  led->testAllLEDs();
Serial.print(F("after LEDs test (FR=")); Serial.print(freeMemory()); Serial.println(F(") "));

  game = new MigrationGame();
Serial.print(F("after MigrationGame new (FR=")); Serial.print(freeMemory()); Serial.println(F(") "));
  game->begin(led, Serial);

#if 0  // print out all data structures (if debug is enabled).
  game->printPlantsWithLED();
  led->printRegions();
#endif

  tpad.begin(MPR121_INT);

  PhotoCellNumber.setInterval(50, 500); // set sample period and threshold for all bits.`
  PhotoCellNumber.ldr[0]->setThreshold(501); // set threshold for individual bits.
  PhotoCellNumber.ldr[1]->setThreshold(502);
  PhotoCellNumber.ldr[2]->setThreshold(503);

  PhotoCellNumber.setInvert(false); // invert all bits. default is false.
  // PhotoCellNumber.ldr[0]->setInvert(true); // invert individual bits.
  // PhotoCellNumber.ldr[1]->setInvert(true);
  // PhotoCellNumber.ldr[2]->setInvert(true);

  for (int i = 0; i < sizeof(ldrPins) / sizeof(ldrPins[0]); i++)
  {
    Serial.print(F("PhotoCellNumber.ldr[")); Serial.print(i); Serial.print(F("] = "));
    PhotoCellNumber.ldr[i]->PrintPins();
  }
  Serial.println();
  PhotoCellNumber.printArray();

  Serial.print(F("initial value = ")); Serial.println(PhotoCellNumber.value);
  value_prv = PhotoCellNumber.value;
#define PRINT_TIME_STAMPS 0 // 1 = ON, 0 = OFF
#define PRINT_VALUES 0
  PhotoCellNumber.setDebugMask( PRINT_TIME_STAMPS << 1 | PRINT_VALUES << 0 );

  // print build stat's.
  Serial.print(F("Build Date: ")); Serial.print(F(__DATE__)); Serial.print(F(" ")); Serial.println(F(__TIME__));
  serial.printf("game->gameState[0] = '%d'(%p)\n", game->gameState[0], stateStr[game->gameState[0]]);

  Serial.println(F("Enter in State Change?"));

  Serial.print(F("Ending  - ")); Serial.print(__func__); Serial.println(F("()"));
}

void loop()
{

  int nextRegionIdx = tpad.scan();
  if (nextRegionIdx > 0) {
    game->updateRegion(nextRegionIdx);
    serial.printf("TouchPad accepted entered Region index %d %p(%d)\n", nextRegionIdx, regions[game->region[0]], game->region[0]);
  }

  PhotoCellNumber.update();
  if (PhotoCellNumber.value != value_prv) {
    value_prv = PhotoCellNumber.value;

    int nextPlantIdx = -1;  //= game->lookforPlant(value_prv) + 1;
    for (int idx = 0; ((idx < SIZE_OF_PLANTS) && (nextPlantIdx < 0)); idx++) {
      if (PhotoCellNumber.value == (int) pgm_read_word(&plants[idx].placeCardID)) {
        nextPlantIdx = idx;
      }
    }
    if ( nextPlantIdx > -1)
    {
      Serial.print(F("PlantID = ")); Serial.println(nextPlantIdx);

      // when positive value will be index of plant.
      if (game->updatePlant(nextPlantIdx)) {
        // when a different plant, update settings
        Serial.println(F("Plant Updated"));

      } else {
        // when same plant, do nothing
        Serial.println(F("No Change in Plant"));
      }
    }
  }

  String response = getConsole();
  if (response.length() > 0) {
    Serial.print(F("Console has recieved = ")); Serial.println(response);
    Serial.println(F("Enter in State Change?"));
  }

  game->checkGameStateMachine();
}

String getConsole() {
  String response;
  //  Serial.print(F("Running - "));Serial.print(__func__);Serial.println(F("()"));

  if (Serial.available() > 0)
  {
    // get Serial Input
    String consoleInputStr = Serial.readStringUntil('\n');
    // clean up serial input
    consoleInputStr.trim();
    consoleInputStr.toUpperCase();
    Serial.print(F("You have entered = '")); Serial.print(consoleInputStr); Serial.println(F("'"));

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
      Serial.print(F("  hopPos  = ")); Serial.println(game->hopPos);
      Serial.print(F("  stepPos = ")); Serial.println(game->stepPos);
      serial.printf("  game->prv_gameState= '%p'(%d)\n", stateStr[game->prv_gameState], game->prv_gameState);
      game->printGameStateHistory();
      Serial.print(F("Free RAM = ")); Serial.println(freeMemory());

    } else if (consoleInputStr == "G") {
      // list out history of gameState[]
      Serial.println(F("History of gameState[] :"));
      serial.printf("  game->prv_gameState = '%p'(%d)\n", stateStr[game->prv_gameState], game->prv_gameState);
      for (int idx = 0; idx < LENGTH_OF_ARRAY(game->gameState); idx++) {
        serial.printf("  gameState[%d]  = '%p'(%d)\n", idx, stateStr[game->gameState[idx]], game->gameState[idx]);
      }
      serial.printf("  plant[%d] = %p(%d)\n", 0, plants[game->plant[0]].plantName, game->plant[0]);
      Serial.print(F("Free RAM = ")); Serial.println(freeMemory());
      
    } else if (consoleInputStr == "R") {
      game->printCurrentDesiredRegion();
      Serial.print(F("Free RAM = ")); Serial.println(freeMemory());

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
      Serial.print(F("Free RAM = ")); Serial.println(freeMemory());

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
        serial.printf("Console accepted entered Region index %d %p(%d)\n", nextRegionIdx, regions[game->region[0]], game->region[0]);
      }
    }
    response = consoleInputStr;
  }

  //  Serial.print(F("Ending  - "));Serial.print(__func__);Serial.println(F("()"));
  return response;
}
