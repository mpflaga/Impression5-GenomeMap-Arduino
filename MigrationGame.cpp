#include "Arduino.h"
#include "pgmStrToRAM.h"

#include "MigrationGame.h"
#include "LEDdisplay.h"

/**
   \brief Constructor
   \param[in] size of memory to allocate for char arrays.

   reserve memory
*/
MigrationGame::MigrationGame(int maxBrightness_) {
  Serial.print(F("Constructed - MigrationGame::")); Serial.print(__func__); Serial.println();
  maxBrightness = maxBrightness_;
}

/**
   \brief Destructortructor

   release alocated memory.
*/
MigrationGame::~MigrationGame() {
  Serial.print(F("Destructed - MigrationGame::")); Serial.print(__func__); Serial.println();

}

/**
   \brief Initialize the MigrationGame
   initialize Char Arraysy
*/
void MigrationGame::begin(LEDdisplay *led) {
  _led = led;

  // initialize histories with invalid values.
  for (int idx = (LENGTH_OF_ARRAY(plant) - 1); idx >= 0 ; idx--) {
    plant[idx] = 0;
  }
  for (int idx = (LENGTH_OF_ARRAY(region) - 1); idx >= 0 ; idx--) {
    region[idx] = 0;
  }
  for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
    gameState[idx] = NO_PLANT_SELECTED;
  }
  hopPos = -1;
  stepPos = -1;
  prv_region = 0; // initialize as none.
  prv_gameState = gameState[0];

}
void MigrationGame::begin(LEDdisplay *ledDisplay, Stream &serial) {
  begin(ledDisplay);
  _serial = &serial;

  _serial->printf("  plants[%d].plantName = %s\n", 0, pgmStrToRAM(plants[0].plantName));
  _serial->printf("  ...\n");
  _serial->printf("  plants[%d].plantName = %s\n", LENGTH_OF_ARRAY(plants) - 1, pgmStrToRAM(plants[LENGTH_OF_ARRAY(plants) - 1].plantName));

}
int MigrationGame::lookforPlant(int consoleInputNumber) {
  int response = -1;
  for (int idx = 0; ((idx < SIZE_OF_PLANTS) && (response < 0)); idx++) {
    if (consoleInputNumber == (int) pgm_read_word(&plants[idx].placeCardID)) {
      response = idx;
    }
  }

  return response;
}

int MigrationGame::lookforRegion(String *consoleInputStr) {
  int matches = 0;
  int response = -1;

  for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    String str_P((const __FlashStringHelper*) regions[idx]);
    str_P.toUpperCase();
    if (str_P.startsWith(*consoleInputStr)) {
      response = idx;
    }
  }
  if (response == -1) {
    for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    }
  }
  if (matches > 1) {
    response = -1;
  }
  return response;
}

void MigrationGame::printPlants() {
  for (int idx = 0; idx < SIZE_OF_PLANTS; idx++) {
    _serial->printf("  %d) %p\n", idx, plants[idx].plantName);
  }
}

void MigrationGame::printPlantHistory() {
  _serial->println(F("History of Plants selected:"));
  for (int idx = (LENGTH_OF_ARRAY(plant) - 1); idx >= 0 ; idx--) {
    if (( -1 < plant[idx] ) && ( plant[idx] < SIZE_OF_PLANTS )) {
      _serial->printf("  plant[%d] = %p(%d)\n", idx, plants[plant[idx]].plantName, plant[idx]);
    }
    else {
      _serial->printf("  plant[%d] = id# %d\n", idx, plant[idx]);
    }
  }
}

void MigrationGame::printGameStateHistory() {
  _serial->println(F("History of gameState:"));
  for (int idx = 0; idx < LENGTH_OF_ARRAY(gameState); idx++) {
    _serial->printf("  gameState[%d]  = '%p'(%d)\n", idx, stateStr[gameState[idx]], gameState[idx]);
  }
}

