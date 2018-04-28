#include "RealtimeClockStrategy.h"

#include <time.h>     //for clock_t

namespace testarduino
{

  static clock_t app_clock_init()
  {
    return ::clock();
  }

  static clock_t gClockAppStartTime = app_clock_init();

  RealtimeClockStrategy::RealtimeClockStrategy()
  {
  }

  RealtimeClockStrategy::~RealtimeClockStrategy()
  {
  }

  RealtimeClockStrategy & RealtimeClockStrategy::getInstance()
  {
    static RealtimeClockStrategy _instance;
    return _instance;
  }

  double RealtimeClockStrategy::clockDiff(clock_t clockEnd, clock_t clockStart)
  {
    static double CLOCKS_PER_MS = CLOCKS_PER_SEC/1000.0;
    clock_t diffticks=clockEnd-clockStart;
    double diffms=(diffticks)/CLOCKS_PER_MS;
    return diffms;
  }

  uint32_t RealtimeClockStrategy::millis()
  {
    clock_t now = ::clock();
    double diffMs = clockDiff(now, testarduino::gClockAppStartTime);
    uint32_t diffFinal = (uint32_t)diffMs;
    return diffFinal;
  }

  uint32_t RealtimeClockStrategy::micros()
  {
    //based on millis() implementation.

    //copy millis() implementation
    clock_t now = ::clock();
    double diffMs = clockDiff(now, testarduino::gClockAppStartTime);
    double diffMicros = diffMs * 1000;

    static const uint32_t MAX_MICROS = (uint32_t)0xFFFFFFFF;
    while(diffMicros > (double)MAX_MICROS)
    {
      diffMicros -= (double)(MAX_MICROS);
    }
    uint32_t finalMicros = (uint32_t)diffMicros;
    return finalMicros;
  }

} //testarduino
