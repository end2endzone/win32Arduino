#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include "arduino.h"
#include <sstream>
#include <bitset>

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
        out << std::hex << t;
        break;
      case OCT:
        out << std::oct << t;
        break;
      case BIN:
        {
          std::string allbits = std::bitset<sizeof(T)*8>(t).to_string();
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
        out << std::hex << t;
        break;
      case OCT:
        out << std::oct << t;
        break;
      case BIN:
        {
          std::string allbits = std::bitset<sizeof(T)*8>(t).to_string();
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
  template<> inline void SerialPrinter::print<uint8_t>(const uint8_t & t, int iRadix)
  {
    SerialPrinter::print<uint16_t>(t, iRadix);
  }
  template<> inline void SerialPrinter::println<uint8_t>(const uint8_t & t, int iRadix)
  {
    SerialPrinter::print<uint16_t>(t, iRadix);
  }
  template<> inline void SerialPrinter::print<uint8_t>(const uint8_t & t)
  {
    SerialPrinter::print<uint16_t>(t);
  }
  template<> inline void SerialPrinter::println<uint8_t>(const uint8_t & t)
  {
    SerialPrinter::print<uint16_t>(t);
  }

} //testarduino


#endif //SERIALPRINTER_H
