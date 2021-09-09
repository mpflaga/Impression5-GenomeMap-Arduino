#include "Arduino.h"
#include "pgmStrToRAM.h"

#include "MigrationGame.h"
#include "LEDdisplay.h"

/**
   \brief Constructor
   \param[in] size of memory to allocate for char arrays.

   reserve memory
*/
MigrationGame::MigrationGame() {
#if DEBUG_LEVEL==1
  Serial.print(F("Constructed - MigrationGame::")); Serial.print(__func__); Serial.println();
#endif

}

/**
   \brief Destructortructor

   release alocated memory.
*/
MigrationGame::~MigrationGame() {
  IFDEBUG(_serial->printf("Destructed - MigrationGame::%s()\n", __func__));

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
  IFDEBUG(_serial = &serial);
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));

  IFDEBUG(_serial->printf("  plants[%d].plantName = %s\n", 0, pgmStrToRAM(plants[0].plantName)));
  IFDEBUG(_serial->printf("  ...\n"));
  IFDEBUG(_serial->printf("  plants[%d].plantName = %s\n", LENGTH_OF_ARRAY(plants) - 1, pgmStrToRAM(plants[LENGTH_OF_ARRAY(plants) - 1].plantName)));

  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}

int MigrationGame::lookforPlant(int consoleInputNumber) {
  int response = -1;
  IFDEBUG(_serial->printf("Running - MigrationGame::%s(%d)\n", __func__, consoleInputNumber));
  for (int idx = 0; ((idx < SIZE_OF_PLANTS) && (response < 0)); idx++) {
    IFDEBUG(_serial->printf("  testing %d) %p\n", idx, plants[idx].plantName));
    if (consoleInputNumber == (int) pgm_read_word(&plants[idx].placeCardID)) {
      response = idx;
      IFDEBUG(_serial->printf("  matched %d) %p\n", idx, plants[idx].plantName));
    }
  }

  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s() = %d, %s\n", __func__, response, ((response == -1) ? "no match" : "matched")));
  return response;
}

int MigrationGame::lookforRegion(String *consoleInputStr) {
  int matches = 0;
  int response = -1;

  IFDEBUG(_serial->printf("Running - MigrationGame::%s(%%s)\n", __func__));
  for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    String str_P((const __FlashStringHelper*) regions[idx]);
    str_P.toUpperCase();
    if (str_P.startsWith(*consoleInputStr)) {
      response = idx;
      IFDEBUG(_serial->printf("  %d) %p\n", ++matches, regions[idx]));
    }
  }
  if (response == -1) {
    for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
      IFDEBUG(_serial->printf("  %d) %p\n", ++matches, regions[idx]));
    }
  }
  if (matches > 1) {
    response = -1;
  }
  IFDEBUG(_serial->printf("  Found Region %p(%d)\n", regions[response < 0 ? 0 : response], response));
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s() = %d\n", __func__, response));
  return response;
}

void MigrationGame::printPlants() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  for (int idx = 0; idx < SIZE_OF_PLANTS; idx++) {
    IFDEBUG(_serial->printf("  %d) %p\n", idx, plants[idx].plantName));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}

void MigrationGame::printPlantHistory() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->println(F("History of Plants selected:")));
  for (int idx = (LENGTH_OF_ARRAY(plant) - 1); idx >= 0 ; idx--) {
    IFDEBUG(_serial->printf("  plant[%d] = %p(%d)\n", idx, plants[plant[idx]].plantName, plant[idx]));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}

void MigrationGame::printGameStateHistory() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->println(F("History of gameState:")));
  for (int idx = 0; idx < LENGTH_OF_ARRAY(gameState); idx++) {
    IFDEBUG(_serial->printf("  gameState[%d]  = '%p'(%d)\n", idx, stateStr[gameState[idx]], gameState[idx]));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}

