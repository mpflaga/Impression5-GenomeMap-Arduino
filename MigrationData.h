#ifndef LENGTH_OF_ARRAY
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(*x)))
#endif

#ifndef MigrationData_h
#define MigrationData_h



typedef enum {
None               , //  0 
AFRICA             , //  1 
ARIZONA_NEW_MEXICO , //  2 
ASIA               , //  3 
AUSTRALIA          , //  4 
BOLIVIA_PERU       , //  5 
BRAZIL             , //  6 
BRITISH_ISLES      , //  7 
CALIFORNIA         , //  8 
CANADA             , //  9 
CARIBBEAN          , // 10 
CHILE              , // 11 
CHINA              , // 12 
E_NORTH_AMERICA    , // 13 
EAST_AFRICA        , // 14 
EUROPE             , // 15 
INDIA              , // 16 
JAPAN              , // 17 
KAZAKHSTAN         , // 18 
KOREA              , // 19 
MADAGASCAR         , // 20 
MALI               , // 21 
MEXICO             , // 22 
NEW_ENGLAND        , // 23 
PACIFIC_ISLANDS    , // 24 
PANAMA             , // 25 
SE_ASIA            , // 26 
SOUTH_CAROLINA     , // 27 
SPAIN              , // 28 
TURKEY             , // 29 
W_SOUTH_AMERICA    , // 30 
WEST_AFRICA          // 31
} Countries;

typedef struct {
  char name[20];
  int chipID;
  int channel;
  int LEDseg;
} regionChipID;

// needs to be in same order as Countries.
static const regionChipID regions[] PROGMEM = {   
  {"None"              , 0x00,  0,  0}, //  0
  {"AFRICA"            , 0x5B,  5, 90}, //  1
  {"ARIZONA_NEW_MEXICO", 0x5D,  5, 54}, //  2
  {"ASIA"              , 0x5A,  7, 34}, //  3
  {"AUSTRALIA"         , 0x5A,  0,  1}, //  4
  {"BOLIVIA_PERU"      , 0x5C,  0, 73}, //  5
  {"BRAZIL"            , 0x5C,  9,  7}, //  6
  {"BRITISH_ISLES"     , 0x5B, 10, 44}, //  7
  {"CALIFORNIA"        , 0x5D,  4, 55}, //  8
  {"CANADA"            , 0x5C,  5, 50}, //  9
  {"CARIBBEAN"         , 0x5C,  3, 77}, // 10
  {"CHILE"             , 0x5C, 11, 75}, // 11
  {"CHINA"             , 0x5A, 10, 17}, // 12
  {"E_NORTH_AMERICA"   , 0x5D,  6, 52}, // 13
  {"EAST_AFRICA"       , 0x5B,  4, 31}, // 14
  {"EUROPE"            , 0x5B, 11, 39}, // 15
  {"INDIA"             , 0x5A,  5, 29}, // 16
  {"JAPAN"             , 0x5A,  9, 20}, // 17
  {"KAZAKHSTAN"        , 0x5A,  6, 35}, // 18
  {"KOREA"             , 0x5A,  8, 22}, // 19
  {"MADAGASCAR"        , 0x5B,  3, 27}, // 20
  {"MALI"              , 0x5B,  7, 86}, // 21
  {"MEXICO"            , 0x5D,  8, 58}, // 22
  {"NEW_ENGLAND"       , 0x5C,  6, 49}, // 23
  {"PACIFIC_ISLANDS"   , 0x5A, 11, 12}, // 24
  {"PANAMA"            , 0x5C,  2, 71}, // 25
  {"SE_ASIA"           , 0x5A,  1, 15}, // 26
  {"SOUTH_CAROLINA"    , 0x5D, 10, 64}, // 27
  {"SPAIN"             , 0x5B,  9, 41}, // 28
  {"TURKEY"            , 0x5B,  0, 37}, // 29
  {"W_SOUTH_AMERICA"   , 0x5C,  1, 67}, // 30
  {"WEST_AFRICA"       , 0x5B,  6, 82}  // 31
}; 
#define SIZE_OF_REGIONS LENGTH_OF_ARRAY(regions)

typedef enum {
  ascend,
  descend
} Orientation;

typedef struct {
  int startPos;
  int endPos;
  Countries buttonID;
} LedSegments;

