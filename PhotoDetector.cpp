#include "PhotoDetector.h"

// Constructor
movingAvg::movingAvg(int samples)
{
  m_samples = samples;
  m_nbrReadings = 0;
  m_sum = 0;
  m_next = 0;
  m_readings = new int[m_samples];
}

// Destructructor
movingAvg::~movingAvg()
{
  delete [] m_readings;
}

// add a new reading and return the new moving average
int movingAvg::reading(int newReading)
{
  // add each new data point to the sum until the m_readings array is filled
  if (m_nbrReadings < m_samples)
  {
    ++m_nbrReadings;
    m_sum = m_sum + newReading;
  }
  // once the array is filled, subtract the oldest data point and add the new one
  else
  {
    m_sum = m_sum - m_readings[m_next] + newReading;
  }

  m_readings[m_next] = newReading;
  if (++m_next >= m_samples) m_next = 0;
  return (m_sum + m_nbrReadings / 2) / m_nbrReadings;
}

// just return the current moving average
int movingAvg::getAvg()
{
  return (m_sum + m_nbrReadings / 2) / m_nbrReadings;
}

// start the moving average over again
void movingAvg::reset()
{
  m_nbrReadings = 0;
  m_sum = 0;
  m_next = 0;
}

// Constructor
PhotoCell::PhotoCell(Pins _pins, int samples) : movingAvg(samples)
{
  // store the pins parameter that make up a LDR.
  pins = _pins;

  // initialize the pins so that one has internal pull up and the other is GND.
  pinMode(pins.sig, INPUT_PULLUP); // pin to measure signal.
  if (pins.gnd != 0) {
    // only initialize GND pin if greater than zero.
    // if so then set to output and set to low as to provide a cheap GND.
    pinMode(pins.gnd, OUTPUT);
    digitalWrite(pins.gnd, LOW);
  }
}

// Destructructor
PhotoCell::~PhotoCell() {
}

void PhotoCell::updateSample() {
  // put a measured A2D value into the running average buffers.
  movingAvg::reading(analogRead(pins.sig));
}

void PhotoCell::PrintPins() {
  // nice print to verify things.
  Serial.print(F("pins.sig = ")); Serial.print(pins.sig); Serial.print(F(" "));
  Serial.print(F("pins.gnd = ")); Serial.print(pins.gnd); Serial.print(F(" "));
}

// Constructor
PhotoDetector::PhotoDetector(Pins _pins, int _samples) {
  ldr = new PhotoCell(_pins, _samples);
  setInterval(100, 1000); //default to 1s
  detectorState = 0;
  percentChange = 60;

};

// Destructructor
PhotoDetector::~PhotoDetector() {
}

void PhotoDetector::setInterval(unsigned long periodA2D_, unsigned long periodUpdateAvg_) {
  unsigned long time_ = millis();
  timerA2D = time_;
  periodA2D = periodA2D_;
  timerAvg = time_;
  periodUpdateAvg = periodUpdateAvg_;
}

void PhotoDetector::setpercentChange(int percentChange_) {
  percentChange = percentChange_;
}

// just return the current moving average
int PhotoDetector::getAvg()
{
  return ldr->getAvg();
}


void PhotoDetector::update() {
  unsigned long time_ = millis();
  int currentAvg = ldr->getAvg();

  if ( (unsigned long) (time_ - timerA2D) >= periodA2D ) {
    // time to update running averages with new samples
    timerA2D = timerA2D + periodA2D;

    ldr->updateSample(); // update each sample of the running average.
  }

  if ( (unsigned long) (time_ - timerAvg) >= periodUpdateAvg ) {
    int diff = 0;
    int percent = 0;

    // time to update running reported averages.
    timerAvg = timerAvg + periodUpdateAvg;
    diff = prvAvg - currentAvg;
    percent = 1 - ((diff * 100) / prvAvg);

    if (percent > percentChange) {
      detectorState = 1;
    }
    else if (percent < -(percentChange)) {
      detectorState = 0;
    }

    prvAvg = currentAvg;
  }
}
