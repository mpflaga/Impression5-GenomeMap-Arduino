#ifndef LENGTH_OF_ARRAY
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(*x)))
#endif

#ifndef MigrationData_h
#define MigrationData_h

typedef enum {
  None               , //  0
  AFRICA             , //  1 0x5b-05
  ARIZONA_NEW_MEXICO , //  2 0x5d-05
  ASIA               , //  3 0x5a-07
  AUSTRALIA          , //  4 0x5a-00
  BOLIVIA_PERU       , //  5 0x5c-00
  BRAZIL             , //  6 0x5c-09
  BRITISH_ISLES      , //  7 0x5b-10
  CALIFORNIA         , //  8 0x5d-04
  CANADA             , //  9 0x5c-05
  CARIBBEAN          , // 10 0x5c-03
  CHILE              , // 11 0x5c-11
  CHINA              , // 12 0x5a-10
  E_NORTH_AMERICA    , // 13 0x5d-06
  EAST_AFRICA        , // 14 0x5b-04
  EUROPE             , // 15 0x5b-11
  INDIA              , // 16 0x5a-05
  JAPAN              , // 17 0x5a-09
  KAZAKHSTAN         , // 18 0x5a-06
  KOREA              , // 19 0x5a-08
  MADAGASCAR         , // 20 0x5b-03
  MALI               , // 21 0x5d-08
  MEXICO             , // 22 0x5b-07
  NEW_ENGLAND        , // 23 0x5c-07
  PACIFIC_ISLANDS    , // 24 0x5a-11
  PANAMA             , // 25 0x5c-02
  SE_ASIA            , // 26 0x5a-01
  SOUTH_CAROLINA     , // 27 0x5d-10
  SPAIN              , // 28 0x5b-09
  TURKEY             , // 29 0x5b-00
  W_SOUTH_AMERICA    , // 30 0x5c-01
  WEST_AFRICA          // 31 0x5b-06
} Countries;