void MigrationGame::printPlantsWithLED() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  for (int plantPos = 0; plantPos < SIZE_OF_PLANTS; plantPos++) {
    //IFDEBUG(_serial->printf("  plants[%d].plantName = %s\n", plantPos, pgmStrToRAM(plants[plantPos].plantName)));
    IFDEBUG(_serial->printf("  plants[%d].\n", plantPos));
    IFDEBUG(_serial->printf("    plantName    = %p\n", plants[plantPos].plantName));
    IFDEBUG(_serial->printf("    placeCardID  = %d\n", (int) pgm_read_word(&plants[plantPos].placeCardID)));
    for (int hop = 0; hop < SIZE_OF_HOPS; hop++) {
      if ((int) pgm_read_word(plants[plantPos].hops[hop].textMSG) != 0) {
        IFDEBUG(_serial->printf("    hops[%d].\n", hop));
        IFDEBUG(_serial->printf("      textMSG    = '%p'\n", plants[plantPos].hops[hop].textMSG));
        for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
          int nextLedSeg = (int) pgm_read_word(&plants[plantPos].hops[hop].nextButtons[nButtonPos]);
          if (nextLedSeg > 0) {
            IFDEBUG(_serial->printf("        nextButtons[%d]  = '%d' : ", nButtonPos, nextLedSeg));
            for (int ledpos = (int) pgm_read_word(&ledSegs[nextLedSeg].startPos);
                     ledpos <= (int) pgm_read_word(&ledSegs[nextLedSeg].endPos);
                     ledpos++) {
              IFDEBUG(_serial->printf("%d, ", ledpos));
            }
            IFDEBUG(_serial->printf("\n"));
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
            IFDEBUG(_serial->printf("        steps[%d]  = '%d'(%s) : ", nStepPos, ledSegPos, reverse ? "R" : "A"));
            int endPos = 0;
            int startPos = 0;

            if (reverse) {
              startPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
              endPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
              for (int ledpos = startPos; ledpos >= endPos; ledpos--) {
                IFDEBUG(_serial->printf("%d, ", ledpos));
              }
            } else {
              startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
              endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
              for (int ledpos = startPos; ledpos <= endPos; ledpos++) {
                IFDEBUG(_serial->printf("%d, ", ledpos));
              }
            }
            IFDEBUG(_serial->printf("\n"));
          }
        }
      }
    }
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void MigrationGame::printRegions() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
    IFDEBUG(_serial->printf("  %d) %p\n", ++idx, regions[idx]));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void MigrationGame::printRegionHistory() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->println(F("History of Regions selected:")));
  for (int idx = (LENGTH_OF_ARRAY(region) - 1); idx >= 0 ; idx--) {
    IFDEBUG(_serial->printf("  region[%d] = %p(%d)\n", idx, regions[region[idx]], region[idx]));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}


bool MigrationGame::updatePlant(int newPlant) {
  IFDEBUG(_serial->printf("Running MigrationGame::%s(%d)\n", __func__, newPlant));
  bool bResult = false;

  IFDEBUG(_serial->printf("  plant[0]=%d, newPlant=%d\n", plant[0], newPlant));

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
    _serial->println(F("  Cleared/Primed first hop and step position."));
    hopPos = 0;
    stepPos = 0;

    // Shift gameState history (FILO).
    for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
      gameState[idx] = gameState[idx - 1];
    }
    // set statemachine corspondingly to PLANT_INTIALLY_SELECTED or NO_PLANT_SELECTED
    if (plant[0] > 0) {
      gameState[0] = PLANT_INTIALLY_SELECTED;
    } else {
      gameState[0] = NO_PLANT_SELECTED;
    }

    bResult = true;
  }

  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s() = %s\n", __func__, (bResult ? "true" : "false")));
  return bResult;
}

void MigrationGame::updateRegion(int newRegion) {
  IFDEBUG(_serial->printf("Running MigrationGame::%s(%d)\n", __func__, newRegion));

  IFDEBUG(_serial->printf("  region[0]=%d, newRegion=%d\n", region[0], newRegion));
  if (region[0] != newRegion) {
    // Shift region history (FILO).
    IFDEBUG(_serial->printf("  New Region History:\n"));
    for (int idx = (LENGTH_OF_ARRAY(region) - 1); idx >= 0 ; idx--) {
      region[idx] = region[idx - 1];
    }
    region[0] = newRegion;
    printRegionHistory();
  }

  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}

void MigrationGame::updateGameState(state_m newState) {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s(%d), '%p'\n", __func__, newState, stateStr[newState]));

  // Shift gameState history (FILO).
  for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
    gameState[idx] = gameState[idx - 1];
  }
  // set new state.
  gameState[0] = newState;

  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}

int MigrationGame::printCurrentDesiredRegion() {
  IFDEBUG(_serial->printf("Running MigrationGame::%s()\n", __func__));
  return checkIfMatchCurrentDesiredRegions(0); // Force a no match, to cause prints of all of them.
}

