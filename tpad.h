#ifndef IFDEBUG
#define IFDEBUG(...) ((void)((DEBUG_LEVEL) && (__VA_ARGS__, 0)))
#define DEBUG_LEVEL 1 // set to 1 to compile in Serial Debug prints
#endif

#ifndef LENGTH_OF_ARRAY
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(x[0])))
#endif

#ifndef tpad_H
#define tpad_H

#ifndef PRINTEX_VERSION
#include <PrintEx.h>
#endif

#include <MPR121.h>
#include "MigrationData.h"
#include "LEDdisplay.h"

#define numElectrodes 12 // per MPR chip
typedef struct {
//  MPR121_type device;
  uint8_t address;
  unsigned char tthresh[numElectrodes];
  unsigned char rthresh[numElectrodes];
  uint8_t identifier[numElectrodes];
} mprs;

const mprs groups[] PROGMEM = {
  (mprs) {
    0x5A,
    {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //tthresh
    {05,   05,   05,   05,   05,   05,   05,   05,   05,   05,   05,   05}, //rthresh
  //{        1,       2, 3, 4, 5,     6,          7,    8,     9,    10,    11,              12}, //identifier
    {AUSTRALIA, SE_ASIA, 0, 0, 0, INDIA, KAZAKHSTAN, ASIA, KOREA, JAPAN, CHINA, PACIFIC_ISLANDS}, //identifier
  },
  { 0x5B,
    {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh
    {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh
  //{13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24}, //identifier
    {TURKEY, 0, 0, MADAGASCAR, EAST_AFRICA, AFRICA, 0, MALI, WEST_AFRICA, SPAIN, BRITISH_ISLES, EUROPE}, //identifier
  },
  { 0x5C,
    {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh
    {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh
  //{25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36}, //identifier
    {BOLIVIA_PERU, W_SOUTH_AMERICA, PANAMA, CARIBBEAN, 0, CANADA, NEW_ENGLAND, 0, 0, BRAZIL, 0, CHILE}, //identifier
  },
  { 0x5D,
    {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh
    {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh
  //{37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48}, //identifier
    {0, 0, 0, 0, CALIFORNIA, ARIZONA_NEW_MEXICO, E_NORTH_AMERICA, 0, MEXICO, 0, SOUTH_CAROLINA, 0}, //identifier
  }
};

typedef struct {
  uint8_t address;
  unsigned char tthresh[numElectrodes];
  unsigned char rthresh[numElectrodes];
  uint8_t identifier[numElectrodes];
} mprs1;

class tpad : public MPR121_type {

  public:
    tpad();
    ~tpad();
    void begin(Stream &serial, int);
    int scan();
    int interruptPin;

  private:
    StreamEx* _serial;
    MPR121_type chips[4];

};

#endif // tpad_H
