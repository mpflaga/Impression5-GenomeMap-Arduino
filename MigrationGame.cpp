#include "Arduino.h"
#include <pgmStrToRAM.h>

#include "MigrationGame.h"

/**
   \brief Constructor
   \param[in] size of memory to allocate for char arrays.

   reserve memory
*/
MigrationGame::MigrationGame() {
#if DEBUG_LEVEL==1
  Serial.print(F("Constructed - MigrationGame::"));Serial.print(__func__);Serial.println();
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
void MigrationGame::begin() {

  // initialize histories with invalid values.
  for (int idx = (LENGTH_OF_ARRAY(history.plant) - 1); idx >= 0 ; idx--) {
    history.plant[idx] = 0;
  }
  for (int idx = (LENGTH_OF_ARRAY(history.region) - 1); idx >= 0 ; idx--) {
    history.region[idx] = 0;
  }
  for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
    gameState[idx] = NO_PLANT_SELECTED;
  }
  history.hopPos = -1;
  history.stepPos = -1;
}

void MigrationGame::begin(Stream &serial) {
  begin();
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
    String str_P((const __FlashStringHelper*) regions[idx].name);
    str_P.toUpperCase();
    if (str_P.startsWith(*consoleInputStr)) {
      response = idx;
      IFDEBUG(_serial->printf("  %d) %p\n", ++matches, regions[idx].name));
    }
  }
  if (response == -1) {
    for (int idx = 0; idx < SIZE_OF_REGIONS; idx++) {
      IFDEBUG(_serial->printf("  %d) %p\n", ++matches, regions[idx].name));
    }
  }
  if (matches > 1) {
    response = -1;
  }
  IFDEBUG(_serial->printf("  Found Region %p(%d)\n", regions[response < 0 ? 0 : response].name, response));
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
  for (int idx = (LENGTH_OF_ARRAY(history.plant) - 1); idx >= 0 ; idx--) {
    IFDEBUG(_serial->printf("  history.plant[%d] = %p(%d)\n", idx, plants[history.plant[idx]].plantName, history.plant[idx]));
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

void MigrationGame::printPlantWithLED() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  for (int idx = 0; idx < SIZE_OF_PLANTS; idx++) {
    //IFDEBUG(_serial->printf("  plants[%d].plantName = %s\n", idx, pgmStrToRAM(plants[idx].plantName)));
    IFDEBUG(_serial->printf("  plants[%d].\n", idx));
    IFDEBUG(_serial->printf("    plantName    = %p\n", plants[idx].plantName));
    IFDEBUG(_serial->printf("    placeCardID  = %d\n", (int) pgm_read_word(&plants[idx].placeCardID)));
    for (int hop = 0; hop < SIZE_OF_HOPS; hop++) {
      if ((int) pgm_read_word(plants[idx].hops[hop].textMSG) != 0) {
        IFDEBUG(_serial->printf("    hops[%d].\n", hop));
        IFDEBUG(_serial->printf("      textMSG    = '%p'\n", plants[idx].hops[hop].textMSG));
        for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
          int nextLedSeg = (int) pgm_read_word(&plants[idx].hops[hop].nextButtons[nButtonPos]);
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
          int ledSegPos = (int) pgm_read_word(&plants[idx].hops[hop].steps[nStepPos]);
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
              for (int ledpos = startPos;ledpos >= endPos; ledpos--) {
                IFDEBUG(_serial->printf("%d, ", ledpos));
              }
            } else {
              startPos = (int) pgm_read_word(&ledSegs[ledSegPos].startPos);
              endPos = (int) pgm_read_word(&ledSegs[ledSegPos].endPos);
              for (int ledpos = startPos;ledpos <= endPos; ledpos++) {
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
    IFDEBUG(_serial->printf("  %d) %p\n", ++idx, regions[idx].name));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->printf("\n"));
}

void MigrationGame::printRegionHistory() {
  IFDEBUG(_serial->printf("Running - MigrationGame::%s()\n", __func__));
  IFDEBUG(_serial->println(F("History of Regions selected:")));
  for (int idx = (LENGTH_OF_ARRAY(history.region) - 1); idx >= 0 ; idx--) {
    IFDEBUG(_serial->printf("  history.region[%d] = %p(%d)\n", idx, regions[history.region[idx]].name, history.region[idx]));
  }
  IFDEBUG(_serial->printf("Ending  - MigrationGame::%s()\n", __func__));
}


bool MigrationGame::updatePlant(int newPlant) {
  IFDEBUG(_serial->printf("Running MigrationGame::%s(%d)\n", __func__, newPlant));
  bool bResult = false;

  IFDEBUG(_serial->printf("  history.plant[0]=%d, newPlant=%d\n", history.plant[0], newPlant));

  if (history.plant[0] != newPlant) {

    // Shift plant history (FILO).
    for (int idx = (LENGTH_OF_ARRAY(history.plant) - 1); idx >= 0 ; idx--) {
      history.plant[idx] = history.plant[idx - 1];
    }
    history.plant[0] = newPlant;
    printPlantHistory();

    // Clear prior history of selected Regions.
    updateRegion(0); // set to initial value of None.

    // Clear/Prime current Hop to 1st position.
   _serial->println(F("  Cleared/Primed first hop and step position."));
    history.hopPos = 0;
    history.stepPos = 0;

    // Shift gameState history (FILO).
    for (int idx = (LENGTH_OF_ARRAY(gameState) - 1); idx >= 0 ; idx--) {
      gameState[idx] = gameState[idx - 1];
    }
    // set statemachine corspondingly to PLANT_INTIALLY_SELECTED or NO_PLANT_SELECTED
    if (history.plant[0] > 0) {
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

  IFDEBUG(_serial->printf("  history.region[0]=%d, newRegion=%d\n", history.region[0], newRegion));
  if (history.region[0] != newRegion) {
    // Shift region history (FILO).
    IFDEBUG(_serial->printf("  New Region History:\n"));
    for (int idx = (LENGTH_OF_ARRAY(history.region) - 1); idx >= 0 ; idx--) {
      history.region[idx] = history.region[idx - 1];
    }
    history.region[0] = newRegion;
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

  if (history.hopPos > -1) {
    // if not uninitialized.
    _serial->printf("%s the following buttons:\n", nextRegionIdx == 0 ? "Looking for" : "Matching to");
    for (int nButtonPos = 0; nButtonPos < SIZE_OF_NEXTBUTTONS; nButtonPos++) {
      // scan thru list of buttons and print those that are found for this list.
      int nextLedSeg = (int) pgm_read_word(&plants[history.plant[0]].hops[history.hopPos].nextButtons[nButtonPos]);
      int nextButtonID = (int) pgm_read_word(&ledSegs[nextLedSeg].buttonID);
      if (nextLedSeg > 0) {
        // when one is found in list, then print it.
        IFDEBUG(_serial->printf("  %p(R%d/L%d),\n", regions[nextButtonID].name, nextLedSeg, nextButtonID));
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

  IFDEBUG(_serial->printf("  Next Hop button = '%d'\n", (int) pgm_read_word(&plants[history.plant[0]].hops[history.hopPos].nextButtons[0])));
  IFDEBUG(_serial->printf("  Next Hop Message = '%p'\n", plants[history.plant[0]].hops[history.hopPos].textMSG));

  bool bResponse = false;
  
  // get value of first button in next hop of current plant.
  int nextLedSeg = (int) pgm_read_word(&plants[history.plant[0]].hops[history.hopPos].nextButtons[0]);


  if (nextLedSeg == 0) {
    // at end of list when eq to zero.
    bResponse = true;
  }

  IFDEBUG(_serial->printf("return MigrationGame::%s() = %s\n", __func__, (bResponse ? "TRUE" : "FALSE")));
  return bResponse;
}



// bool MigrationGame::checkUpdateHop(int nextRegionIdx) {
//   IFDEBUG(_serial->printf("Running MigrationGame::%s(%d)\n", __func__, nextRegionIdx));
//   bool bResult = false;
// 
//   if (history.region[0] != nextRegionIdx) {
//     // Shift Hop Status History (FILO).
//     for (int idx = (LENGTH_OF_ARRAY(history.region) - 1); idx >= 0 ; idx--) {
//       history.region[idx] = history.region[idx - 1];
//     }
//     history.region[0] = nextRegionIdx;
//     IFDEBUG(_serial->printf("history.hopPos = %d\n", history.hopPos));
// 
// 
//     if (history.hopPos < 0) {
//       // illegal state of game
//       IFDEBUG(_serial->println(F("Current Hop Position is not positive")));
//       return bResult;
//     }
//     bResult = checkIfMatchCurrentDesiredRegions(nextRegionIdx);
// 
//     if (!bResult ) {
//       // no match to Next Desired Region selected.
//       IFDEBUG(_serial->printf("  No Match Found\n"));
//       IFDEBUG(_serial->printf("  Current textMSG = '%p'\n", plants[history.plant[0]].hops[history.hopPos].textMSG));
// 
//     } else if (bResult == 1) {
//       // ONE match to Next Desired Region.
//       history.hopPos++;
//       history.stepPos = 0;
// 
//       IFDEBUG(_serial->printf("  Found a Match\n"));
//       _serial->printf("  New textMSG = '%p'\n", plants[history.plant[0]].hops[history.hopPos].textMSG);
//       
//       if ((int) pgm_read_word(&plants[history.plant[0]].hops[history.hopPos].nextButtons[0]) == 0) {
//         _serial->println("You have won!");
// //        gameState = winner;
//         history.hopPos--;
//       }
//     } else {
//       // Detected multiple regions.
//       IFDEBUG(_serial->printf("  Multiple Matches Found\n"));
//     }
//   }
//   IFDEBUG(_serial->printf("  history.hopPos = %d\n", history.hopPos));
//   return bResult;
// }
// 