bool MigrationGame::checkIfMatchCurrentDesiredRegions(int nextRegionIdx) {
  IFDEBUG(_serial->printf("Running MigrationGame::%s(%d)\n", __func__, nextRegionIdx));

  bool bResponse = false;

  if (hopPos > -1) {
    // if not uninitialized.
    _serial->printf("%s the following buttons:\n", nextRegionIdx == 0 ? "Looking for" : "Matching to");
    for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
      // scan thru list of buttons and print those that are found for this list.
      int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[nButtonPos]);
      int nextButtonID = (int) pgm_read_word(&ledSegs[nextLedSeg].buttonID);
      if (nextLedSeg > 0) {
        // when one is found in list, then print it.
        IFDEBUG(_serial->printf("  %p(R%d/L%d),\n", regions[nextButtonID], nextLedSeg, nextButtonID));
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
  IFDEBUG(_serial->printf("return MigrationGame::%s(%d) = %s\n", __func__, nextRegionIdx, (bResponse ? "TRUE" : "FALSE")));
  return bResponse;
}

bool MigrationGame::checkIfAtEndOfRegions() {
  IFDEBUG(_serial->printf("Running MigrationGame::%s()\n", __func__));

  IFDEBUG(_serial->printf("  Next Hop button = '%d'\n", (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[0])));
  IFDEBUG(_serial->printf("  Next Hop Message = '%p'\n", plants[plant[0]].hops[hopPos].textMSG));

  bool bResponse = false;

  // get value of first button in next hop of current plant.
  int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[0]);


  if (nextLedSeg == 0) {
    // at end of list when eq to zero.
    bResponse = true;
  }

  IFDEBUG(_serial->printf("return MigrationGame::%s() = %s\n", __func__, (bResponse ? "TRUE" : "FALSE")));
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
      _led->colorFillAll(_led->Color( 0, 0, 0));
      updateGameState(NO_PLANT_PRIMED);
      break;

    case NO_PLANT_PRIMED:
      //_serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

      break;

    case PLANT_INTIALLY_SELECTED:

      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);
      _led->colorFillAll(_led->Color(  255, 255,   0)); // RED + GREEN = YELLOW
      ledDelayMillis = 100/4; // MPF - WIP keep low to speed up development.
      ledNextMillis = uint32_t(currentLoopMillis + ledDelayMillis);
      ledStartMillis = ledNextMillis;

      _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
      _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
      _serial->print(" currentBrightness = "); _serial->println(currentBrightness);

      updateGameState(PLANT_INTIALLY_DIMMING_ALL);
      break;

    case PLANT_INTIALLY_DIMMING_ALL:

      if (currentLoopMillis > ledNextMillis) {
#if 1 // extra debug typically not needed.
        _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

        _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
        _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
        _serial->print(" currentBrightness = "); _serial->println(currentBrightness);
#endif

        ledNextMillis = uint32_t(currentLoopMillis + ledDelayMillis);
        currentBrightness = currentBrightness - (maxBrightness / 10);
        if (currentBrightness < 0) {
          currentBrightness = 0;
        }
        _led->setBrightness(currentBrightness);
        _led->show();
      }
      if (currentBrightness <= 0) {
        _serial->print("currentLoopMillis = "); _serial->println(currentLoopMillis);
        _serial->print("duration = "); _serial->print(currentLoopMillis - ledStartMillis);
        _serial->print(" currentBrightness = "); _serial->println(currentBrightness);

        currentBrightness = maxBrightness;;
        _led->setBrightness(currentBrightness);  // reset brightness for next use, but do not turn it on.
        updateGameState(PLANT_INTIALLY_DIMMING_ALL_FINISHED);
      }
      break;

    case PLANT_INTIALLY_DIMMING_ALL_FINISHED:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      updateGameState(PLANT_INTIAL_START_POSITION);
      prv_region = region[0];  //0; // initialize as none, or current value.
      break;

    case PLANT_INTIAL_START_POSITION:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);
      segment.startPos = (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].startPos);
      segment.endPos =   (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].endPos);
      _serial->printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
      _led->colorFillRange(_led->Color( 0, 255, 0), segment.startPos, segment.endPos);

      updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      break;

    case PLANT_ACCEPTED_WAITING_FOR_BUTTON:
      if (prv_region != region[0]) {
        _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
        _serial->printf("Accepting new Region %p(%d)\n", regions[region[0]], region[0]);
        prv_region = region[0];

        bool correctRegion = checkIfMatchCurrentDesiredRegions(region[0]);
        _serial->printf("Region is %s\n", correctRegion ? "correct" : "incorrect");

        if (correctRegion) {
          updateGameState(CORRECT_REGION_SELECTED);
        } else {
          updateGameState(INCORRECT_REGION_SELECTED);
        }
      }
      break;

    case INCORRECT_REGION_SELECTED:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);

      segment = _led->findRegionsLedRange(region[0]);
      _serial->printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
      _led->colorFillRange(_led->Color(255, 0, 0), segment.startPos, segment.endPos);

      ledNextMillis = uint32_t(currentLoopMillis + 2000);
      ledStartMillis = ledNextMillis;

      _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
      _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
      _serial->print(" currentBrightness = "); _serial->println(currentBrightness);
      updateGameState(INCORRECT_REGION_WAIT_TO_TURN_OFF);

      break;

    case INCORRECT_REGION_WAIT_TO_TURN_OFF:

      if (currentLoopMillis > ledNextMillis) {
#if 1 // extra debug typically not needed.
        _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

        _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
        _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
        _serial->print(" currentBrightness = "); _serial->println(currentBrightness);
#endif

        segment = _led->findRegionsLedRange(region[0]);
        _serial->printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
        _led->colorFillRange(_led->Color( 0, 0, 0), segment.startPos, segment.endPos);

        updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      }
      break;

    case CORRECT_REGION_SELECTED:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      currentBrightness = maxBrightness;
      _led->setBrightness(currentBrightness);

      // illuminate all buttons associated with HopPos.
      for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
        int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hopPos].nextButtons[nButtonPos]);
        if (nextLedSeg > 0) {
          IFDEBUG(_serial->printf("        nextButtons[%d]  = '%d' : ", nButtonPos, nextLedSeg));
          _led->colorFillRange(_led->Color( 0, 255, 0), 
                               (int) pgm_read_word(&ledSegs[nextLedSeg].startPos), 
                               (int) pgm_read_word(&ledSegs[nextLedSeg].endPos)
                              );

          IFDEBUG(_serial->printf("\n"));
        }
      }


      ledNextMillis = uint32_t(currentLoopMillis + 2000);
      ledStartMillis = ledNextMillis;

      _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
      _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
      _serial->print(" currentBrightness = "); _serial->println(currentBrightness);
      updateGameState(START_DRAWING_LINE_TO_REGION);

      break;

    case START_DRAWING_LINE_TO_REGION:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      printCurrentDesiredRegion();

      stepPos = 0;
      _serial->printf("  hopPos  = %d\n", hopPos);
      _serial->printf("  stepPos = %d\n", stepPos);

      updateGameState(DRAWING_LINE_TO_REGION);
      break;

    case DRAWING_LINE_TO_REGION:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

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
          IFDEBUG(_serial->printf("        steps[%d]  = '%d'(%s) : \n", stepPos, ledSegPos, reverse ? "R" : "A"));

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
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

      if (reverse) {
        startPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos) - 1;
        endPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos) - 1;
      } else {
        startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos) - 1;
        endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos) -1 ;
      }

      ledpos = startPos;