// needs to be in same order as Countries.
static const char regions[][20] PROGMEM = {
  "None"              , //  0
  "AFRICA"            , //  1
  "ARIZONA_NEW_MEXICO", //  2
  "ASIA"              , //  3
  "AUSTRALIA"         , //  4
  "BOLIVIA_PERU"      , //  5
  "BRAZIL"            , //  6
  "BRITISH_ISLES"     , //  7
  "CALIFORNIA"        , //  8
  "CANADA"            , //  9
  "CARIBBEAN"         , // 10
  "CHILE"             , // 11
  "CHINA"             , // 12
  "E_NORTH_AMERICA"   , // 13
  "EAST_AFRICA"       , // 14
  "EUROPE"            , // 15
  "INDIA"             , // 16
  "JAPAN"             , // 17
  "KAZAKHSTAN"        , // 18
  "KOREA"             , // 19
  "MADAGASCAR"        , // 20
  "MALI"              , // 21
  "MEXICO"            , // 22
  "NEW_ENGLAND"       , // 23
  "PACIFIC_ISLANDS"   , // 24
  "PANAMA"            , // 25
  "SE_ASIA"           , // 26
  "SOUTH_CAROLINA"    , // 27
  "SPAIN"             , // 28
  "TURKEY"            , // 29
  "W_SOUTH_AMERICA"   , // 30
  "WEST_AFRICA"         // 31
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
  { 937,  969,  None              }, // 60
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
  {1374, 1389,  AFRICA            }, // 90
  { 141,  141,  None              }, // 91
  {1389, 1389,  None              }, // 93 // Need last value to be max LED for autodetect Length of LEDs.

};
#define SIZE_OF_LEDSEGS LENGTH_OF_ARRAY(ledSegs)

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
    "Potato",                // plantName
    1,                       // placeCardID
    73,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches W. SOUTH AMERICA or CHILE)", // textMSG
        {                    // nextButtons
          67, 75
        },
        {                    // steps
          72 + DESCEND, 74
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches MEXICO or CARIBBEAN)", // textMSG
        {                    // nextButtons
          58, 77
        },
        {                    // steps
          66 + DESCEND, 65 + DESCEND, 76
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches SPAIN)", // textMSG
        {                    // nextButtons
          41
        },
        {                    // steps
          59, 62 + DESCEND
        }
      },                     // end Hop_3
      {                      // Hop_4
        "Step 5 (guest touches EUROPE or BRITISH ISLES)", // textMSG
        {                    // nextButtons
          39, 44
        },
        {                    // steps
          40 + DESCEND, 42, 46 + DESCEND
        }
      },                     // end Hop_4
      {                      // Hop_5
        "Step 6 (guest touches AFRICA INDIA or AUSTRALIA)", // textMSG
        {                    // nextButtons
          90, 29, 1
        },
        {                    // steps
          61 + DESCEND, 80 + DESCEND, 84, 88, 89, 4 + DESCEND, 3 + DESCEND, 10, 25 + DESCEND, 28, 2 + DESCEND
        }
      },                     // end Hop_5
      {                      // Hop_6
        "Step 7 (guest touches NEW ENGLAND)", // textMSG
        {                    // nextButtons
          49

        },
        {                    // steps
          45, 48
        }
      }
    }                        // end Hops
  },                         // end plant

  {                          // plant #2
    "African_Rice",          // plantName
    2,                       // placeCardID
    86,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches WEST AFRICA)", // textMSG
        {                    // nextButtons
          82
        },
        {                    // steps
          85 + DESCEND
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches BRAZIL)", // textMSG
        {                    // nextButtons
          7
        },
        {                    // steps
          83, 79 + DESCEND, 8 + DESCEND, 91 // MPF - WIP need to add LED141.
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches SOUTH CAROLINA)", // textMSG
        {                    // nextButtons
          64
        },
        {                    // steps
          81, 60 + DESCEND, 63
        }
      }
    }                        // end Hops
  },                         // end plant

  {                          // plant #3
    "Maize",                 // plantName
    3,                       // placeCardID
    58,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches W. SOUTH AMERICA)", // textMSG
        {                    // nextButtons
          67
        },
        {                    // steps
          65, 66
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches ARIZONA/NEW MEXICO)", // textMSG
        {                    // nextButtons
          54
        },
        {                    // steps
          57 + DESCEND
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches E. NORTH AMERICA)", // textMSG
        {                    // nextButtons
          52
        },
        {                    // steps
          53 + DESCEND
        }
      },                     // end Hop_3
      {                      // Hop_4
        "Step 5 (guest touches CANADA)", // textMSG
        {                    // nextButtons
          50
        },
        {                    // steps
          51 + DESCEND
        }
      },                     // end Hop_4
      {                      // Hop_5
        "Step 6 (guest touches SPAIN or AFRICA or ASIA)", // textMSG
        {                    // nextButtons
          41, 90, 34

        },
        {                    // steps
          59, 62 + DESCEND, 61 + DESCEND, 80 + DESCEND, 84, 88, 89, 4 + DESCEND, 3 + DESCEND, 10, 25 + DESCEND, 24 + DESCEND, 33
        }
      }
    }                        // end Hops
  },                         // end plant

  {                          // plant #4
    "Apple",                 // plantName
    4,                       // placeCardID
    35,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches TURKEY)", // textMSG
        {                    // nextButtons
          37
        },
        {                    // steps
          36
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches EUROPE)", // textMSG
        {                    // nextButtons
          39
        },
        {                    // steps
          38
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches NEW ENGLAND)", // textMSG
        {                    // nextButtons
          49
        },
        {                    // steps
          43, 47, 48
        }
      }
    }                        // end Hops
  },                         // end plant

  {                          // plant #5
    "Indo_Atlantic_Coconut", // plantName
    5,                       // placeCardID
    29,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches EAST AFRICA)", // textMSG
        {                    // nextButtons
          31
        },
        {                    // steps
          30
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches WEST AFRICA, BRAZIL or CARIBBEAN)", // textMSG
        {                    // nextButtons
          82, 7, 77
        },
        {                    // steps
          28 + DESCEND, 25, 10 + DESCEND, 3, 4, 88 + DESCEND, 87 + DESCEND, 5, 6, 9 + DESCEND, 78 + DESCEND
        }
      }
    }                        // end Hops
  },                         // end plant

  {                          // plant #6
    "Asian_Rice",            // plantName
    6,                       // placeCardID
    17,                      // beginRingID
    {                        // Hops
      {                      // Hop_1
        "Step 2 (guest touches INDIA)", // textMSG
        {                    // nextButtons
          29
        },
        {                    // steps
          23, 32 + DESCEND
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches JAPAN KOREA or SE ASIA)", // textMSG
        {                    // nextButtons
          20, 22, 15
        },
        {                    // steps
          19, 21, 16 + DESCEND
        }
      },                     // end Hop_2
      {                      // Hop_3
        "Step 4 (guest touches CALIFORNIA)", // textMSG
        {                    // nextButtons
          55
        },
        {                    // steps
          18, 56 + DESCEND
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
          14, 11 + DESCEND, 26
        }
      },                     // end Hop_1
      {                      // Hop_2
        "Step 3 (guest touches PANAMA or W. SOUTH AMERICA)", // textMSG
        {                    // nextButtons
          71, 67
        },
        {                    // steps
          13, 69 + DESCEND, 70, 68 + DESCEND
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
