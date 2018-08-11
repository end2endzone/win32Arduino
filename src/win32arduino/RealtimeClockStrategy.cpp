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
