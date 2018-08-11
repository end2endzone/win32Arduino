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