#if(0)// pause between each segement.
      updateGameState(LOOP_EACH_LED_PAUSE);
      break;

    case LOOP_EACH_LED_PAUSE:
      _serial->println("Press Enter to Continue.");

      while (!Serial.available()) { }
      Serial.println(Serial.read());
#endif // pause between each segement.

      ledDelayMillis = 10/5; // MPF - WIP keep low to speed up development.
      ledNextMillis = uint32_t(currentLoopMillis + ledDelayMillis);
      ledStartMillis = ledNextMillis;

      _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
      _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
      _serial->print(" currentBrightness = "); _serial->println(currentBrightness);

      updateGameState(LOOP_EACH_LED);

      break;

    case LOOP_EACH_LED:

      if (currentLoopMillis > ledNextMillis) {
#if 0 // extra debug typically not needed.
        _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

        _serial->print("currentLoopMillis = "); _serial->print(currentLoopMillis);
        _serial->print(" ledNextMillis = "); _serial->print(ledNextMillis);
        _serial->print(" currentBrightness = "); _serial->println(currentBrightness);
#endif

        ledNextMillis = uint32_t(currentLoopMillis + ledDelayMillis);

        // _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
        IFDEBUG(_serial->printf("%d, ", ledpos));
        _led->setPixelColor(ledpos, _led->Color(  255, 255,   0));
        _led->show();

        // IFDEBUG(_serial->printf("\n  ledSegPos = %d(%s), startPos = %d, endPos = %d, ledpos = %d\n", ledSegPos, (reverse ? "R" : "A"), startPos, endPos, ledpos));

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
          IFDEBUG(_serial->printf("\n"));
          updateGameState(FINISHED_SEGMENT);
        }
      }

      break;

    case FINISHED_SEGMENT:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

      IFDEBUG(_serial->printf("\n"));
      stepPos++;

      if (0) {
        updateGameState(START_DRAWING_SEGMENT);
      } else {
        updateGameState(DRAWING_LINE_TO_REGION);
      }
      break;

    case FINISHED_LINE_TO_REGION:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      hopPos++;
      _serial->printf("  hopPos  = %d\n", hopPos);
      isThereAnextHop = !checkIfAtEndOfRegions();
      _serial->printf("Is there a next Hop = %s\n", (isThereAnextHop ? "True" : "False"));

      if (isThereAnextHop) {
        updateGameState(PLANT_ACCEPTED_WAITING_FOR_BUTTON);
      } else {
        //updateGameState(WINNER_START);
        updateGameState(BEGIN_WIN); // MPF WIP - TEST
        _serial->printf("BEGIN_WIN gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      }

      break;

    case BEGIN_WIN: // MPF - WIP for testing.
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
      updateGameState(END_WIN);
      break;

    case END_WIN: // MPF - WIP for testing.
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

      redrawWholeMigration();

      updateGameState(FOO2);
      break;

    case FOO2: // MPF - WIP for testing.
/*      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);
     delay(1000); */
      break;
    
    default:
      _serial->printf("Entering State of gameState[0] = '%d'(%p)\n", gameState[0], stateStr[gameState[0]]);

      break;
  }

}

