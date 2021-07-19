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
LEDdisplay* l;

#include "MigrationGame.h"
MigrationGame* m;

StreamEx serial = Serial;
state_m prv_gameState;
int prv_region;

void setup()
{
  //Serial.begin(115200);
  Serial.begin(500000);
  //while(!Serial);  // only needed if you want serial feedback with the
  		   // Arduino Leonardo or Bare Touch Board
  Serial.println();
  Serial.print(F("Running - "));Serial.print(__func__);Serial.println("()");

  m = new MigrationGame();
  l = new LEDdisplay();

  int lastLED = (int) pgm_read_word(&ledSegs[SIZE_OF_LEDSEGS - 1].endPos);

  strip = new Adafruit_NeoPixel(lastLED, LED_PIN, NEO_GRB + NEO_KHZ800);
  strip->begin();
  strip->setBrightness(255 * .9);
  strip->clear();
  strip->show();
  Serial.print(F("Instanced ")); Serial.print(lastLED); Serial.println(F(" LED NeoPixel strip."));

  l->begin(strip, Serial);
  m->begin(Serial);

  // print out all data structures (if debug is enabled).
#if 0
  m->printPlantWithLED();
  l->printRegions();
  l->printSegs();
  l->printRingSegs();
#endif

  // print build stat's.
  Serial.print(F("Build Date: ")); Serial.print(F(__DATE__)); Serial.print(F(" ")); Serial.println(F(__TIME__));
  serial.printf("m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
  prv_gameState = m->gameState[0];
  prv_region = 0; // initialize as none.
  
  Serial.println(F("Enter in State Change?"));


  Serial.print(F("Ending  - "));Serial.print(__func__);Serial.println("()");
}

void loop()
{
  
  String response = getConsole();
  if (response.length() > 0) {
    Serial.print(F("Console has recieved = ")); Serial.println(response);
    Serial.println(F("Enter in State Change?"));
  }

  checkGameStateMachine();
}

void checkGameStateMachine() {
   // process the Game State Machine.

  // locally used in some state transistions. Not persistent.
  uint32_t currentLoopMillis = millis();
  LedSegments segment; 

  // print when the state is about to change.
  if (prv_gameState != m->gameState[0]) {
    serial.printf("Change of gameState from (%d)'%p' to (%d)'%p'\n", prv_gameState, stateStr[prv_gameState], m->gameState[0], stateStr[m->gameState[0]]);
    prv_gameState = m->gameState[0];
  }

  switch(m->gameState[0]) {
    case NO_PLANT_SELECTED:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      l->colorFill(strip->Color( 0, 0, 0));
      m->updateGameState(NO_PLANT_PRIMED);
      break;
      
    case NO_PLANT_PRIMED:
      //serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);

      break;
      
    case PLANT_INTIALLY_SELECTED:

      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      m->updateGameState(PLANT_INTIALLY_DIMMING);
      m->currentBrightness = 255;
      strip->setBrightness(m->currentBrightness);
      l->colorFill(strip->Color(  255, 255,   0)); // RED + GREEN = YELLOW
      m->ledDelayMillis = 100;
      m->ledNextMillis = uint32_t(currentLoopMillis + m->ledDelayMillis);
      m->ledStartMillis = m->ledNextMillis;

      serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
        serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
        serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
      break;

    case PLANT_INTIALLY_DIMMING:
      
      if (currentLoopMillis > m->ledNextMillis) {
        #if 1 // extra debug typically not needed.
          serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
          
          serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
            serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
            serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
        #endif
        
        m->ledNextMillis = uint32_t(currentLoopMillis + m->ledDelayMillis);
        m->currentBrightness = m->currentBrightness - (255 / 10);
        if (m->currentBrightness < 0) {
          m->currentBrightness = 0;
        }
        strip->setBrightness(m->currentBrightness);
        strip->show();
      }
      if (m->currentBrightness <= 0) {
        serial.print("currentLoopMillis = "); serial.println(currentLoopMillis);
          serial.print("duration = "); serial.print(currentLoopMillis - m->ledStartMillis);
          serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);

        m->currentBrightness = 255;;
        strip->setBrightness(m->currentBrightness);  // reset brightness for next use, but do not turn it on.
        m->updateGameState(PLANT_INTIALLY_DIMMING_FINISHED);
      }
      break;

    case PLANT_INTIALLY_DIMMING_FINISHED:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      m->updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      prv_region = m->history.region[0];  //0; // initialize as none, or current value.
      break;

    case PLANT_ACCEPTED_WAITING_FOR_BUTTON:
      if (prv_region != m->history.region[0]) {
        serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
        serial.printf("Accepting new Region %p(%d)\n", regions[m->history.region[0]].name, m->history.region[0]);
        prv_region = m->history.region[0];
        
        bool correctRegion = m->checkIfMatchCurrentDesiredRegions(m->history.region[0]); 
        serial.printf("Region is %s\n", correctRegion ? "correct" : "incorrect");
        
        if (correctRegion) {
          m->updateGameState(CORRECT_REGION_SELECTED);
        } else {
          m->updateGameState(INCORRECT_REGION_SELECTED);
        }
      }
      break;

    case INCORRECT_REGION_SELECTED:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      m->currentBrightness = 255;
      strip->setBrightness(m->currentBrightness);

      segment = l->findRegionsLedRange(m->history.region[0]);
      serial.printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
      l->colorFillRange(strip->Color(255, 0, 0), segment.startPos, segment.endPos);

      m->ledNextMillis = uint32_t(currentLoopMillis + 2000);
      m->ledStartMillis = m->ledNextMillis;

      serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
        serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
        serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
        m->updateGameState(INCORRECT_REGION_WAIT_TO_TURN_OFF);

      break;

    case INCORRECT_REGION_WAIT_TO_TURN_OFF:

      if (currentLoopMillis > m->ledNextMillis) {
        #if 1 // extra debug typically not needed.
          serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
          
          serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
            serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
            serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
        #endif

        segment = l->findRegionsLedRange(m->history.region[0]);
        serial.printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
        l->colorFillRange(strip->Color( 0, 0, 0), segment.startPos, segment.endPos);

        m->updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      }
      break;

    case CORRECT_REGION_SELECTED:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      m->currentBrightness = 255;
      strip->setBrightness(m->currentBrightness);

       segment = l->findRegionsLedRange(m->history.region[0]);
      serial.printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
      l->colorFillRange(strip->Color( 0, 255, 0), segment.startPos, segment.endPos);

      m->ledNextMillis = uint32_t(currentLoopMillis + 2000);
      m->ledStartMillis = m->ledNextMillis;

      serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
        serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
        serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
        m->updateGameState(CORRECT_REGION_WAIT_TO_TURN_OFF);

      break;

    case CORRECT_REGION_WAIT_TO_TURN_OFF:

      if (currentLoopMillis > m->ledNextMillis) {
        #if 1 // extra debug typically not needed.
          serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
          
          serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
            serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
            serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
        #endif
        
        segment = l->findRegionsLedRange(m->history.region[0]);
        serial.printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
        l->colorFillRange(strip->Color( 0, 0, 0), segment.startPos, segment.endPos);
       
        m->updateGameState(START_DRAWING_LINE_TO_REGION);
      }
      break;

    case START_DRAWING_LINE_TO_REGION:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      m->printCurrentDesiredRegion();

      m->history.stepPos = 0;      
      serial.printf("  history.hopPos  = %d\n", m->history.hopPos);
      serial.printf("  history.stepPos = %d\n", m->history.stepPos);
      
      m->updateGameState(DRAWING_LINE_TO_REGION);
      break;

    case DRAWING_LINE_TO_REGION:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);

      if (m->history.stepPos < SIZE_OF_STEPS) {
        // before end of buffer size of array of steps.
        m->ledSegPos = (int) pgm_read_word(&plants[m->history.plant[0]].hops[m->history.hopPos].steps[m->history.stepPos]);
        if (m->ledSegPos != 0) {
          // before last in array of steps.
          if (m->ledSegPos > SIZE_OF_LEDSEGS) {
            m->reverse = true;
            m->ledSegPos -= SIZE_OF_LEDSEGS;
          } else {
            m->reverse = false;
          }
          IFDEBUG(serial.printf("        steps[%d]  = '%d'(%s) : \n", m->history.stepPos, m->ledSegPos, m->reverse ? "R" : "A"));

          m->updateGameState(START_DRAWING_SEGMENT);

        } else {
          // exceeded end of array of steps.
          m->updateGameState(FINISHED_LINE_TO_REGION);
          m->history.stepPos++;
        }
      } else {
        // exceeded boundary of construct.
        m->updateGameState(FINISHED_LINE_TO_REGION);
        m->history.stepPos++;
      }

      break;

    case START_DRAWING_SEGMENT:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);

      if (m->reverse) {
        m->startPos = (int) pgm_read_word(&ledSegs[m->ledSegPos].endPos);
        m->endPos = (int) pgm_read_word(&ledSegs[m->ledSegPos].startPos);
      } else {
        m->startPos = (int) pgm_read_word(&ledSegs[m->ledSegPos].startPos);
        m->endPos = (int) pgm_read_word(&ledSegs[m->ledSegPos].endPos);
      }
      
      m->ledpos = m->startPos;

      m->updateGameState(LOOP_EACH_LED);

      m->ledDelayMillis = 50;
      m->ledNextMillis = uint32_t(currentLoopMillis + m->ledDelayMillis);
      m->ledStartMillis = m->ledNextMillis;

      serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
        serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
        serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);

      break;

    case LOOP_EACH_LED:

      if (currentLoopMillis > m->ledNextMillis) {
        #if 0 // extra debug typically not needed.
          serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
          
          serial.print("currentLoopMillis = "); serial.print(currentLoopMillis);
            serial.print(" m->ledNextMillis = "); serial.print(m->ledNextMillis);
            serial.print(" m->currentBrightness = "); serial.println(m->currentBrightness);
        #endif
        
        m->ledNextMillis = uint32_t(currentLoopMillis + m->ledDelayMillis);

        // serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
        IFDEBUG(serial.printf("%d, ", m->ledpos));

        // IFDEBUG(serial.printf("\n  m->ledSegPos = %d(%s), m->startPos = %d, m->endPos = %d, m->ledpos = %d\n", m->ledSegPos, (m->reverse ? "R" : "A"), m->startPos, m->endPos, m->ledpos));

        if (m->reverse) {
          m->ledpos--;
        } else {
          m->ledpos++;
        }
        if (m->reverse && (m->ledpos >= m->endPos)) {
          // m->updateGameState(LOOP_EACH_LED);
        } else if (!m->reverse && (m->ledpos <= m->endPos)) {
          // m->updateGameState(LOOP_EACH_LED);
        } else {
          IFDEBUG(serial.printf("\n"));
          m->updateGameState(FINISHED_SEGMENT);
        }
      }

      break;

    case FINISHED_SEGMENT:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);

      IFDEBUG(serial.printf("\n"));
      m->history.stepPos++;

      if (0) {
        m->updateGameState(START_DRAWING_SEGMENT);
      } else {
        m->updateGameState(DRAWING_LINE_TO_REGION);
      }
      break;

    case FINISHED_LINE_TO_REGION:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      m->history.hopPos++;
      serial.printf("  history.hopPos  = %d\n", m->history.hopPos);
      bool isThereAnextHop = !m->checkIfAtEndOfRegions();
      serial.printf("Is there a next Hop = %s\n", (isThereAnextHop ? "True" : "False"));

      if (isThereAnextHop) {
        m->updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      } else {
        m->updateGameState(WINNER_START);
      }

      break;

    case WINNER_START:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      
        m->updateGameState(WINNER_END);
      break;


    case WINNER_END:
      //serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      
      break;
    case ABANDONED:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      
      break;

    default:
      serial.printf("Entering State of m->gameState[0] = '%d'(%p)\n", m->gameState[0], stateStr[m->gameState[0]]);
      
      break;
  }

}

