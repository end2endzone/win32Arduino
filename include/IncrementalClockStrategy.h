#ifndef INCREMENTALCLOCKSTRATEGY_H
#define INCREMENTALCLOCKSTRATEGY_H

#include "IClockStrategy.h"

namespace testarduino
{
  class IncrementalClockStrategy : public virtual IClockStrategy
  {
  private:
    IncrementalClockStrategy();
    virtual ~IncrementalClockStrategy();
  public:
    static IncrementalClockStrategy & getInstance();
    virtual uint32_t millis();
    virtual uint32_t micros();

    ///<summary>
    ///Defines the minimum increments of the internal microseconds counter for every call to the micros() function.
    ///</summary>
    ///<param name="iResolution">The resolution of the microseconds counter.</param>
    void setMicrosecondsResolution(uint32_t iResolution);

    const uint32_t & getMicrosecondsResolution() const;

    void setMicrosecondsCounter(uint32_t iCounter);

    const uint32_t & getMicrosecondsCounter() const;
  };
} //testarduino

#endif //INCREMENTALCLOCKSTRATEGY_H
