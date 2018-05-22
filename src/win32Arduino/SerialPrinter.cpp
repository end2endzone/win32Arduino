#include "arduino.h"
#include <cstdio>

namespace testarduino
{

  void SerialPrinter::begin(uint16_t baudrate)
  {
  }

  void SerialPrinter::begin(int16_t baudrate)
  {
  }

  void SerialPrinter::begin(int baudrate)
  {
  }

  void SerialPrinter::printString(const char * iValue)
  {
    std::printf("%s", iValue);
    testarduino::log(iValue);
  }

  void SerialPrinter::printlnString(const char * iValue)
  {
    std::string tmp = iValue;
    tmp.append("\n");
    std::printf("%s",tmp.c_str());

    testarduino::log(iValue); //do not need the added \n since _log() will add a line separator after logging the value
  }

  void SerialPrinter::println()
  {
    std::printf("\n");
    testarduino::log(""); //do not need the added \n since _log() will add a line separator after logging the value
  }

  const char * getFloatPrecisionFormat(int iPrecision)
  {
    static const char * gPrecisions[] = {
      "%.0f",   "%.1f",  "%.2f",  "%.3f",  "%.4f",  "%.5f",  "%.6f",  "%.7f",  "%.8f",  "%.9f",
      "%.10f", "%.11f", "%.12f", "%.13f", "%.14f", "%.15f", "%.16f", "%.17f", "%.18f", "%.19f",
    };
    static const int NUM_PRECISIONS = sizeof(gPrecisions)/sizeof(gPrecisions[0]);
    static const int MAX_PRECISION = NUM_PRECISIONS-1;

    if (iPrecision >= MAX_PRECISION)
      iPrecision = MAX_PRECISION;

    return gPrecisions[iPrecision];
  }

} //testarduino
