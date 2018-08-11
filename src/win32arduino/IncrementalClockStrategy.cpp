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

#include "IncrementalClockStrategy.h"

namespace testarduino
{

  static uint32_t gMicroResolution = 8; //8 usec resolution (increment for each calls)
  static uint32_t gMicroCounter = 0;

  IncrementalClockStrategy::IncrementalClockStrategy()
  {
  }

  IncrementalClockStrategy::~IncrementalClockStrategy()
  {
  }

  IncrementalClockStrategy & IncrementalClockStrategy::getInstance()
  {
    static IncrementalClockStrategy _instance;
    return _instance;
  }

  uint32_t IncrementalClockStrategy::millis()
  {
    //based on micro
    testarduino::gMicroCounter += testarduino::gMicroResolution;
  
    uint32_t microSeconds = testarduino::gMicroCounter;
    uint32_t milliSeconds = microSeconds/1000;
    return milliSeconds;
  }

  uint32_t IncrementalClockStrategy::micros()
  {
    //counter increments
    testarduino::gMicroCounter += testarduino::gMicroResolution;
    return testarduino::gMicroCounter;
  }

  void IncrementalClockStrategy::setMicrosecondsResolution(uint32_t iResolution)
  {
    gMicroResolution = iResolution;
  }

  const uint32_t & IncrementalClockStrategy::getMicrosecondsResolution() const
  {
    return gMicroResolution;
  }

  void IncrementalClockStrategy::setMicrosecondsCounter(uint32_t iCounter)
  {
    gMicroCounter = iCounter;
  }

  const uint32_t & IncrementalClockStrategy::getMicrosecondsCounter() const
  {
    return gMicroCounter;
  }

} //testarduino
