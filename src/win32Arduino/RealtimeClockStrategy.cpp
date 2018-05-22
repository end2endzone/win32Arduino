#include "RealtimeClockStrategy.h"

#include "rapidassist/time_.h"

namespace testarduino
{
  RealtimeClockStrategy::RealtimeClockStrategy()
  {
    //init both timers
    millis();
    micros();
  }

  RealtimeClockStrategy::~RealtimeClockStrategy()
  {
  }

  RealtimeClockStrategy & RealtimeClockStrategy::getInstance()
  {
    static RealtimeClockStrategy _instance;
    return _instance;
  }

  uint32_t RealtimeClockStrategy::millis()
  {
    double seconds = ra::time::getMillisecondsTimer();
    uint32_t milliseconds = (uint32_t)seconds*1000;
    return milliseconds;
  }

  uint32_t RealtimeClockStrategy::micros()
  {
    double seconds = ra::time::getMicrosecondsTimer();
    uint32_t microseconds = (uint32_t)seconds*1000*1000;
    return microseconds;
  }

} //testarduino