String getConsole() {
  String response;
//  Serial.print(F("Running - "));Serial.print(__func__);Serial.println("()");

  if(Serial.available() > 0)
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
      serial.printf("  history.hopPos  = %d\n", m->history.hopPos);
      serial.printf("  history.stepPos = %d\n", m->history.stepPos);
      serial.printf("  prv_gameState   = '%p'(%d)\n", stateStr[prv_gameState], prv_gameState);
      m->printGameStateHistory();

    } else if (consoleInputStr == "G") {
      // list out history of gameState[]
      Serial.println("History of gameState[] :");
      serial.printf("  prv_gameState = '%p'(%d)\n", stateStr[prv_gameState], prv_gameState);
      for (int idx = 0; idx < LENGTH_OF_ARRAY(m->gameState); idx++) {
        serial.printf("  gameState[%d]  = '%p'(%d)\n", idx, stateStr[m->gameState[idx]], m->gameState[idx]);
      }
      serial.printf("  history.plant[%d] = %p(%d)\n", 0, plants[m->history.plant[0]].plantName, m->history.plant[0]);

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
        serial.printf("Accepted entered Region %p(%d)\n", regions[m->history.region[0]].name, m->history.region[0]);
      }
      
//      if (nextRegionIdx >= 0) {
//        // when positive, value will be index of new region.
//        if (m->checkUpdateHop(nextRegionIdx)) {
//          // check if expected region, if so then True.
////          serial.printf("selected Region is %p[%d]\n", regions[nextRegionIdx].name, nextRegionIdx);
////          m->printCurrentDesiredRegion();
////          l->startLEDs();
//          
//        } else {
//          // not a valid next region
//          Serial.print(F("No Change"));
//        }
//      }    
    }
    response = consoleInputStr;
  }
  
//  l->updateLEDs();
          
//  Serial.print(F("Ending  - "));Serial.print(__func__);Serial.println("()");
  return response;
}