void MigrationGame::printPlantsWithLED() {
  for (int plantPos = 0; plantPos < SIZE_OF_PLANTS; plantPos++) {
    _serial->printf("  plants[%d].\n", plantPos);
    _serial->printf("    plantName    = %p\n", plants[plantPos].plantName);
    _serial->printf("    placeCardID  = %d\n", (int) pgm_read_word(&plants[plantPos].placeCardID));
    for (int hop = 0; hop < SIZE_OF_HOPS; hop++) {
      if ((int) pgm_read_word(plants[plantPos].hops[hop].textMSG) != 0) {
        for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
          int nextLedSeg = (int) pgm_read_word(&plants[plantPos].hops[hop].nextButtons[nButtonPos]);
          if (nextLedSeg > 0) {
            for (int ledpos = (int) pgm_read_word(&ledSegs[nextLedSeg].startPos);
                 ledpos <= (int) pgm_read_word(&ledSegs[nextLedSeg].endPos);
                 ledpos++) {
            }
          }
        }
        for (int nStepPos = 0; nStepPos < SIZE_OF_STEPS; nStepPos++) {
          int ledSegPos = (int) pgm_read_word(&plants[plantPos].hops[hop].steps[nStepPos]);
          if (ledSegPos != 0) {
            bool reverse = false;

            if (ledSegPos > SIZE_OF_LEDSEGS) {
              reverse = true;
              ledSegPos -= SIZE_OF_LEDSEGS;
            }
            int endPos = 0;
            int startPos = 0;

            if (reverse) {
              startPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
              endPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
              for (int ledpos = startPos; ledpos >= endPos; ledpos--) {
              }
            } else {
              startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
              endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
              for (int ledpos = startPos; ledpos <= endPos; ledpos++) {
              }
            }
          }
        }
      }
    }
  }
}

void MigrationGame::printRegions() {
  for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    _serial->printf("  %d) %p\n", ++idx, regions[idx]);
  }
}

void MigrationGame::printRegionHistory() {
  _serial->println(F("History of Regions selected:"));
  for (int idx = (LENGTH_OF_ARRAY(region) - 1); idx >= 0 ; idx--) {
    _serial->printf("  region[%d] = %p(%d)\n", idx, regions[region[idx]], region[idx]);
  }
}

bool MigrationGame::updatePlant(int newPlant) {
  bool bResult = false;

  if (plant[0] != newPlant) {
    // Shift plant history (FILO).
    for (int idx = (LENGTH_OF_ARRAY(plant) - 1); idx >= 0 ; idx--) {
      plant[idx] = plant[idx - 1];
    }
    plant[0] = newPlant;
    printPlantHistory();

    // Clear prior history of selected Regions.
    updateRegion(0); // set to initial value of None.

    // Clear/Prime current Hop to 1st position.
    hopPos = 0;
    stepPos = 0;

    // Shift gameState history (FILO).
    for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
      gameState[idx] = gameState[idx - 1];
    }
    // set statemachine corspondingly to PLANT_INTIALLY_SELECTED or NO_PLANT_SELECTED
    if (( 0 < plant[0] ) && ( plant[0] < SIZE_OF_PLANTS )) {
      gameState[0] = PLANT_INTIALLY_SELECTED;
    } 
    else if (plant[0] == 0 ) {
      // if not legitimate plant, set to No Plant. Such as with Diag Cards.
      updateGameState(NO_PLANT_SELECTED);
    } 
    else if (plant[0] == 8 ) {
      updateGameState(TEST_ALL_RED);
    } 
    else if (plant[0] == 9 ) {
      updateGameState(TEST_ALL_GREEN);
    } 
    else if (plant[0] == 10 ) {
      updateGameState(TEST_ALL_BLUE);
    }
    else {
      // if not legitimate plant, set to No Plant. Such as with Diag Cards.
      updateGameState(GAME_STOP);
      _led->colorFillAll(_led->Color( OFF ));
    }

    bResult = true;
  }

  return bResult;
}

void MigrationGame::updateRegion(int newRegion) {

  if (region[0] != newRegion) {
    // Shift region history (FILO).
    for (int idx = (LENGTH_OF_ARRAY(region) - 1); idx >= 0 ; idx--) {
      region[idx] = region[idx - 1];
    }
    region[0] = newRegion;
    printRegionHistory();
  }
}

