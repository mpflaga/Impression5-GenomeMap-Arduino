#include "Arduino.h"
#include "pgmStrToRAM.h"

#ifndef LENGTH_OF_ARRAY
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(*x)))
#endif

#ifndef RFid_h
#define RFid_h
#include "CRC32.h"
#include "PhotCellArrayAveraged.h"
#include "RFidData.h"

class RFid : public PhotoCell {
  public:
    RFid(Pins _pins, int samples);
    ~RFid();
void begin(Stream &serialRFid);
    bool available();
    int read();
    bool cardPresent;

  protected:
    char RFidBuffer[12 + 1];
    int8_t RFidBuffer_pos = 0;
    Stream* _serialRFid = &Serial;
    int detectedChipID;
    CRC32 crc;
    uint32_t lastDetectedCRC32;
    unsigned int delayToSampleThreshold = 2000;
    long unsigned whenToSampleThreshold;
};

#endif
