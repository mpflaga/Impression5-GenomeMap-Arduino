/**
  \file Impression5-GenomeMap.ino
  \brief Main Arduino sketch for Impression 5s Genome Migration Map Interactive Game.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@.flaga.net
*/

#include <EEPROM.h>
#define EEPROMLEDBRIGHTADDRESS 0x0

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

#include "RFid.h"
Pins ldrPins =
  // {Signal Pin, GND Pin, Threshold}
  // Note - GND = 0 assumes LDR is connected to real ground.
{A6, A7 };

RFid RFid(ldrPins, (int) 10);

// instanciate Driver to monitor array of photocells
// PhotoCellNumber PhotoCellNumber(ldrPins, sizeof(ldrPins) / sizeof(ldrPins[0]), 10);
// int value_prv = 0; // value to monitor change in photo cells composite number


void setup()
{
  int maxBrightness = EEPROM.read(EEPROMLEDBRIGHTADDRESS);
  //EEPROM.update(EEPROMLEDBRIGHTADDRESS, (int) 50); // MPF to set for Lab purposes

  //Serial.begin(115200);
  Serial.begin(500000);
  Serial.print(F("Free RAM after Serial Started is = ")); Serial.println(freeMemory());
  //while(!Serial);  // only needed if you want serial feedback with the
  // Arduino Leonardo or Bare Touch Board
  Serial.println();
  Serial.print(F("Running - ")); Serial.print(__func__); Serial.println(F("()"));

  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);

  Serial.print(F("Instanced ")); Serial.print(lastLED); Serial.println(F(" LED NeoPixel strip."));

  led = new LEDdisplay(lastLED, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  led->begin(maxBrightness);
  
 if (maxBrightness < 255) {
   // then in Demo Lab 
    led->testAllLEDs(); // 1/4s for R, G and B.
 } 
 else {
   // I5 - requested this.
   led->colorFillAll(led->Color( WHITE ));
   delay(10000);
   led->colorFillAll(led->Color( OFF ));
 }
 led->colorFillAll(led->Color( OFF ));

  game = new MigrationGame(maxBrightness);
  game->begin(led, Serial);

#if 0  // print out all data structures (if debug is enabled).
  game->printPlantsWithLED();
  led->printRegions();
#endif

  tpad.begin(MPR121_INT);
  Serial1.begin(9600);
  RFid.begin(Serial1);

  randomSeed(analogRead(8));

  // print build stat's.
  Serial.print(F("Build Date: ")); Serial.print(F(__DATE__)); Serial.print(F(" ")); Serial.println(F(__TIME__));
  Serial.print(F("Free RAM at end of setup() is = ")); Serial.println(freeMemory());
  Serial.print(F("Ending  - ")); Serial.print(__func__); Serial.println(F("()"));

  Serial.println(F("Enter in State Change?"));
}

