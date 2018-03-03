#ifndef INCREMENTALTIMESTRATEGY_H
#define INCREMENTALTIMESTRATEGY_H

#include "ITimeStrategy.h"

namespace testarduino
{
  class IncrementalTimeStrategy : public virtual ITimeStrategy
  {
  private:
    IncrementalTimeStrategy();
    virtual ~IncrementalTimeStrategy();
  public:
    static IncrementalTimeStrategy & getInstance();
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

#endif //INCREMENTALTIMESTRATEGY_H
