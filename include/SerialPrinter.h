#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include "arduino.h"
#include <sstream>

namespace testarduino
{
  class SerialPrinter
  {
  public:

    static void begin(uint16_t baudrate);
    static void printString(const char * iValue);
    static void printlnString(const char * iValue);

    //enable implicit conversion
    //allow code like `while (!Serial);`
    operator bool() const { return true; }

    //Note: http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
    template <class T>
    inline void print(const T & t, int /*iRadix*/)
    {
      std::stringstream out;
      out << t;
      printString(out.str().c_str());
    }

    template <class T>
    inline void println(const T & t, int /*iRadix*/)
    {
      std::stringstream out;
      out << t;
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
  template<> inline void SerialPrinter::print<uint8_t>(const uint8_t & t, int /*iRadix*/)
  {
    std::stringstream out;
    out << (int)t;
    printString(out.str().c_str());
  }
  template<> inline void SerialPrinter::println<uint8_t>(const uint8_t & t, int /*iRadix*/)
  {
    std::stringstream out;
    out << (int)t;
    printlnString(out.str().c_str());
  }
  template<> inline void SerialPrinter::print<uint8_t>(const uint8_t & t)
  {
    std::stringstream out;
    out << (int)t;
    printString(out.str().c_str());
  }
  template<> inline void SerialPrinter::println<uint8_t>(const uint8_t & t)
  {
    std::stringstream out;
    out << (int)t;
    printlnString(out.str().c_str());
  }

} //testarduino


#endif //SERIALPRINTER_H