void MigrationGame::redrawWholeMigration() {
  LedSegments segment;

  IFDEBUG(_serial->printf("Running MigrationGame::%s()\n", __func__));

  segment.startPos = (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].startPos);
  segment.endPos =   (int) pgm_read_word(&ledSegs[(int) pgm_read_word(&plants[plant[0]].beginRingID)].endPos);
  _serial->printf("segment.startPos = %d, segment.endPos = %d, segment.buttonID = %d\n", segment.startPos, segment.endPos, segment.buttonID);
  _led->colorFillRange(_led->Color( 255, 255, 255), segment.startPos, segment.endPos);


  for (int hop = 0; hop < SIZE_OF_HOPS; hop++) {
    if ((int) pgm_read_word(plants[plant[0]].hops[hop].textMSG) != 0) {
      IFDEBUG(_serial->printf("    hops[%d].\n", hop));
      for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
        int nextLedSeg = (int) pgm_read_word(&plants[plant[0]].hops[hop].nextButtons[nButtonPos]);
        if (nextLedSeg > 0) {
          IFDEBUG(_serial->printf("        nextButtons[%d]  = '%d' : ", nButtonPos, nextLedSeg));
          for (int ledpos = (int) pgm_read_word(&ledSegs[nextLedSeg].startPos);
                   ledpos <= (int) pgm_read_word(&ledSegs[nextLedSeg].endPos);
                   ledpos++) {
            IFDEBUG(_serial->printf("%d, ", ledpos));
            _led->setPixelColor(ledpos - 1, _led->Color(  255, 255, 255));
          }
          IFDEBUG(_serial->printf("\n"));
        }
      }
      for (int nStepPos = 0; nStepPos < SIZE_OF_STEPS; nStepPos++) {
        int ledSegPos = (int) pgm_read_word(&plants[plant[0]].hops[hop].steps[nStepPos]);
        if (ledSegPos != 0) {
          bool reverse = false;

          if (ledSegPos > SIZE_OF_LEDSEGS) {
            reverse = true;
            ledSegPos -= SIZE_OF_LEDSEGS;
          }
          IFDEBUG(_serial->printf("        steps[%d]  = '%d'(%s) : ", nStepPos, ledSegPos, reverse ? "R" : "A"));
          int endPos = 0;
          int startPos = 0;

          if (reverse) {
            startPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
            endPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
            for (int ledpos = startPos; ledpos >= endPos; ledpos--) {
              IFDEBUG(_serial->printf("%d, ", ledpos));
              _led->setPixelColor(ledpos - 1, _led->Color(  255, 255, 255));

            }
          } else {
            startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
            endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
            for (int ledpos = startPos; ledpos <= endPos; ledpos++) {
              IFDEBUG(_serial->printf("%d, ", ledpos));
              _led->setPixelColor(ledpos - 1, _led->Color(  255, 255, 255));
            }
          }
          IFDEBUG(_serial->printf("\n"));
        }
      }
    }
  }
  _led->show();

}
