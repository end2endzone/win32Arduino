#ifndef WIN32_ARDUINO_H
#define WIN32_ARDUINO_H

#include <stdio.h>
#include <sstream>
#include <time.h>
#include <stdint.h>

#include "SerialPrinter.h"

namespace testarduino
{
  //realtime millis() support
  static clock_t app_clock_init();
  static double diffclock(clock_t clockEnd,clock_t clockStart);

  //last command support
  const char * getLastCommand();

  //logging support
  void setLogFile(const char * iFilePath);
  void log(const char * iValue);

  //clock hanlding
  enum CLOCK_STRATEGY
  {
    CLOCK_REALTIME = 0,
    CLOCK_SIMULATION,
  };
  void setClockStrategy(CLOCK_STRATEGY iClock);
  CLOCK_STRATEGY getClockStrategy();
  void setMicrosResolution(uint32_t iResolution);
  void setMicrosCounter(uint32_t iCounter);

  //enums to string functions
  const char * toDigitalPinString(uint8_t value);
  const char * toPinModeString(uint8_t value);
  const char * toBitOrderString(uint8_t value);
  const char * toInterruptModeString(uint8_t value);

}

//types definitions
typedef unsigned char byte;

//digital read/write
#define HIGH 1
#define LOW 0

//attachInterrupt
#define CHANGE 1
#define FALLING 2
#define RISING 3

//pin modes
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

//shiftOut definition
#define LSBFIRST 0
#define MSBFIRST 1

void tone(byte iPin, uint16_t freq, uint32_t duration);
void noTone(byte iPin);

extern SerialPrinter Serial;

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
uint8_t digitalRead(uint8_t pin);
void analogWrite(uint8_t pin, uint16_t value);
uint16_t analogRead(uint8_t pin);
void analogReadResolution(uint8_t bits);
void analogWriteResolution(uint8_t bits);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t data);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
uint32_t pulseIn(uint8_t pin, uint8_t digitalState, uint32_t timeout);
uint32_t pulseIn(uint8_t pin, uint8_t digitalState);
uint32_t micros();
uint32_t millis();
void delay(uint32_t value);
void delayMicroseconds(uint16_t value);

template<typename T>
T abs(T x)
{
  if (x < 0)
    return -x;
  else
    return x;
}

template<typename T>
T max(T x, T y)
{
  if (x > y)
    return x;
  else
    return y;
}

template<typename T>
T min(T x, T y)
{
  if (x < y)
    return x;
  else
    return y;
}

template<typename T>
T constrain(T x, T a, T b)
{
  if (x < a)
    return a;
  else if (x > b)
    return b;
  else
    return x;
}

template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
  T value = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return value;
}

//pow(base, exponent)
//sqrt(x)

typedef void (*ISR)();
void attachInterrupt(uint8_t pin, ISR func, uint8_t mode);
void detachInterrupt(uint8_t pin);

extern uint8_t SREG;
void cli();
void noInterrupts();
void interrupts();

//Characters 
bool isAlphaNumeric(int8_t value);
bool isAlpha(int8_t value);
bool isAscii(int8_t value);
bool isWhitespace(int8_t value);
bool isControl(int8_t value);
bool isDigit(int8_t value);
bool isGraph(int8_t value);
bool isLowerCase(int8_t value);
bool isPrintable(int8_t value);
bool isPunct(int8_t value);
bool isSpace(int8_t value);
bool isUpperCase(int8_t value);
bool isHexadecimalDigit(int8_t value);

//Random Numbers
void randomSeed(int16_t value);
void randomSeed(int32_t value);
int32_t random(int32_t min, int32_t max);
int32_t random(int32_t max);

//Bits and Bytes
//-lowByte() 
//-highByte() 
//-bitRead() 
//-bitWrite() 
//-bitSet() 
//-bitClear() 
//-bit() 
template<typename T>
uint8_t lowByte(T x)
{
  const uint8_t * bytes = (uint8_t*)&x;
  uint8_t lowByteIndex = 0;
  return bytes[lowByteIndex];
}

template<typename T>
uint8_t highByte(T x)
{
  const uint8_t * bytes = (uint8_t*)&x;
  uint8_t highByteIndex = sizeof(T)-1;
  return bytes[highByteIndex];
}

template<typename T>
T bitRead(const T & x, T n)
{
  return ( (x & (1<<n)) >> n );
}

template<typename T>
void bitWrite(T & x, T n, T b)
{
  x = (x & ~(1<<n)) | (b<<n);
}

template<typename T>
void bitSet(T & x, T n)
{
  x |= (1<<n);
}

template<typename T>
void bitClear(T & x, T n)
{
  x &= ~((T)1<<n);
}

template<typename T>
T bit(T n)
{
  return ((T)1<<n);
}

#endif //WIN32_ARDUINO_H