static const LedSegments ledSegs[] PROGMEM = {   
  {   0,    0,  None              }, //  0
  {   1,   16,  AUSTRALIA         }, //  1 
  {  17,   66,  None              }, //  2
  {  67,   84,  None              }, //  3
  {  85,  106,  None              }, //  4
  { 107,  125,  None              }, //  5
  { 126,  141,  None              }, //  6
  { 142,  157,  BRAZIL            }, //  7 
  { 158,  165,  None              }, //  8
  { 166,  174,  None              }, //  9
  { 175,  193,  None              }, // 10
  { 194,  235,  None              }, // 11
  { 236,  251,  PACIFIC_ISLANDS   }, // 12
  { 252,  278,  None              }, // 13
  { 279,  289,  None              }, // 14
  { 290,  305,  SE_ASIA           }, // 15
  { 306,  312,  None              }, // 16
  { 313,  328,  CHINA             }, // 17
  { 329,  362,  None              }, // 18
  { 363,  378,  None              }, // 19
  { 379,  394,  JAPAN             }, // 20
  { 395,  403,  None              }, // 21
  { 404,  419,  KOREA             }, // 22
  { 420,  422,  None              }, // 23
  { 423,  456,  None              }, // 24
  { 457,  458,  None              }, // 25
  { 459,  468,  None              }, // 26
  { 469,  484,  MADAGASCAR        }, // 27
  { 485,  501,  None              }, // 28
  { 502,  517,  INDIA             }, // 29
  { 518,  539,  None              }, // 30
  { 540,  555,  EAST_AFRICA       }, // 31
  { 556,  573,  None              }, // 32
  { 574,  575,  None              }, // 33
  { 576,  591,  ASIA              }, // 34
  { 592,  607,  KAZAKHSTAN        }, // 35
  { 608,  631,  None              }, // 36
  { 632,  647,  TURKEY            }, // 37
  { 648,  661,  None              }, // 38
  { 662,  677,  EUROPE            }, // 39
  { 678,  680,  None              }, // 40
  { 681,  696,  SPAIN             }, // 41
  { 697,  699,  None              }, // 42
  { 700,  700,  None              }, // 43
  { 701,  716,  BRITISH_ISLES     }, // 44
  { 717,  731,  None              }, // 45
  { 732,  734,  None              }, // 46
  { 735,  750,  None              }, // 47
  { 751,  776,  None              }, // 48
  { 777,  792,  NEW_ENGLAND       }, // 49
  { 793,  808,  CANADA            }, // 50
  { 809,  812,  None              }, // 51
  { 813,  828,  E_NORTH_AMERICA   }, // 52
  { 829,  837,  None              }, // 53
  { 838,  853,  ARIZONA_NEW_MEXICO}, // 54
  { 854,  869,  CALIFORNIA        }, // 55
  { 870,  898,  None              }, // 56
  { 899,  907,  None              }, // 57
  { 908,  923,  MEXICO            }, // 58
  { 924,  937,  None              }, // 59
  { 938,  969,  None              }, // 60
  { 970,  987,  None              }, // 61
  { 988, 1027,  None              }, // 62
  {1028, 1032,  None              }, // 63
  {1033, 1048,  SOUTH_CAROLINA    }, // 64
  {1049, 1061,  None              }, // 65
  {1062, 1067,  None              }, // 66
  {1068, 1083,  W_SOUTH_AMERICA   }, // 67
  {1084, 1109,  None              }, // 68
  {1110, 1140,  None              }, // 69
  {1141, 1168,  None              }, // 70
  {1169, 1184,  PANAMA            }, // 71
  {1185, 1190,  None              }, // 72
  {1191, 1206,  BOLIVIA_PERU      }, // 73
  {1207, 1212,  None              }, // 74
  {1213, 1228,  CHILE             }, // 75
  {1229, 1233,  None              }, // 76
  {1234, 1249,  CARIBBEAN         }, // 77
  {1250, 1283,  None              }, // 78
  {1284, 1295,  None              }, // 79
  {1296, 1300,  None              }, // 80
  {1301, 1301,  None              }, // 81
  {1302, 1317,  WEST_AFRICA       }, // 82
  {1318, 1320,  None              }, // 83
  {1321, 1325,  None              }, // 84
  {1326, 1328,  None              }, // 85
  {1329, 1344,  MALI              }, // 86
  {1345, 1351,  None              }, // 87
  {1352, 1357,  None              }, // 88
  {1358, 1373,  None              }, // 89
  {1374, 1389,  AFRICA            }  // 90
};              
#define SIZE_OF_LEDSEGS LENGTH_OF_ARRAY(ledSegs)

