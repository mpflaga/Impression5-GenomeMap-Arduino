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
    { 7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7}, //tthresh
    { 4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4}, //rthresh
  //{        1,       4, 3, 4, 7,     6,          7,    8,     9,     7,    11,              12}, //identifier
    {AUSTRALIA, SE_ASIA, 0, 0, 0, INDIA, KAZAKHSTAN, ASIA, KOREA, JAPAN, CHINA, PACIFIC_ISLANDS}, //identifier
  },
  { 0x5B,
    { 7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7}, //tthresh
    { 4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4}, //rthresh
  //{13,   14,   15,   16,   17,   18,   19,   40,   41,   42,   43,   44}, //identifier
    {TURKEY, 0, 0, MADAGASCAR, EAST_AFRICA, AFRICA, WEST_AFRICA, MALI, 0, SPAIN, BRITISH_ISLES, EUROPE}, //identifier
  },
  { 0x5C,
    { 7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7}, //tthresh
    { 4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4}, //rthresh
  //{25,   46,   47,   48,   49,   30,   31,   32,   33,   34,   35,   36}, //identifier
    {BOLIVIA_PERU, W_SOUTH_AMERICA, PANAMA, CARIBBEAN, 0, CANADA, NEW_ENGLAND, 0, 0, BRAZIL, 0, CHILE}, //identifier
  },
  { 0x5D,
    { 7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    7}, //tthresh
    { 4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,    4}, //rthresh
  //{37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48}, //identifier
    {0, 0, 0, 0, CALIFORNIA, ARIZONA_NEW_MEXICO, E_NORTH_AMERICA, 0, MEXICO, 0, SOUTH_CAROLINA, 0}, //identifier
  }
};
#define SIZE_OF_CHIPS LENGTH_OF_ARRAY(groups)

class tpad : public MPR121_type {

  public:
    tpad();
    ~tpad();
    void begin(int);
    int scan();
    int interruptPin;

  private:
    MPR121_type chips[SIZE_OF_CHIPS];

};

#endif // tpad_H
