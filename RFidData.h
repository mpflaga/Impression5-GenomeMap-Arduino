
typedef struct {
  char cardId[12 + 1] ;
  int  placeCardID;
} RFidStruct;

static const RFidStruct RFtags[] PROGMEM = {
  // something else.
  { "0C00296BC18F", 0},

  // Potato
  { "5000E23935BE", 1},
  { "5000E1E4F5A0", 1},
  { "5000E248748E", 1},
  { "5000E20572C5", 1},
  { "600061E0D938", 1},

  // African_Rice
  { "5000E2238213", 2},
  { "5000E24904FF", 2},
  { "5000E1DCB1DC", 2},
  { "5000E2047FC9", 2},
  { "600061FB9268", 2},

  // Maize
  { "5000E1E77A2C", 3},
  { "5000E1E985DD", 3},
  { "5000E1DCFB96", 3},
  { "5000E22D9B04", 3},
  { "600061E11AFA", 3},

  // Apple
  { "5000E24907FC", 4},
  { "5000E25CB856", 4},
  { "5000E2357EF9", 4},
  { "5000E1FB80CA", 4},
  { "600061F59165", 4},

  // Indo_Atlantic_Coconut
  { "5000E26101D2", 5},
  { "5000E22D35AA", 5},
  { "5000E208CE74", 5},
  { "5000E1F94008", 5},
  { "600061F658AF", 5},

  // Asian_Rice
  { "5000E23D5ED1", 6},
  { "5000E2645781", 6},
  { "5000E240DF2D", 6},
  { "5000E1E0ADFC", 6},
  { "600061DC7DA0", 6},

  // Pacific_Coconut
  { "5000E146D324", 7},
  { "5000E24A9A62", 7},
  { "5000E20E55E9", 7},
  { "5000E2586A80", 7},
  { "6000615B7228", 7}
};
