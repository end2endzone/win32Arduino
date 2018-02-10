#include <stdarg.h>
#include <shlobj.h>
#undef max
#undef min

#include "arduino.h"

namespace testarduino
{
  //millis() support
  static clock_t app_clock_init()
  {
    return ::clock();
  }
  static clock_t gClockAppStartTime = testarduino::app_clock_init();
  
  double clockDiff(clock_t clockEnd, clock_t clockStart)
  {
    static double CLOCKS_PER_MS = CLOCKS_PER_SEC/1000;
    clock_t diffticks=clockEnd-clockStart;
    double diffms=(diffticks)/CLOCKS_PER_MS;
    return diffms;
  }

  //pins data
  static uint16_t pinStates[256] = {0}; //uint16_t to support analog values

  //last command support
  static std::string gLastCommand;
  const char * getLastCommand()
  {
    return gLastCommand.c_str();
  }

  //function logging support
  std::string gLogFile = "arduino.log";
  std::string gPreviousLogFile;
  void setLogFile(const char * iFilePath)
  {
    gLogFile = iFilePath;
  }
  void log(const char * iValue)
  {
    //if no logging required, leave now
    if (gLogFile == "")
      return;

    if (gPreviousLogFile == gLogFile)
    {
      //continue logging to the same file
      FILE * f = fopen(gLogFile.c_str(), "a");
      fputs(iValue, f);
      fclose(f);
    }
    else
    {
      //that is a new log file
      FILE * f = fopen(gLogFile.c_str(), "w");
      fputs(iValue, f);
      fclose(f);
    }

    //remember last output file
    gPreviousLogFile = gLogFile;

    //remember last command
    gLastCommand = iValue;
  }

  //clock handling
  static CLOCK_STRATEGY gClockStrategy = CLOCK_SIMULATION;
  void setClockStrategy(CLOCK_STRATEGY iClockStrategy)
  {
    gClockStrategy = iClockStrategy;
  }

  CLOCK_STRATEGY getClockStrategy()
  {
    return gClockStrategy;
  }

  static uint32_t gMicroResolution = 8; //8 usec resolution (increment for each calls)
  static uint32_t gMicroCounter = 0;

  void setMicrosecondsResolution(uint32_t iResolution)
  {
    gMicroResolution = iResolution;
  }

  void setMicrosecondsCounter(uint32_t iCounter)
  {
    gMicroCounter = iCounter;
  }

}

void tone(byte iPin, uint16_t freq, uint32_t duration)
{
  char buffer[10240];
  sprintf(buffer, "tone(%d,%d,%d);\n", iPin, freq, duration);
  testarduino::log(buffer);
}

void noTone(byte iPin)
{
  char buffer[10240];
  sprintf(buffer, "noTone(%d);\n", iPin);
  testarduino::log(buffer);
}

//declare global Serial object
SerialPrinter Serial;

//----------------------------------------------------------------------

//https://www.arduino.cc/en/Reference/HomePage

//digital read/write
const char * testarduino::toDigitalPinString(uint8_t value)
{
  if (value == HIGH)
    return "HIGH";
  else if (value == LOW)
    return "LOW";
  return "EXPECTING_HIGH_OR_LOW";
}

//pin modes
const char * testarduino::toPinModeString(uint8_t value)
{
  if (value == OUTPUT)
    return "OUTPUT";
  else if (value == INPUT)
    return "INPUT";
  else if (value == INPUT_PULLUP)
    return "INPUT_PULLUP";
  return "EXPECTING_OUTPUT_INPUT_OR_INPUT_PULLUP";
}

//shiftOut definition
const char * testarduino::toBitOrderString(uint8_t value)
{
  if (value == MSBFIRST)
    return "MSBFIRST";
  else if (value == LSBFIRST)
    return "LSBFIRST";
  return "EXPECTING_MSBFIRST_OR_LSBFIRST";
}

void pinMode(uint8_t pin, uint8_t mode)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  const char * modeString = testarduino::toPinModeString(mode);
  sprintf(buffer, "%s(%d, %s);\n", __FUNCTION__, pin, modeString);
  testarduino::log(buffer);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  const char * digitalString = testarduino::toDigitalPinString(value);
  sprintf(buffer, "%s(%d, %s);\n", __FUNCTION__, pin, digitalString);
  testarduino::log(buffer);

  //update pin state
  if (value == LOW)
    testarduino::pinStates[pin] = LOW;
  else
    testarduino::pinStates[pin] = HIGH;
}

uint8_t digitalRead(uint8_t pin)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, pin);
  testarduino::log(buffer);

  //update pin state
  static const uint8_t DIGITAL_LOW = (uint8_t)LOW;
  static const uint8_t DIGITAL_HIGH = (uint8_t)HIGH;
  if (testarduino::pinStates[pin] == 0)
    return DIGITAL_LOW;
  else
    return DIGITAL_HIGH;
}

