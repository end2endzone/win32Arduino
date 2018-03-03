#include "IncrementalTimeStrategy.h"

namespace testarduino
{

  static uint32_t gMicroResolution = 8; //8 usec resolution (increment for each calls)
  static uint32_t gMicroCounter = 0;

  IncrementalTimeStrategy::IncrementalTimeStrategy()
  {
  }

  IncrementalTimeStrategy::~IncrementalTimeStrategy()
  {
  }

  IncrementalTimeStrategy & IncrementalTimeStrategy::getInstance()
  {
    static IncrementalTimeStrategy _instance;
    return _instance;
  }

  uint32_t IncrementalTimeStrategy::millis()
  {
    //based on micro
    testarduino::gMicroCounter += testarduino::gMicroResolution;
  
    uint32_t microSeconds = testarduino::gMicroCounter;
    uint32_t milliSeconds = microSeconds/1000;
    return milliSeconds;
  }

  uint32_t IncrementalTimeStrategy::micros()
  {
    //counter increments
    testarduino::gMicroCounter += testarduino::gMicroResolution;
    return testarduino::gMicroCounter;
  }

  void IncrementalTimeStrategy::setMicrosecondsResolution(uint32_t iResolution)
  {
    gMicroResolution = iResolution;
  }

  const uint32_t & IncrementalTimeStrategy::getMicrosecondsResolution() const
  {
    return gMicroResolution;
  }

  void IncrementalTimeStrategy::setMicrosecondsCounter(uint32_t iCounter)
  {
    gMicroCounter = iCounter;
  }

  const uint32_t & IncrementalTimeStrategy::getMicrosecondsCounter() const
  {
    return gMicroCounter;
  }

} //testarduino
