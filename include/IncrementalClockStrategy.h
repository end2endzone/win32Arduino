/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

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
