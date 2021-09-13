#include "PhotoCellNumber.h"

//constructor
movingAvg::movingAvg(int samples)
{
  m_samples = samples;
  m_nbrReadings = 0;
  m_sum = 0;
  m_next = 0;
  m_readings = new int[m_samples];
}

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

// constructor
PhotoCell::PhotoCell(Pins _pins, int samples) : movingAvg(samples)
{
  // store the pins parameter that make up a LDR.
  pins = _pins;
  invert = false;

  // initialize the pins so that one has internal pull up and the other is GND.
  pinMode(pins.sig, INPUT_PULLUP); // pin to measure signal.
  if (pins.gnd != 0) {
    // only initialize GND pin if greater than zero.
    // if so then set to output and set to low as to provide a cheap GND.
    pinMode(pins.gnd, OUTPUT);
    digitalWrite(pins.gnd, LOW);
  }
}

void PhotoCell::updateSample() {
  // put a measured A2D value into the running average buffers.
  movingAvg::reading(analogRead(pins.sig));
}

void PhotoCell::PrintPins() {
  // nice print to verify things.
  Serial.print("pins.sig = "); Serial.print(pins.sig); Serial.print(" ");
  Serial.print("pins.gnd = "); Serial.print(pins.gnd); Serial.print(" ");
  Serial.print("pins.thr = "); Serial.print(pins.thr); Serial.println();
}

bool PhotoCell::GetBit() {
  // return a bit that is converted from the running average value compared to a threshold.
  bool value;
  if ( !invert ? (getAvg() > pins.thr) : (getAvg() < pins.thr)) {
    value = true;
  } else {
    value = false;
  }
  return value;
}

void PhotoCell::setThreshold(int thr) {
  // incase the values need to be adjusted on the fly.
  pins.thr = thr;
}

void PhotoCell::setInvert(bool invert_) {
  // incase the values need to be adjusted on the fly.
  invert = invert_;
}
//constructor
PhotoCellNumber::PhotoCellNumber(Pins xMotion_[], int nChannels_, int avgSize) {
  // set initial values.
  xMotion = xMotion_;
  nChannels = nChannels_;

  //PhotoCell* ldr[nChannels]; // not sure why this does not work.
  // PhotoCell ldr[0](Pins xMotion[0]);
  PhotoCell *ldr1 = (PhotoCell *)malloc(nChannels);

  for (int i = 0; i < nChannels; i++)
  {
    ldr[i] = new PhotoCell(xMotion[i], avgSize);
  }
  setInterval(100, 1000); //default to 1s
  value = 0;
};

void PhotoCellNumber::printArray() {
  for (int i = 0; i < nChannels; i++)
  {
    ldr[i]->PrintPins();
  }
};

void PhotoCellNumber::setInvert(bool invert_) {
  for (int i = 0; i < nChannels; i++)
  {
    ldr[i]->setInvert(invert_);
  }
}

void PhotoCellNumber::setInterval(unsigned long periodA2D_, unsigned long periodUpdateAvg_) {
  unsigned long time_ = millis();
  active = true;
  debugMask = 0;
  timerA2D = time_;
  periodA2D = periodA2D_;
  timerAvg = time_;
  periodUpdateAvg = periodUpdateAvg_;
}

void PhotoCellNumber::disable() {
  active = false;
}

void PhotoCellNumber::enable() {
  active = true;
}

void PhotoCellNumber::setDebugMask(unsigned int mask) {
  debugMask = mask;
}

void PhotoCellNumber::update() {
  unsigned long time_ = millis();

  if ( active && ((unsigned long) (time_ - timerA2D) >= periodA2D) ) {
    // time to update running averages with new samples
    timerA2D = timerA2D + periodA2D;
      if(debugMask & 0b10) {
        Serial.print(" ");Serial.println(time_);
      }

    for (int i = 0; i < nChannels; i++)
    {
      ldr[i]->updateSample(); // update each sample of the running average.
      if(debugMask & 0b01) {
        Serial.print(" "); Serial.print(ldr[i]->getAvg());
      }
    }
    if(debugMask & 0b01) {
      Serial.println();
    }
  }

  if ( active && ((unsigned long) (time_ - timerAvg) >= periodUpdateAvg) ) {
    // time to update running reported averages.
    timerAvg = timerAvg + periodUpdateAvg;
    //Serial.print("~");Serial.println(time_);

    // build up number from bits
    int value_ = 0;
    for (int i = 0; i < nChannels; i++)
    {
      value_ <<= 1;
      if ( ldr[i]->GetBit()) {
        value_ |= 0x1;
      } else {
      }
    }
    value = value_;
  }
}
