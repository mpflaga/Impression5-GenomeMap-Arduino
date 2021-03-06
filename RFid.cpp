#include "RFid.h"

// Constructor
RFid::RFid(Pins _pins, int samples) : PhotoCell( _pins, samples)
{
  // Initialize Tag reader memory.
  memset(RFidBuffer, 0x0, LENGTH_OF_ARRAY(RFidBuffer));
  RFidBuffer_pos = 0;
}

// Destructructor
RFid::~RFid()
{

}

void RFid::begin(Stream &serialRFid) {

  Serial.print(F("RFidBuffer_size = "));  Serial.print(LENGTH_OF_ARRAY(RFidBuffer)); Serial.println();
  Serial.print(F("RFidBuffer_pos = "));  Serial.print(RFidBuffer_pos); Serial.println();
  _serialRFid = &serialRFid;
  detectedChipID = -1;
  cardPresent = 0;
  whenToSampleThreshold = 0;
  
  // initialize the CRC calculator.
  crc.reset();
  //crc.setPolynome(0x04C11DB8);

  // Loop thru each tag, to print Tag ID and matching place card ID.
  for (int idx = 0; idx < LENGTH_OF_ARRAY(RFtags); idx++) {
    Serial.print(F("RFtags.cardId[")); Serial.print(idx); Serial.print(F("]= '"));
    Serial.print(pgmStrToRAM(RFtags[idx].cardId));
    Serial.print(F("' is placeCardID = "));
    Serial.print((int) pgm_read_word(&RFtags[idx].placeCardID));
    Serial.print(F("', CRC is = '"));

    crc.restart();
    lastDetectedCRC32 = 0x0;
    crc.add((uint8_t*) pgmStrToRAM(RFtags[idx].cardId), 12);
    Serial.print(F("crc.getCRC() = '"));  Serial.print(crc.getCRC()); Serial.print(F("'"));
    Serial.println();

  }
}

bool RFid::available() {
  bool result = 0;
  int diff = 0;
  int percent = 0;
  long unsigned currentTime = millis();

  updateReading();

  // Look for new RF tag data.
  if (_serialRFid->available()) {
    // At least on new character is detected.

    int inByte = _serialRFid->read();

    if (( 0x20 <= inByte) && (inByte <= 0x127)) {
      // When the character printable?

      if (RFidBuffer_pos > LENGTH_OF_ARRAY(RFidBuffer) - 2) {
        // Shift RFidBuffer left when at buffer end. Then add new character to end.
        memmove	(	RFidBuffer, RFidBuffer + 1, (LENGTH_OF_ARRAY(RFidBuffer) - 2) );
        RFidBuffer[LENGTH_OF_ARRAY(RFidBuffer) - 2] = inByte;
        // RFidBuffer[LENGTH_OF_ARRAY(RFidBuffer) - 1] = 0x0; // not needed, but safer.
      }
      else {
        // Add new character directly to current position of RFidBuffer, when not at end.
        RFidBuffer[RFidBuffer_pos++] = inByte;
      }
    }

    if (inByte == 0x02) {
      // Detected First Character of an RFid tag.
      // Serial.print(F("STX detected'"));
      // Serial.print(F(", RFidBuffer_pos = '"));  Serial.print(RFidBuffer_pos); Serial.println(F("'"));
      RFidBuffer_pos = 0;
      memset(RFidBuffer, 0x0, LENGTH_OF_ARRAY(RFidBuffer));
      detectedChipID = -1;

    }
    if (inByte == 0x03 && RFidBuffer_pos == LENGTH_OF_ARRAY(RFidBuffer) - 1 ) {
      // Detected End Character of an RFid tag.

      // Serial.print(F("ETX detected'"));
      // Serial.print(F(", RFidBuffer_pos = '"));  Serial.print(RFidBuffer_pos); Serial.println(F("'"));
      crc.reset();
      crc.add((uint8_t*) RFidBuffer, 12);
      if (lastDetectedCRC32 != crc.getCRC() ) {
        // when RFid tag changed.
        lastDetectedCRC32 = crc.getCRC();

        // scan list to find if read tag is in list.
        for (int idx = 0; idx < LENGTH_OF_ARRAY(RFtags); idx++) {
          if (!strcmp_P(RFidBuffer, RFtags[idx].cardId)) { // strcmp_P returns 0 when match.
            // when a match is found.
            detectedChipID = (int) pgm_read_word(&RFtags[idx].placeCardID);
            Serial.print(F("Match found, detectedChipID = '"));  Serial.print(detectedChipID); Serial.println(F("'"));
            result = 1;
            break;
          }
        }
        if (detectedChipID == -1) {
          // When a match is not detected.
          Serial.print(F("Unknown RfID tag = '"));  Serial.print(RFidBuffer); Serial.print(F("'"));
          Serial.print(F(", CRC32 = '"));  Serial.print(crc.getCRC()); Serial.println(F("'"));
        }
        
        whenToSampleThreshold = (long unsigned) (whenToSampleThreshold + currentTime);        
        cardPresent = true;

        thresholdStatus = cardPresent;
        Serial.print(F("Card Detected, "));
        Serial.print(F("PhotoCell::average = ")); Serial.print(average);
        Serial.print(F(", PhotoCell::threshold = ")); Serial.println(threshold);
      }
    }
  }
  
  if ( (whenToSampleThreshold != 0) && (currentTime > whenToSampleThreshold)) {
    Serial.print(F("Threshold was set, "));
    Serial.print(F("PhotoCell::average = ")); Serial.print(average);
    Serial.print(F(", PhotoCell::threshold = ")); Serial.println(threshold);
    threshold = average / 2 ;
    whenToSampleThreshold = 0;
    Serial.print(F(", new threshold = ")); Serial.println(threshold);
  }

  if (cardPresent && !thresholdStatus) {
    Serial.print(F("Card was Removed, "));
    Serial.print(F("PhotoCell::average = ")); Serial.print(average);
    Serial.print(F(", PhotoCell::threshold = ")); Serial.println(threshold);

    cardPresent = thresholdStatus;
    detectedChipID = -1;
    result = 1;
    lastDetectedCRC32 = 0x0;
  }

  return result;
}

int RFid::read() {
  return detectedChipID;
}