void MigrationGame::updateGameState(state_m newState) {

  // Shift gameState history (FILO).
  for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
    gameState[idx] = gameState[idx - 1];
  }
  // set new state.
  gameState[0] = newState;

}

int MigrationGame::printCurrentDesiredRegion() {
  return checkIfMatchCurrentDesiredRegions(0); // Force a no match, to cause prints of all of them.
}

bool MigrationGame::checkIfMatchCurrentDesiredRegions(int nextRegionIdx) {
  bool bResponse = false;

  if (hopPos > -1) {
    // if not uninitialized.
    for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
      // scan thru list of buttons and print those that are found for this list.
      int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[nButtonPos]);
      int nextButtonID = (int) pgm_read_word(&ledSegs[nextLedSeg].buttonID);
      if (nextLedSeg > 0) {
        // when one is found in list, then print it.
        if (nextButtonID == nextRegionIdx) {
          // tally up list that is printed
          bResponse = true;
          break;
        }
      }
    }
  } else {
    _serial->println("Unitialized State.");
  }
  return bResponse;
}

bool MigrationGame::checkIfAtEndOfRegions() {
  bool bResponse = false;

  // get value of first button in next hop of current plant.
  int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[0]);

  if (nextLedSeg == 0) {
    // at end of list when eq to zero.
    bResponse = true;
  }

  return bResponse;
}