static const LedSegments RingSegs[] PROGMEM = {
  {  99,   88,  None              }, //  0
  {   1,   16,  AUSTRALIA         }, //  1 
  { 142,  157,  BRAZIL            }, //  7 
  { 236,  251,  PACIFIC_ISLANDS   }, // 12
  { 290,  305,  SE_ASIA           }, // 15
  { 313,  328,  CHINA             }, // 17
  { 379,  394,  JAPAN             }, // 20
  { 404,  419,  KOREA             }, // 22
  { 469,  484,  MADAGASCAR        }, // 27
  { 502,  517,  INDIA             }, // 29
  { 540,  555,  EAST_AFRICA       }, // 31
  { 576,  591,  ASIA              }, // 34
  { 592,  607,  KAZAKHSTAN        }, // 35
  { 632,  647,  TURKEY            }, // 37
  { 662,  677,  EUROPE            }, // 39
  { 681,  696,  SPAIN             }, // 41
  { 701,  716,  BRITISH_ISLES     }, // 44
  { 777,  792,  NEW_ENGLAND       }, // 49
  { 793,  808,  CANADA            }, // 50
  { 813,  828,  E_NORTH_AMERICA   }, // 52
  { 838,  853,  ARIZONA_NEW_MEXICO}, // 54
  { 854,  869,  CALIFORNIA        }, // 55
  { 908,  923,  MEXICO            }, // 58
  {1033, 1048,  SOUTH_CAROLINA    }, // 64
  {1068, 1083,  W_SOUTH_AMERICA   }, // 67
  {1169, 1184,  PANAMA            }, // 71
  {1191, 1206,  BOLIVIA_PERU      }, // 73
  {1213, 1228,  CHILE             }, // 75
  {1234, 1249,  CARIBBEAN         }, // 77
  {1302, 1317,  WEST_AFRICA       }, // 82
  {1329, 1344,  MALI              }, // 86
  {1374, 1389,  AFRICA            }  // 90
};
#define SIZE_OF_RINGSEGS LENGTH_OF_ARRAY(RingSegs)

// static const int buttonLEDsegment[] PROGMEM = {
// //LedSegments, ButtonID.
             // 1, //  1
             // 7, //  2
            // 12, //  3
            // 15, //  4
            // 17, //  5
            // 20, //  6
            // 22, //  7
            // 27, //  8
            // 29, //  9
            // 31, // 10
            // 34, // 11
            // 35, // 12
            // 37, // 13
            // 39, // 14
            // 41, // 15
            // 44, // 16
            // 49, // 17
            // 50, // 18
            // 52, // 19
            // 54, // 20
            // 55, // 21
            // 58, // 22
            // 64, // 23
            // 67, // 24
            // 71, // 25
            // 73, // 26
            // 75, // 27
            // 77, // 28
            // 82, // 29
            // 86, // 30
            // 90, // 31
// };

// typedef struct {
  // int segment;
  // int endPos;
// } ButtonLED;


typedef struct {
  char textMSG[71+1];
  int  nextButtons[4];
  int  steps[17];
} hopStruct;

typedef struct {
  char plantName[25+1] ;
  int  placeCardID;
  int  beginRingID;
  hopStruct hops[7];
} plantStruct;

