#ifndef REALTIMESTRATEGY_H
#define REALTIMESTRATEGY_H

#include "ITimeStrategy.h"
#include <time.h> //for clock_t

namespace testarduino
{
  class RealtimeStrategy : public virtual ITimeStrategy
  {
  private:
    RealtimeStrategy();
    virtual ~RealtimeStrategy();
  public:
    static RealtimeStrategy & getInstance();

    ///<summary>
    ///Calculate the elapsed time in milliseconds between two clock_t events.
    ///</summary>
    ///<param name="clockEnd">The time when the event ends.</param>
    ///<param name="clockStart">The time when the event starts.</param>
    ///<returns>Returns elapsed time in milliseconds between two clock_t events.</returns>
    static double clockDiff(clock_t clockEnd, clock_t clockStart);

    virtual uint32_t millis();
    virtual uint32_t micros();
  };
} //testarduino

#endif //REALTIMESTRATEGY_H
