#include "arduino.h"
#include <cstdlib>

namespace testarduino
{

  void SerialPrinter::begin(uint16_t baudrate)
  {
  }

  void SerialPrinter::printString(const char * iValue)
  {
    std::printf("%s",  iValue);
    testarduino::log(iValue);
  }

  void SerialPrinter::printlnString(const char * iValue)
  {
    std::printf("%s\n",iValue);
    std::string tmp = iValue;
    tmp.append("\n");
    testarduino::log(tmp.c_str());
  }

  void SerialPrinter::println()
  {
    std::printf("\n");
  }

} //testarduino
