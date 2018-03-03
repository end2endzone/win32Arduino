#ifndef ICLOCKSTRATEGY_H
#define ICLOCKSTRATEGY_H

#include <stdint.h>

namespace testarduino
{
  class IClockStrategy
  {
  public:

    ///<summary>
    ///Returns the number of milliseconds since the Arduino board began running the current program.
    ///</summary>
    ///<remarks>The returned values returned by the implementation is specific.</remarks>
    ///<returns>Returns the number of milliseconds since the Arduino board began running.</returns>
    virtual uint32_t millis() = 0;

    ///<summary>
    ///Returns the number of microseconds since the Arduino board began running the current program.
    ///</summary>
    ///<remarks>The returned values returned by the implementation is specific.</remarks>
    ///<returns>Returns the number of microseconds since the Arduino board began running.</returns>
    virtual uint32_t micros() = 0;
  };
} //testarduino

#endif //ICLOCKSTRATEGY_H
