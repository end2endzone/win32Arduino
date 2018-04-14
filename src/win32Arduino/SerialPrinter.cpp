#include "arduino.h"

namespace testarduino
{

  void SerialPrinter::begin(uint16_t baudrate)
  {
  }

  void SerialPrinter::printString(const char * iValue)
  {
    printf("%s",  iValue);
    testarduino::log(iValue);
  }

  void SerialPrinter::printlnString(const char * iValue)
  {
    printf("%s\n",iValue);
    std::string tmp = iValue;
    tmp.append("\n");
    testarduino::log(tmp.c_str());
  }

  void SerialPrinter::println()
  {
    printf("\n");
  }

} //testarduino