void MigrationGame::checkGameStateMachine() {
  // process the Game State Machine.

  // locally used in some state transistions. Not persistent.
  uint32_t currentLoopMillis = millis();
  LedSegments segment;
  bool isThereAnextHop;

  // print when the state is about to change.
  if (prv_gameState != gameState[0]) {
    _serial->printf("Change of gameState from (%d)'%p' to (%d)'%p'\n", prv_gameState, stateStr[prv_gameState], gameState[0], stateStr[gameState[0]]);
    prv_gameState = gameState[0];
  }

  switch (gameState[0]) {
    case NO_PLANT_SELECTED:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      _led->colorFillAll(_led->Color( OFF ));
      touchNextMillis = uint32_t (currentLoopMillis + 500);
      flashCounter = 0;
      updateGameState(NO_PLANT_PRIMED);
      break;

    case NO_PLANT_PRIMED:
        // do nothing and wait for external stimulus, such as button.

        if (currentLoopMillis > touchNextMillis) {
            flashCounter++;
          touchNextMillis = uint32_t (touchNextMillis + 500);
          if (( flashCounter % 2 ) == 0) {
            _led->colorFillAllRegions(_led->Color( OFF ));
          } else {
            segment = _led->findRegionsLedRange(random(1, (SIZE_OF_REGIONS + 1)));
            _led->colorFillRange(_led->Color( GREEN ), segment.startPos, segment.endPos);
          }
        }
      break;

    case PLANT_INTIALLY_SELECTED:
      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);
      _led->colorFillAll(_led->Color( GREEN ));
      ledDelayMillis = 25; // MPF - WIP keep low to speed up development.
      ledNextMillis = uint32_t (currentLoopMillis + ledDelayMillis);
      ledStartMillis = ledNextMillis;

      updateGameState(PLANT_INTIALLY_DIMMING_ALL);
      break;

    case PLANT_INTIALLY_DIMMING_ALL:

      if (currentLoopMillis > ledNextMillis) {
        ledNextMillis = uint32_t(ledNextMillis + ledDelayMillis);
        currentBrightness = currentBrightness - (((maxBrightness / 20) < 2) ? 1 : (maxBrightness / 20));
        if (currentBrightness < 0) {
          currentBrightness = 0;
        }
        _led->colorFillAll(_led->Color( 0, currentBrightness, 0 ), false);
        segment.startPos = (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].startPos);
        segment.endPos =   (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].endPos);
        _led->colorFillRange(_led->Color( 0, maxBrightness, 0 ), segment.startPos, segment.endPos, true);
      }
      if (currentBrightness <= 0) {
        currentBrightness = maxBrightness;;
        _led->setBrightness(currentBrightness);  // reset brightness for next use, but do not turn it on.
        updateGameState(PLANT_INTIALLY_DIMMING_ALL_FINISHED);

      }
      break;

    case PLANT_INTIALLY_DIMMING_ALL_FINISHED:
      updateGameState(PLANT_INTIAL_START_POSITION);
      prv_region = region[0];  //0; // initialize as none, or current value.
      break;

    case PLANT_INTIAL_START_POSITION:
      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);
      segment.startPos = (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].startPos);
      segment.endPos =   (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].endPos);
      _led->colorFillRange(_led->Color( GREEN ), segment.startPos, segment.endPos);
      touchNextMillis = uint32_t (currentLoopMillis + touchTimeOutMillis);

      updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      break;

    case PLANT_ACCEPTED_WAITING_FOR_BUTTON:

      if (prv_region != region[0]) {
        prv_region = region[0];

        bool correctRegion = checkIfMatchCurrentDesiredRegions(region[0]);
        _serial->printf("Region is %s\n", correctRegion ? "correct" : "incorrect");

        _led->removeAllColor( _led->Color(RED), false );

        if (correctRegion) {
          updateGameState(CORRECT_REGION_SELECTED);
        } else {
          updateGameState(INCORRECT_REGION_SELECTED);
        }
        
        touchNextMillis = uint32_t (currentLoopMillis + touchTimeOutMillis);
      }
      else {
        if (currentLoopMillis > touchNextMillis) {
          if (hopPos > 0) {
            _serial->println(F("gameState: touch timeout, restarting plant"));

            // Clear prior history of selected Regions.
            updateRegion(0); // set to initial value of None.

            // Clear/Prime current Hop to 1st position.
            hopPos = 0;
            stepPos = 0;

            _led->colorFillAll(_led->Color( OFF ), false);
            updateGameState(PLANT_INTIAL_START_POSITION);
          }
        }
      }
      break;

    case INCORRECT_REGION_SELECTED:

      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);
      segment = _led->findRegionsLedRange(region[0]);

      if (( _led->getPixelColor(segment.startPos) & 0x0000FF00) != 0 ) {
        updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      }
      else {
        flashCounter = 0;
        ledDelayMillis = 125;
        ledNextMillis = uint32_t(currentLoopMillis + ledDelayMillis);
        ledStartMillis = ledNextMillis;
        updateGameState(INCORRECT_REGION_FLASHING);
      }

      break;
      
    case INCORRECT_REGION_FLASHING:
      
      if (prv_region == region[0]) {
        if (flashCounter > ( 2 * 3 ) + 1 ) {
          updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
        }
        else {
          if (currentLoopMillis > ledNextMillis) {
            segment = _led->findRegionsLedRange(region[0]);
            flashCounter++;

            if (( flashCounter % 2 ) == 0) {
              Serial.println(F("OFF"));
              _led->colorFillRange(_led->Color( OFF ), segment.startPos, segment.endPos);
            }
            else {
              Serial.println(F("RED"));
              _led->colorFillRange(_led->Color( RED ), segment.startPos, segment.endPos);
            }
            ledNextMillis = uint32_t(ledNextMillis + ledDelayMillis);
          }
        }
      }
      else {
        updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      }


      break;

    case CORRECT_REGION_SELECTED:

      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);

      // redraw prior migration, without incorrect selections.
      redrawMigration(hopPos);

      // illuminate all buttons associated with HopPos.
      for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
        int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[nButtonPos]);
        if (nextLedSeg > 0) {
          _led->colorFillRange(_led->Color( GREEN ),
                               (int) pgm_read_word(&ledSegs[nextLedSeg].startPos),
                               (int) pgm_read_word(&ledSegs[nextLedSeg].endPos)
                              );
        }
      }

      ledNextMillis = uint32_t(currentLoopMillis + 2000);
      ledStartMillis = ledNextMillis;

      updateGameState(START_DRAWING_LINE_TO_REGION);
      _serial->println();

      break;

    case START_DRAWING_LINE_TO_REGION:
      printCurrentDesiredRegion();
      stepPos = 0;
      updateGameState(DRAWING_LINE_TO_REGION);
        lengthOfSegment = loopThurAllSegmentsOfHop( hopPos, _led->Color( OFF ), false );
        delayBetweenSegLEDsMillis = ( 1000 / lengthOfSegment );
        if ( delayBetweenSegLEDsMillis > 100 ) {
          // don't let it go too slow
          delayBetweenSegLEDsMillis = 100;
        }

      break;

    case DRAWING_LINE_TO_REGION:

//      _serial->println();
      if (stepPos < SIZE_OF_STEPS) {
        // before end of buffer size of array of steps.
        ledSegPos = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].steps[stepPos]);
        if (ledSegPos != 0) {
          // before last in array of steps.
          if (ledSegPos > SIZE_OF_LEDSEGS) {
            reverse = true;
            ledSegPos -= SIZE_OF_LEDSEGS;
          } else {
            reverse = false;
          }
          updateGameState(START_DRAWING_SEGMENT);
        } else {
          // exceeded end of array of steps.
          updateGameState(FINISHED_LINE_TO_REGION);
          stepPos++;
        }
      } else {
        // exceeded boundary of construct.
        updateGameState(FINISHED_LINE_TO_REGION);
        stepPos++;
      }

      break;

    case START_DRAWING_SEGMENT:

      if (reverse) {
        startPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos) - 1;
        endPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos) - 1;
      } else {
        startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos) - 1;
        endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos) - 1 ;
      }

      ledpos = startPos;

#if(0)// pause between each segement.
      updateGameState(LOOP_EACH_LED_PAUSE);
      break;

    case LOOP_EACH_LED_PAUSE:
      _serial->println("Press Enter to Continue.");

      while (!Serial.available()) { }
      _serial->println(Serial.read());
#endif // pause between each segement.

      ledNextMillis = uint32_t(currentLoopMillis + 0);
      ledStartMillis = ledNextMillis;

      updateGameState(LOOP_EACH_LED);

      break;

    case LOOP_EACH_LED:

      if (currentLoopMillis > ledNextMillis) {
        uint32_t prvLedNextMillis = ledNextMillis; 
        ledNextMillis = uint32_t(ledNextMillis + delayBetweenSegLEDsMillis);

        for (int i = 0; i < (delayBetweenSegLEDsMillis < 30 ? (30 / delayBetweenSegLEDsMillis) : 1 ); i++) {
          _led->setPixelColor(ledpos, _led->Color( WHITE ));

          if (reverse) {
            ledpos--;
          } else {
            ledpos++;
          }
          if (reverse && (ledpos >= endPos)) {
            // updateGameState(LOOP_EACH_LED);
          } else if (!reverse && (ledpos <= endPos)) {
            // updateGameState(LOOP_EACH_LED);
          } else {
            updateGameState(FINISHED_SEGMENT);
            break;
          }
        }
        _led->show();
      }

      break;

    case FINISHED_SEGMENT:

      stepPos++;

      if (0) {
        updateGameState(START_DRAWING_SEGMENT);
      } else {
        updateGameState(DRAWING_LINE_TO_REGION);
      }

      break;

    case FINISHED_LINE_TO_REGION:
      hopPos++;
      redrawMigration(hopPos);
      isThereAnextHop = !checkIfAtEndOfRegions();

      if (isThereAnextHop) {
        updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      } else {
        //updateGameState(WINNER_START);
        updateGameState(BEGIN_WIN); // MPF WIP - TEST
      }

      break;

    case BEGIN_WIN:
      flashCounter = 0;
      touchNextMillis = uint32_t (currentLoopMillis + 250);
      updateGameState(END_WIN);
      
      break;

    case END_WIN:
      if (flashCounter > ( 2 * 3 )) {
        updateGameState(WIN_IDLE);
        touchNextMillis = uint32_t (touchNextMillis + touchTimeOutMillis);
      }    
      else {
        if (currentLoopMillis > touchNextMillis) {
          flashCounter++;
          touchNextMillis = uint32_t (touchNextMillis + 250);
          if (( flashCounter % 2 ) == 0) {
            redrawMigration(SIZE_OF_HOPS, _led->Color( OFF ), _led->Color( OFF ));
          } else {
            redrawMigration(SIZE_OF_HOPS, _led->Color( GREEN ), _led->Color( GREEN ));
          }
        }
      }

      break;

    case WIN_IDLE:
      if (currentLoopMillis > touchNextMillis) {
        _serial->println(F("gameState: Win Idle timeout, restarting plant"));

        // Clear prior history of selected Regions.
        updateRegion(0); // set to initial value of None.

        // Clear/Prime current Hop to 1st position.
        hopPos = 0;
        stepPos = 0;

        _led->colorFillAll(_led->Color( OFF ), false);
        updateGameState(PLANT_INTIAL_START_POSITION);
      }

      break;

    case TEST_ALL_RED:
      _led->colorFillAll(_led->Color( RED ), true);
      updateGameState(GAME_STOP);
      break;
      
    case TEST_ALL_GREEN:
      _led->colorFillAll(_led->Color( GREEN ), true);
      updateGameState(GAME_STOP);
      break;
      
    case TEST_ALL_BLUE:
      _led->colorFillAll(_led->Color( BLUE ), true);
      updateGameState(GAME_STOP);
      break;
      
    case GAME_STOP:
      // do nothing while game is disabled. 
      break;
      
    default:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

      break;
  }

}