void analogWrite(uint8_t pin, uint16_t value)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d, %d);\n", __FUNCTION__, pin, (int)value);
  testarduino::log(buffer);

  //update pin state
  testarduino::pinStates[pin] = value;
}

uint16_t analogRead(uint8_t pin)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, pin);
  testarduino::log(buffer);

  //update pin state
  return testarduino::pinStates[pin];
}

void analogReadResolution(uint8_t bits)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, bits);
  testarduino::log(buffer);
}

void analogWriteResolution(uint8_t bits)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, bits);
  testarduino::log(buffer);
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t data)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d, %d, %s, %d);\n", __FUNCTION__, dataPin, clockPin, testarduino::toBitOrderString(bitOrder), data);
  testarduino::log(buffer);
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d, %d, %s);\n", __FUNCTION__, dataPin, clockPin, testarduino::toBitOrderString(bitOrder));
  testarduino::log(buffer);

  return 0;
}

uint32_t pulseIn(uint8_t pin, uint8_t digitalState, uint32_t timeout)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d, %s, %d);\n", __FUNCTION__, pin, testarduino::toDigitalPinString(digitalState), timeout);
  testarduino::log(buffer);

  return 200; //200 usec
}

uint32_t pulseIn(uint8_t pin, uint8_t digitalState)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d, %s);\n", __FUNCTION__, pin, testarduino::toDigitalPinString(digitalState));
  testarduino::log(buffer);

  return 200; //200 usec
}

namespace realtime
{

uint32_t micros()
{
  //based on millis() implementation.

  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "micros");
  testarduino::log(buffer);

  //copy millis() implementation
  //realtime
  clock_t now = ::clock();
  int32_t diffMs = testarduino::clockDiff(now, testarduino::gClockAppStartTime);
  double diffMicros = diffMs * 1000;

  static const uint32_t MAX_MICROS = (uint32_t)0xFFFFFFFF;
  while(diffMicros > (double)MAX_MICROS)
  {
    diffMicros -= (double)(MAX_MICROS);
  }
  uint32_t finalMicros = (uint32_t)diffMicros;
  return finalMicros;
}

uint32_t millis()
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "millis");
  testarduino::log(buffer);

  //realtime
  clock_t now = ::clock();
  double diffMs = testarduino::clockDiff(now, testarduino::gClockAppStartTime);
  uint32_t diffFinal = (uint32_t)diffMs;
  return diffFinal;
}

void delay(uint32_t value)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", "delay", value);
  testarduino::log(buffer);

  //realtime
  Sleep(value);
}

void delayMicroseconds(uint16_t value)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "delayMicroseconds");
  testarduino::log(buffer);

  //realtime
  uint32_t microSeconds = value;
  uint32_t milliSeconds = microSeconds/1000;
  if (milliSeconds == 0)
    milliSeconds = 1;
  Sleep(milliSeconds);
}

}; //namespace realtime

namespace simulation
{

uint32_t micros()
{
  //counter increments
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "micros");
  testarduino::log(buffer);

  testarduino::gMicroCounter += testarduino::gMicroResolution;
  return testarduino::gMicroCounter;
}

uint32_t millis()
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "millis");
  testarduino::log(buffer);

  //based on micro
  testarduino::gMicroCounter += testarduino::gMicroResolution;
  
  uint32_t microSeconds = testarduino::gMicroCounter;
  uint32_t milliSeconds = microSeconds/1000;
  return milliSeconds;
}

void delay(uint32_t value)
{
  //based on millis() timing
  uint32_t startTime = millis();
  uint32_t endTime = startTime + value;
  while( millis() < endTime )
  {
  }
}

void delayMicroseconds(uint16_t value)
{
  //based on micros() timing
  uint32_t startTime = micros();
  uint32_t endTime = startTime + value;
  while( micros() < endTime )
  {
  }
}

}; //namespace simulation

uint32_t micros()
{
  if (testarduino::gClockStrategy == testarduino::CLOCK_SIMULATION)
    return simulation::micros();
  else
    return realtime::micros();
}

uint32_t millis()
{
  if (testarduino::gClockStrategy == testarduino::CLOCK_SIMULATION)
    return simulation::millis();
  else
    return realtime::millis();
}

void delay(uint32_t value)
{
  if (testarduino::gClockStrategy == testarduino::CLOCK_SIMULATION)
    return simulation::delay(value);
  else
    return realtime::delay(value);
}

void delayMicroseconds(uint16_t value)
{
  if (testarduino::gClockStrategy == testarduino::CLOCK_SIMULATION)
    return simulation::delayMicroseconds(value);
  else
    return realtime::delayMicroseconds(value);
}