#define DESCEND SIZE_OF_LEDSEGS // default is to ascend
static const plantStruct plants[] PROGMEM = {
  {                          // plant #0
    "None",                  // plantName
    0,                       // placeCardID
    0,                       // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 1 (guest placed card in reader)", // textMSG
      }
    }
  },
  {                          // plant #1
    "Maize",                 // plantName
    1,                       // placeCardID
    58,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches W. SOUTH AMERICA)", // textMSG
        {                    // nextButtons
          67
        },
        {                    // steps
          67 + DESCEND, 65, 66
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches ARIZONA/NEW MEXICO)", // textMSG
        {                    // nextButtons
          54
        },
        {                    // steps
          54, 57
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches E. NORTH AMERICA)", // textMSG
        {                    // nextButtons
          52
        },
        {                    // steps
          52, 53
        }
      },                     // end Hop_3
      {                      // Hop_4
        "Step 5 (guest touches CANADA)", // textMSG
        {                    // nextButtons
          50
        },
        {                    // steps
          50, 51
        }
      },                     // end Hop_4
      {                      // Hop_5
        "Step 6 (guest touches SPAIN or AFRICA or ASIA)", // textMSG
        {                    // nextButtons
          41, 90, 34

        },
        {                    // steps
          41, 90, 34, 59, 62, 61, 80, 84, 88, 89, 4, 3, 10, 25, 24, 33
        }
      }
    }                        // end Hops  
  },                         // end plant

  {                          // plant #2
    "Potato",                // plantName
    2,                       // placeCardID
    73,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches W. SOUTH AMERICA or CHILE)", // textMSG
        {                    // nextButtons
          67, 75
        },
        {                    // steps
          67, 75, 72, 74
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches MEXICO or CARIBBEAN)", // textMSG
        {                    // nextButtons
          58, 77
        },
        {                    // steps
          58, 77, 66, 65, 76
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches SPAIN)", // textMSG
        {                    // nextButtons
          41
        },
        {                    // steps
          41, 59, 62
        }
      },                     // end Hop_3
      {                      // Hop_4
        "Step 5 (guest touches EUROPE or BRITISH ISLES)", // textMSG
        {                    // nextButtons
          39, 44
        },
        {                    // steps
          39, 44, 40, 42, 46
        }
      },                     // end Hop_4
      {                      // Hop_5
        "Step 6 (guest touches AFRICA INDIA or AUSTRALIA)", // textMSG
        {                    // nextButtons
          90, 29, 1
        },
        {                    // steps
          90, 29, 1, 61, 80, 84, 88, 89, 90, 4, 3, 10, 25, 28, 2
        }
      },                     // end Hop_5
      {                      // Hop_6
        "Step 7 (guest touches NEW ENGLAND)", // textMSG
        {                    // nextButtons
          49

        },
        {                    // steps
          49, 45, 48
        }
      }
    }                        // end Hops  
  },                         // end plant

  {                          // plant #3
    "Apple",                 // plantName
    3,                       // placeCardID
    35,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches TURKEY)", // textMSG
        {                    // nextButtons
          37
        },
        {                    // steps
          37, 36
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches EUROPE)", // textMSG
        {                    // nextButtons
          39
        },
        {                    // steps
          39, 38
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches NEW ENGLAND)", // textMSG
        {                    // nextButtons
          49
        },
        {                    // steps
          49, 43, 47, 48
        }
      }
    }                        // end Hops  
  },                         // end plant

  {                          // plant #4
    "Asian_Rice",            // plantName
    4,                       // placeCardID
    17,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches INDIA)", // textMSG
        {                    // nextButtons
          29
        },
        {                    // steps
          29, 23, 32
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches JAPAN KOREA or SE ASIA)", // textMSG
        {                    // nextButtons
          20, 22, 15, 19
        },
        {                    // steps
          20, 22, 15, 19, 21, 16
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches CALIFORNIA)", // textMSG
        {                    // nextButtons
          55
        },
        {                    // steps
          55, 18, 56
        }
      }
    }                        // end Hops  
  },                         // end plant

  {                          // plant #5
    "African_Rice",          // plantName
    5,                       // placeCardID
    86,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches WEST AFRICA)", // textMSG
        {                    // nextButtons
          82
        },
        {                    // steps
          82, 85
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches BRAZIL)", // textMSG
        {                    // nextButtons
          7
        },
        {                    // steps
          7, 83, 79, 8
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches SOUTH CAROLINA)", // textMSG
        {                    // nextButtons
          64
        },
        {                    // steps
          64, 81, 60, 63
        }
      }
    }                        // end Hops  
  },                         // end plant

  {                          // plant #6
    "Indo_Atlantic_Coconut", // plantName
    6,                       // placeCardID
    29,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches EAST AFRICA)", // textMSG
        {                    // nextButtons
          31
        },
        {                    // steps
          31, 30
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches WEST AFRICA BRAZIL or CARIBBEAN)", // textMSG
        {                    // nextButtons
          82, 7, 77
        },
        {                    // steps
          82, 7, 77, 28, 25, 10, 3, 4, 88, 87, 5, 6, 9, 78
        }
      }
    }                        // end Hops  
  },                         // end plant

  {                          // plant #7
    "Pacific_Coconut",       // plantName
    7,                       // placeCardID
    12,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches SE ASIA or MADAGASCAR)", // textMSG
        {                    // nextButtons
          15, 27
        },
        {                    // steps
          15, 27, 14, 11, 26
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches PANAMA or W. SOUTH AMERICA)", // textMSG
        {                    // nextButtons
          71, 67
        },
        {                    // steps
          71, 67, 13, 69, 70, 68+DESCEND
        }
      }
    }                        // end Hops  
  },                         // end plant

};

#define SIZE_OF_PLANTS         LENGTH_OF_ARRAY(plants)
#define SIZE_OF_HOPS           LENGTH_OF_ARRAY(plants[0].hops)
#define SIZE_OF_NEXTBUTTONS    LENGTH_OF_ARRAY(plants[0].hops[0].nextButtons)
#define SIZE_OF_STEPS          LENGTH_OF_ARRAY(plants[0].hops[0].steps)


#endif  // MigrationData_h
