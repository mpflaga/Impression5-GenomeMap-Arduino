#include "PhotCellArrayAveraged.h"

PhotoCell::PhotoCell(Pins _pins, int samples) {
  pins = _pins;
  pinMode(pins.sig, INPUT_PULLUP);
  pinMode(pins.gnd, OUTPUT);
  digitalWrite(pins.gnd, LOW);
  m_samples = samples;
  m_readings = new int[m_samples];
  m_period = 1000 / samples; 
  threshold = 100;
  thresholdStatus = false;
  reset();
}

PhotoCell::~PhotoCell() {
  delete [] m_readings;
}

int PhotoCell::read() {

  return analogRead(pins.sig);
}

// add a new reading and return the new moving average
int PhotoCell::updateReading() {
  unsigned long currentTime = millis();
  int newReading = analogRead(pins.sig);
  
  if (currentTime > m_nextReadingTime) {
    m_nextReadingTime = (unsigned long) (currentTime + m_period);
    
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
    average = (m_sum + m_nbrReadings / 2) / m_nbrReadings;    
    thresholdStatus = (average > threshold) ? true : false;
    return average;
  }
}

// start the moving average over again
void PhotoCell::reset() {
  m_nbrReadings = 0;
  m_sum = 0;
  m_next = 0;
  m_nextReadingTime = (unsigned long) (millis() + m_period);
  average = 0;
}