void loop()
{

  int nextRegionIdx = tpad.scan();
  if (nextRegionIdx > 0) {
    game->updateRegion(nextRegionIdx);
    serial.printf("TouchPad accepted entered Region index %d %p(%d)\n", nextRegionIdx, regions[game->region[0]], game->region[0]);
  }

  if ( RFid.available() ) {
    int nextPlantIdx = RFid.read();
    Serial.print(F("Detected Card ID = ")); Serial.println(nextPlantIdx);
    Serial.print(F("SIZE_OF_PLANTS = ")); Serial.println(SIZE_OF_PLANTS);
    if (( -1 < nextPlantIdx ) && (nextPlantIdx < SIZE_OF_PLANTS)) {
      if (game->updatePlant(nextPlantIdx)) {
        // when a different plant, update settings
        Serial.println(F("Plant Updated."));

      } else {
        // when same plant, do nothing
        Serial.println(F("No Change in Plant."));
      }
    }
    else if ( nextPlantIdx < 1 ) {
      nextPlantIdx = 0;
      Serial.println(F("Plant Card Removed."));
      game->updatePlant(nextPlantIdx);
    }
    else if (( -1 < nextPlantIdx ) && ( nextPlantIdx >= SIZE_OF_PLANTS )) {
      Serial.print(F("Diagnostic Card Detected = ")); Serial.println(nextPlantIdx);
      game->updatePlant(nextPlantIdx);
    }
    Serial.print(F("Changed plant to ")); Serial.println(nextPlantIdx);

  }

  String response = getConsole();
  if (response.length() > 0) {
    Serial.print(F("Console has recieved = ")); Serial.println(response);
    Serial.println(F("Enter in State Change?"));
  }

  game->checkGameStateMachine();
  
  if ( game->gameState[0] == GAME_STOP ) {

    // MPF - WIP, need to add 1 second updates delays. 
    // then draw Ring to match tthresh
    // thne move to class function.

    Serial.print("GAME_STOP: ");
    if ( game->plant[0] == 11 ) {
      //led->colorFillAllRegions( led->Color( YELLOW ) );
      Serial.println( "Thouch Threshold " );


      for (int idx = 0; idx < LENGTH_OF_ARRAY(ledSegs); idx++) {
        Countries ledSegOffset = ((int) pgm_read_word(&ledSegs[idx].buttonID));
        if (ledSegOffset != None) {
          LedSegments segment;
          segment.startPos = (int) pgm_read_word(&ledSegs[idx].startPos);
          segment.endPos = (int) pgm_read_word(&ledSegs[idx].endPos);
          led->colorFillRange( led->Color( YELLOW ), segment.startPos, segment.endPos, false);
        }
      }
      led->show();

      for (int touchIdx = 0; touchIdx < ( SIZE_OF_CHIPS * numElectrodes ); touchIdx++) {
        uint8_t chipIdx = touchIdx / numElectrodes;
        uint8_t electrodeIdx = touchIdx % numElectrodes;
        
        Serial.print( "touchIdx = " );Serial.print( touchIdx );
        Serial.print( " touchIdx / numElectrodes = " );Serial.print( chipIdx );
        Serial.print( " touchIdx % numElectrodes = " );Serial.print( electrodeIdx );
        
        uint8_t tthresh = tpad.getTouchThreshold(chipIdx, electrodeIdx);
        Serial.print( " tthresh = " );Serial.print( tthresh );
        uint8_t rthresh = tpad.getReleaseThreshold(chipIdx, electrodeIdx);
        Serial.print( " rthresh = " );Serial.print( rthresh );

        
        Serial.println();
      }

    }
    Serial.println();
  }
  
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

    } else if (consoleInputStr == "<") {
      // Store a dim value into the EEprom
        EEPROM.write(EEPROMLEDBRIGHTADDRESS, 15);
        Serial.println(F("Changed EEPROM to minimal brightness"));

    } else if (consoleInputStr == ">") {
      // Store a the max(full) value into the EEprom
        EEPROM.write(EEPROMLEDBRIGHTADDRESS, 255);
        Serial.println(F("Changed EEPROM to max brightness"));

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
      
      if (( -1 < game->plant[0] ) && ( game->plant[0] < SIZE_OF_PLANTS )) {
        serial.printf("  plant[%d] = %p(%d)\n", 0, plants[game->plant[0]].plantName, game->plant[0]);
      }
      else {
        serial.printf("  plant[%d] = id# %d\n", 0, game->plant[0]);
      }      
      
      Serial.print(F("Free RAM = ")); Serial.println(freeMemory());
      Serial.print(F("RFid.average = ")); Serial.println(RFid.average);
      Serial.print(F("RFid.threshold = ")); Serial.println(RFid.threshold);
      
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
      Serial.println(F("  < - set EEPROM to dim Max LEDs"));
      Serial.println(F("  > - set EEPROM to full Max LEDs"));
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
        Serial.println(F("No Change"));
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
