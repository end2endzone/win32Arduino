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

#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include "Arduino.h"
#include <sstream>
#include <bitset>
#include <iomanip> //for std::precision
#include <ios> //for std::uppercase
#include <cstdio> //for sprintf

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

namespace testarduino
{
  class SerialPrinter
  {
  public:

    static void begin(uint16_t baudrate);
    static void begin(int16_t baudrate); //prevents warning when calling 'Serial.begin(115200)'
    static void begin(int baudrate); //prevents warning when calling 'Serial.begin(115200)'
    static void printString(const char * iValue);
    static void printlnString(const char * iValue);

    //enable implicit conversion
    //allow code like `while (!Serial);`
    operator bool() const { return true; }

    //Note: http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
    template <class T>
    inline void print(const T & t, int iRadix)
    {
      //See https://www.arduino.cc/en/Serial/Print

      std::stringstream out;
      switch(iRadix)
      {
      case DEC:
        out << t;
        break;
      case HEX:
        out << std::uppercase << std::hex << t;
        break;
      case OCT:
        out << std::oct << t;
        break;
      case BIN:
        {
          std::string allbits;
          switch(sizeof(T))
          {
          case 1:
            allbits = std::bitset<sizeof(T)*8>(static_cast<int8_t>(t)).to_string();
            break;
          case 2:
            allbits = std::bitset<sizeof(T)*8>(static_cast<int16_t>(t)).to_string();
            break;
          case 4:
            allbits = std::bitset<sizeof(T)*8>(static_cast<int32_t>(t)).to_string();
            break;
          };
          while(!allbits.empty() && allbits[0] == '0') //trim leading 0
            allbits.erase(allbits.begin());
          out << allbits;
        }
        break;
      };
      printString(out.str().c_str());
    }

    template <class T>
    inline void println(const T & t, int iRadix)
    {
      //See https://www.arduino.cc/en/Serial/Print

      std::stringstream out;
      switch(iRadix)
      {
      case DEC:
        out << t;
        break;
      case HEX:
        out << std::uppercase << std::hex << t;
        break;
      case OCT:
        out << std::oct << t;
        break;
      case BIN:
        {
          std::string allbits;
          switch(sizeof(T))
          {
          case 1:
            allbits = std::bitset<sizeof(T)*8>(static_cast<int8_t>(t)).to_string();
            break;
          case 2:
            allbits = std::bitset<sizeof(T)*8>(static_cast<int16_t>(t)).to_string();
            break;
          case 4:
            allbits = std::bitset<sizeof(T)*8>(static_cast<int32_t>(t)).to_string();
            break;
          };
          while(!allbits.empty() && allbits[0] == '0') //trim leading 0
            allbits.erase(allbits.begin());
          out << allbits;
        }
        break;
      };
      printlnString(out.str().c_str());
    }

    template <class T>
    inline void print(const T & t)
    {
      std::stringstream out;
      out << t;
      printString(out.str().c_str());
    }

    template <class T>
    inline void println(const T & t)
    {
      std::stringstream out;
      out << t;
      printlnString(out.str().c_str());
    }

    static void println();
  };

  //specializations
  
  //specializations: force 8 bit integer to 16 bits
  template<> inline void SerialPrinter::print<uint8_t>(const uint8_t & t, int iRadix)
  {
    SerialPrinter::print<uint16_t>(t, iRadix);
  }
  template<> inline void SerialPrinter::println<uint8_t>(const uint8_t & t, int iRadix)
  {
    SerialPrinter::print<uint16_t>(t, iRadix);
  }

  template<> inline void SerialPrinter::print<int8_t>(const int8_t & t, int iRadix)
  {
    SerialPrinter::print<int16_t>(t, iRadix);
  }
  template<> inline void SerialPrinter::println<int8_t>(const int8_t & t, int iRadix)
  {
    SerialPrinter::print<int16_t>(t, iRadix);
  }

  template<> inline void SerialPrinter::print<uint8_t>(const uint8_t & t)
  {
    SerialPrinter::print<uint16_t>(t);
  }
  template<> inline void SerialPrinter::println<uint8_t>(const uint8_t & t)
  {
    SerialPrinter::print<uint16_t>(t);
  }

  template<> inline void SerialPrinter::print<int8_t>(const int8_t & t)
  {
    SerialPrinter::print<int16_t>(t);
  }
  template<> inline void SerialPrinter::println<int8_t>(const int8_t & t)
  {
    SerialPrinter::print<int16_t>(t);
  }

  //specializations: handle cstr pointer
  typedef const char * CHAR_PTR;
  template<> inline void SerialPrinter::print<CHAR_PTR>(const CHAR_PTR & t)
  {
    std::stringstream out;
    out << t;
    printString(out.str().c_str());
  }
  template<> inline void SerialPrinter::println<CHAR_PTR>(const CHAR_PTR & t)
  {
    std::stringstream out;
    out << t;
    printlnString(out.str().c_str());
  }
  template<> inline void SerialPrinter::print<CHAR_PTR>(const CHAR_PTR & t, int iRadix)
  {
    std::stringstream out;
    out << t;
    printString(out.str().c_str());
  }
  template<> inline void SerialPrinter::println<CHAR_PTR>(const CHAR_PTR & t, int iRadix)
  {
    std::stringstream out;
    out << t;
    printlnString(out.str().c_str());
  }
  
  const char * getFloatPrecisionFormat(int iPrecision);
  static const size_t SERIALPRINTER_FLOAT_BUFFER_SIZE = 48;

  //specializations: float
  template<> inline void SerialPrinter::print<float>(const float & t, int iPrecision)
  {
    char buffer[SERIALPRINTER_FLOAT_BUFFER_SIZE];
    sprintf(buffer, getFloatPrecisionFormat(iPrecision), t);
    printString(buffer);
  }
  template<> inline void SerialPrinter::println<float>(const float & t, int iPrecision)
  {
    char buffer[SERIALPRINTER_FLOAT_BUFFER_SIZE];
    sprintf(buffer, getFloatPrecisionFormat(iPrecision), t);
    printlnString(buffer);
  }

  //specializations: double
  template<> inline void SerialPrinter::print<double>(const double & t, int iPrecision)
  {
    char buffer[SERIALPRINTER_FLOAT_BUFFER_SIZE];
    sprintf(buffer, getFloatPrecisionFormat(iPrecision), t);
    printString(buffer);
  }
  template<> inline void SerialPrinter::println<double>(const double & t, int iPrecision)
  {
    char buffer[SERIALPRINTER_FLOAT_BUFFER_SIZE];
    sprintf(buffer, getFloatPrecisionFormat(iPrecision), t);
    printlnString(buffer);
  }

} //testarduino


#endif //SERIALPRINTER_H
