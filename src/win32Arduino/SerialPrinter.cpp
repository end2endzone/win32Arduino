#include "arduino.h"

void SerialPrinter::begin(uint16_t baudrate)
{
}

void SerialPrinter::printString(const char * iValue)
{
  printf("%s",  iValue);
  arduino_stub::log(iValue);
}

void SerialPrinter::printlnString(const char * iValue)
{
  printf("%s\n",iValue);
  std::string tmp = iValue;
  tmp.append("\n");
  arduino_stub::log(tmp.c_str());
}

void SerialPrinter::println()
{
  printf("\n");
}
