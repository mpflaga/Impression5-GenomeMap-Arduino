#include "Arduino.h"

struct Pins {
  uint8_t sig;
  uint8_t gnd;
};

class PhotoCell {
  public:
    PhotoCell(Pins _pins, int samples);
    ~PhotoCell();
    int read();
    int updateReading();
    int average;
    int getCount() {
      return m_nbrReadings;
    }
    void reset();
    int* getReadings() {
      return m_readings;
    }
    int threshold;
    bool thresholdStatus;

  protected:
    Pins pins;
    int m_samples;      // number of data points for the moving average
    int m_nbrReadings;  // number of readings
    long m_sum;         // sum of the m_readings array
    int m_next;         // index to the next reading
    int *m_readings;    // pointer to the dynamically allocated samples array
    unsigned m_period;
    unsigned long m_nextReadingTime;
};
