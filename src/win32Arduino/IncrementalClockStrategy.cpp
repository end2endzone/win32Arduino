#include "IncrementalClockStrategy.h"

namespace testarduino
{

  static uint32_t gMicroResolution = 8; //8 usec resolution (increment for each calls)
  static uint32_t gMicroCounter = 0;

  IncrementalClockStrategy::IncrementalClockStrategy()
  {
  }

  IncrementalClockStrategy::~IncrementalClockStrategy()
  {
  }

  IncrementalClockStrategy & IncrementalClockStrategy::getInstance()
  {
    static IncrementalClockStrategy _instance;
    return _instance;
  }

  uint32_t IncrementalClockStrategy::millis()
  {
    //based on micro
    testarduino::gMicroCounter += testarduino::gMicroResolution;
  
    uint32_t microSeconds = testarduino::gMicroCounter;
    uint32_t milliSeconds = microSeconds/1000;
    return milliSeconds;
  }

  uint32_t IncrementalClockStrategy::micros()
  {
    //counter increments
    testarduino::gMicroCounter += testarduino::gMicroResolution;
    return testarduino::gMicroCounter;
  }

  void IncrementalClockStrategy::setMicrosecondsResolution(uint32_t iResolution)
  {
    gMicroResolution = iResolution;
  }

  const uint32_t & IncrementalClockStrategy::getMicrosecondsResolution() const
  {
    return gMicroResolution;
  }

  void IncrementalClockStrategy::setMicrosecondsCounter(uint32_t iCounter)
  {
    gMicroCounter = iCounter;
  }

  const uint32_t & IncrementalClockStrategy::getMicrosecondsCounter() const
  {
    return gMicroCounter;
  }

} //testarduino
