#ifndef REALTIMECLOCKSTRATEGY_H
#define REALTIMECLOCKSTRATEGY_H

#include "IClockStrategy.h"

namespace testarduino
{
  class RealtimeClockStrategy : public virtual IClockStrategy
  {
  private:
    RealtimeClockStrategy();
    virtual ~RealtimeClockStrategy();
  public:
    static RealtimeClockStrategy & getInstance();

    virtual uint32_t millis();
    virtual uint32_t micros();
  };
} //testarduino

#endif //REALTIMECLOCKSTRATEGY_H