//pow(base, exponent)
//sqrt(x)

//typedef void (*ISR)();
const char * testarduino::toInterruptModeString(uint8_t value)
{
  if (value == CHANGE)
    return "CHANGE";
  else if (value == RISING)
    return "RISING";
  else if (value == FALLING)
    return "FALLING";
  return "EXPECTING_CHANGE_RISING_OR_FALLING";
}

void attachInterrupt(uint8_t pin, ISR func, uint8_t mode)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d, ISR=0x%x, %s);\n", __FUNCTION__, pin, func, testarduino::toInterruptModeString(mode));
  testarduino::log(buffer);
}

void detachInterrupt(uint8_t pin)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, pin);
  testarduino::log(buffer);
}

static const uint8_t DEFAULT_STATUS_REGISTER = 130;
static const uint8_t DEFAULT_NO_INTERRUPTS_STATUS_REGISTER = 2;
uint8_t SREG = DEFAULT_STATUS_REGISTER;

void cli()
{
  noInterrupts();
}

void noInterrupts()
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", __FUNCTION__);
  testarduino::log(buffer);

  SREG = DEFAULT_NO_INTERRUPTS_STATUS_REGISTER;
}

void interrupts()
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", __FUNCTION__);
  testarduino::log(buffer);

  SREG = DEFAULT_STATUS_REGISTER;
}

/*
Characters 
-isAlphaNumeric() 
-isAlpha() 
-isAscii() 
-isWhitespace() 
-isControl() 
-isDigit() 
-isGraph() 
-isLowerCase() 
-isPrintable() 
-isPunct() 
-isSpace() 
-isUpperCase() 
-isHexadecimalDigit() 
*/
bool isAlpha(int8_t value);
bool isDigit(int8_t value);
bool isAlphaNumeric(int8_t value)
{
  return isAlpha(value) || isDigit(value);
}

bool isAlpha(int8_t value)
{
  bool alpha =  value >= 'A' && value <= 'Z' ||
                value >= 'a' && value <= 'z';
  return alpha;
}

bool isAscii(int8_t value)
{
  return true;  
}

bool isWhitespace(int8_t value)
{
  bool white =  value == ' '  ||
                value == '\t' ||
                value == '\a' ||
                value == '\n';
  return white;  
}

bool isControl(int8_t value)
{
  //http://ascii.cl/control-characters.htm
  return value >=0 && value <= 31;
}

bool isDigit(int8_t value)
{
  bool digit = value >= '0' && value <= '9';
  return digit;
}

bool isGraph(int8_t value)
{
  return false;
}

bool isLowerCase(int8_t value)
{
  return value >= 'a' && value <= 'z';
}

bool isUpperCase(int8_t value);
bool isPunct(int8_t value);
bool isPrintable(int8_t value)
{
  return  isAlphaNumeric(value) ||
          isWhitespace(value) ||
          isAscii(value) ||
          isLowerCase(value) ||
          isUpperCase(value) ||
          isPunct(value);
}

bool isPunct(int8_t value)
{
/*
Symbol ASCII 
! 33 
" 34 
# 35 
$ 36 
% 37 
& 38 
' 39 
( 40 
) 41 
* 42 
+ 43 
, 44 
- 45 
. 46 
/ 47 
: 58 
; 59 
< 60 
= 61 
> 62 
? 63 
 
Symbol ASCII
@ 64
[ 91
\ 92
] 93
^ 94
_ 95
` 96
{ 123
¦ 124
} 125
~ 126
0 48
1 49
2 50
3 51
4 52
5 53
6 54
7 55
8 56
9 57
*/   
  return  (value >=  33 && value <=  47) ||
          (value >=  58 && value <=  64) ||
          (value >=  91 && value <=  96) ||
          (value >= 123 && value <= 126);
}

bool isSpace(int8_t value)
{
  return value == ' ';
}

bool isUpperCase(int8_t value)
{
  return value >= 'A' && value <= 'Z';
}

bool isHexadecimalDigit(int8_t value)
{
  return  isDigit(value) ||
          (value >= 'a' && value <= 'f') ||
          (value >= 'A' && value <= 'F') ;
}

/*
Random Numbers
-randomSeed() 
-random() 
*/
void randomSeed(int16_t value)
{
  ::srand(value);
}
void randomSeed(int32_t value)
{
  ::srand(value);
}

int32_t random(int32_t min, int32_t max)
{
  int systemMax = RAND_MAX;
  int value = rand(); //between 0 (inclusive) and RAND_MAX (exclusive).
  int32_t width = max-min;
  value = value%width; //from 0 (inclusive) to width (exclusive)
  value += min;
  return value;
}
int32_t random(int32_t max)
{
  return random(0, max);
}
