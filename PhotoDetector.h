#include <Arduino.h>

#ifndef IFDEBUG
#define IFDEBUG(...) ((void)((DEBUG_LEVEL) && (__VA_ARGS__, 0)))
#define IFDEBUG 1 // set to 1 to compile in Serial Debug prints
#endif

#ifndef LENGTH_OF_ARRAY
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(*x)))
#endif

struct Pins {
  uint8_t sig;
  uint8_t gnd;
};

class movingAvg
{
  public:
    movingAvg(int samples);
    ~movingAvg();
    int reading(int newReading);
    int getAvg();
    int getCount() {
      return m_nbrReadings;
    }
    void reset();
    int* getReadings() {
      return m_readings;
    }

  private:
    int m_samples;      // number of data points for the moving average
    int m_nbrReadings;  // number of readings
    long m_sum;         // sum of the m_readings array
    int m_next;         // index to the next reading
    int *m_readings;    // pointer to the dynamically allocated samples array
};

class PhotoCell : public movingAvg {
  public:
    PhotoCell(Pins _pins, int samples);
    ~PhotoCell();
    void updateSample();
    void PrintPins();
    bool GetBit();

  protected:
    Pins pins;
};

class PhotoDetector {
  private:
    int nChannels;
    unsigned long timerA2D;
    unsigned long periodA2D;
    unsigned long timerAvg;
    unsigned long periodUpdateAvg;
    int prvAvg = 0;
    int percentChange = 30;

  public:
    PhotoDetector(Pins _pins, int samples);
    ~PhotoDetector();
    PhotoCell* ldr; // not sure why it must be fixed.

    void update();
    void setDebugMask(unsigned int mask);
    void setInterval( unsigned long periodA2D_, unsigned long periodUpdateAvg_ ); // periodA2D in ms to update the detected value from the running average.
    int getAvg();
    void setpercentChange(int percentChange_);
    bool detectorState = 0;

};