void MigrationGame::redrawMigration(int currentHop, unsigned long segmentColor, unsigned long buttonColor, bool show) {
  LedSegments segment;
  int length = 0;

  segment.startPos = (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].startPos);
  segment.endPos =   (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].endPos);

  // blank all pixels before redrawing fresh
  for (int pos = ((0 + 1) - 1); pos < (_led->numPixels() + 1); pos++) {
    _led->setPixelColor(pos, _led->Color( OFF )); // 0's for off
  }

  // draw first ring.
  for (int pos = (segment.startPos - 1); pos < segment.endPos; pos++) {
    _led->setPixelColor(pos, buttonColor);
  }

  for (int hop = 0; hop < currentHop; hop++) {
    if ((int) pgm_read_word(plants[plant[0]].hops[hop].textMSG) != 0) {

      // loop thru all the buttons
      for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
        int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hop].nextButtons[nButtonPos]);
        if (nextLedSeg > 0) {

          for (int ledpos = (int) pgm_read_word(&ledSegs[nextLedSeg].startPos);
               ledpos <= (int) pgm_read_word(&ledSegs[nextLedSeg].endPos);
               ledpos++) {
            _led->setPixelColor(ledpos - 1, buttonColor);
          }
        }
      }

      length += loopThurAllSegmentsOfHop( hop, segmentColor );
    }
  }
  if ( show ) {
    _led->show();
  }
}

int MigrationGame::loopThurAllSegmentsOfHop( int hop, unsigned long segmentColor, bool justCount ) {
  int length = 0;
  int LEDcounter = 0;
  
  for (int nStepPos = 0; nStepPos < SIZE_OF_STEPS; nStepPos++) {
    int ledSegPos = (int) pgm_read_word(&plants[plant[0]].hops[hop].steps[nStepPos]);
    if (ledSegPos != 0) {
      bool reverse = false;

      if (ledSegPos > SIZE_OF_LEDSEGS) {
        reverse = true;
        ledSegPos -= SIZE_OF_LEDSEGS;
      }
      int endPos = 0;
      int startPos = 0;

      if (reverse) {
        startPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
        endPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
        length = length + (( startPos + 1 )- endPos);
        if ( !justCount) {
          for (int ledpos = startPos; ledpos >= endPos; ledpos--) {
            _led->setPixelColor(ledpos - 1, segmentColor);
            LEDcounter++;
          }
        }
      } else {
        startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
        endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
        length = length + (( endPos + 1 ) - startPos);
        if ( !justCount) {
          for (int ledpos = startPos; ledpos <= endPos; ledpos++) {
            _led->setPixelColor(ledpos - 1, segmentColor);
            LEDcounter++;
          }
        }
      }
    }
  }  
  return length ;
}