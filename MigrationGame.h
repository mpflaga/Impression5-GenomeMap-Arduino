
#ifndef IFDEBUG
#define IFDEBUG(...) ((void)((DEBUG_LEVEL) && (__VA_ARGS__, 0)))
#define DEBUG_LEVEL 1 // set to 1 to compile in Serial Debug prints
#endif

#ifndef PRINTEX_VERSION
#include <PrintEx.h>
#endif

#ifndef MigrationGame_h
#define MigrationGame_h

#include "MigrationData.h"
#include "LEDdisplay.h"

typedef enum  {
  NO_PLANT_SELECTED,
  NO_PLANT_PRIMED,
  PLANT_INTIALLY_SELECTED,
  PLANT_INTIALLY_DIMMING_ALL,
  PLANT_INTIALLY_DIMMING_ALL_FINISHED,
  PLANT_INTIAL_START_POSITION,
  PLANT_ACCEPTED_WAITING_FOR_BUTTON,
  INCORRECT_REGION_SELECTED,
  INCORRECT_REGION_WAIT_TO_TURN_OFF,
  CORRECT_REGION_SELECTED,
  START_DRAWING_LINE_TO_REGION,
  DRAWING_LINE_TO_REGION,
  START_DRAWING_SEGMENT,
  LOOP_EACH_LED,
  LOOP_EACH_LED_PAUSE,
  FINISHED_SEGMENT,
  BEGIN_WIN,
  END_WIN,
  FOO2,
  FINISHED_LINE_TO_REGION
} state_m; //enum state_m

static const char stateStr[][42] PROGMEM = {
  "NO_PLANT_SELECTED",
  "NO_PLANT_PRIMED",
  "PLANT_INTIALLY_SELECTED",
  "PLANT_INTIALLY_DIMMING_ALL",
  "PLANT_INTIALLY_DIMMING_ALL_FINISHED",
  "PLANT_INTIAL_START_POSITION",
  "PLANT_ACCEPTED_WAITING_FOR_BUTTON",
  "INCORRECT_REGION_SELECTED",
  "INCORRECT_REGION_WAIT_TO_TURN_OFF",
  "CORRECT_REGION_SELECTED",
  "START_DRAWING_LINE_TO_REGION",
  "DRAWING_LINE_TO_REGION",
  "START_DRAWING_SEGMENT",
  "LOOP_EACH_LED",
  "LOOP_EACH_LED_PAUSE",
  "FINISHED_SEGMENT",
  "BEGIN_WIN",
  "END_WIN",
  "FOO2",
  "FINISHED_LINE_TO_REGION"
};

class MigrationGame {
  private:

    //Stream* _serial;
    StreamEx* _serial;
    LEDdisplay* _led;
    int prv_region;
    uint32_t ledNextMillis;
    uint32_t ledStartMillis;
    uint32_t ledDelayMillis;
    bool reverse;
    int ledSegPos;
    int ledpos;
    int startPos;
    int endPos;
    int currentBrightness;
    int maxBrightness = 15;

  public:

    int plant[4];  // current Plant is index 0
    int hopPos;
    int stepPos;
    int region[4]; // index 0 is buffered, index 1 will be current.
    state_m gameState[10];
    state_m prv_gameState;

    MigrationGame();
    ~MigrationGame();
    void begin(LEDdisplay *ledDisplay);
    void begin(LEDdisplay *ledDisplay, Stream &serial);
    int  lookforRegion(String *consoleInputStr);
    int  lookforPlant(int consoleInputNumber);
    void printPlantsWithLED();
    void printRegionHistory();
    void printPlantHistory();
    void printGameStateHistory();
    void printPlants();
    void printRegions();
    bool updatePlant(int newPlant);
    void updateRegion(int newRegion);
    void updateGameState(state_m newState);
    int  printCurrentDesiredRegion();
    bool checkIfMatchCurrentDesiredRegions(int nextRegionIdx);
    bool checkIfAtEndOfRegions();
    void checkGameStateMachine();
    void redrawWholeMigration();
};

#endif  // MigrationGame_h