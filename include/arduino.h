#ifndef WIN32ARDUINO_H
#define WIN32ARDUINO_H

#include <stdint.h>   //for uintxx_t, intxx_t

#include "SerialPrinter.h"
#include "IClockStrategy.h"
#include "avr/pgmspace.h"

namespace testarduino
{
  ///<summary>
  ///Sets the analog value of a pin for tests.
  ///</summary>
  ///<param name="pin">The pin number</param>
  ///<param name="value">The new analog value of the pin</param>
  void setPinAnalogValue(const uint8_t & pin, const uint16_t & value);

  ///<summary>
  ///Reads the analog value of a pin.
  ///</summary>
  ///<param name="pin">The pin number</param>
  ///<returns>Returns the internal analog value of the pin.</returns>
  uint16_t getPinAnalogValue(const uint8_t & pin);

  ///<summary>
  ///Sets the digital value of a pin for tests.
  ///</summary>
  ///<param name="pin">The pin number</param>
  ///<param name="value">The new digital value of the pin</param>
  void setPinDigitalValue(const uint8_t & pin, const uint16_t & value);

  ///<summary>
  ///Reads the digital value of a pin.
  ///</summary>
  ///<param name="pin">The pin number</param>
  ///<returns>Returns the internal digital value of the pin.</returns>
  uint16_t getPinDigitalValue(const uint8_t & pin);

  ///<summary>
  ///Resets all internal states to their default values.
  ///</summary>
  void reset();

  //last command support
  ///<summary>
  ///Provides the last arduino command that was called.
  ///</summary>
  ///<returns>Returns a string which describes the last arduino command that was called.</returns>
  const char * getLastCommand();

  //logging support
  ///<summary>
  ///Defines the path of the log file. See also function log()
  ///</summary>
  void setLogFile(const char * iFilePath);

  ///<summary>
  ///Adds the given arguments to the log file defined by setLogFile().
  ///</summary>
  ///<param name="iFormat">The format of the arguments. (same as printf format)</param>
  void log(const char * iFormat, ...);

  ///<summary>
  ///Set the desired clock strategy.
  ///</summary>
  ///<param name="iClock">The new value for the clock strategy.</param>
  void setClockStrategy(IClockStrategy * iClockStrategy);

  ///<summary>
  ///Provides the selected clock strategy.
  ///</summary>
  ///<returns>Returns the selected clock strategy.</returns>
  IClockStrategy * getClockStrategy();

  //enums to string functions
  const char * toDigitalPinString(uint8_t value);
  const char * toPinModeString(uint8_t value);
  const char * toBitOrderString(uint8_t value);
  const char * toInterruptModeString(uint8_t value);

  ///<summary>
  ///Type definition for function callbacks.
  ///</summary>
  typedef void (*Callback)();

  ///<summary>
  ///Attach a function callback that is run when the specified function is executed.
  ///</summary>
  ///<param name="name">The name of the function to monitor.</param>
  ///<param name="enter">Pointer to a callback function that is run when entering the specified function.</param>
  ///<param name="leave">Pointer to a callback function that is run when leaving  the specified function.</param>
  void attachFunctionCallback(const char * name, Callback enter, Callback leave);
 
  ///<summary>
  ///Attach a function callback that is run when the specified function is executed.
  ///</summary>
  ///<param name="name">The name of the function to monitor.</param>
  ///<param name="func">Pointer to a callback function that is run when entering the specified function.</param>
  void attachFunctionCallback(const char * name, Callback func);
 
  ///<summary>
  ///Detach any function callback of the specified function.
  ///</summary>
  ///<param name="name">The name of the function to monitor.</param>
  void detachFunctionCallback(const char * name);

  ///<summary>
  ///Attach a function callback that is run at a specific time.
  ///</summary>
  ///<remarks>The callback is only evaluated when a library or a program is running the millis() arduino function.</remarks>
  ///<param name="time">The time in milliseconds at which the callback must be run.</param>
  ///<param name="func">Pointer to a callback function that is run at a specific time.</param>
  void attachMillisecondsCallback(uint32_t time, Callback func);
 
  ///<summary>
  ///Detach a function callback that is run at a specific time.
  ///</summary>
  ///<param name="func">Pointer to a callback function that is run at a specific time.</param>
  void detachMillisecondsCallback(Callback func);
 
  ///<summary>
  ///Attach a function callback that is run at a specific time.
  ///</summary>
  ///<remarks>The callback is only evaluated when a library or a program is running the micros() arduino function.</remarks>
  ///<param name="time">The time in milliseconds at which the callback must be run.</param>
  ///<param name="func">Pointer to a callback function that is run at a specific time.</param>
  void attachMicrosecondsCallback(uint32_t time, Callback func);
 
  ///<summary>
  ///Detach a function callback that is run at a specific time.
  ///</summary>
  ///<param name="func">Pointer to a callback function that is run at a specific time.</param>
  void detachMicrosecondsCallback(Callback func);
 
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

//math definitions
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

void tone(byte iPin, uint16_t freq, uint32_t duration);
void noTone(byte iPin);

extern testarduino::SerialPrinter Serial;

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

//math macros
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
  T value = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return value;
}

//pow(base, exponent)
//sqrt(x)

typedef void (*ISR)(); //interrupt service routine
void attachInterrupt(uint8_t pin, ISR func, uint8_t mode);
void detachInterrupt(uint8_t pin);

extern uint8_t SREG;
void cli(); //same as noInterrupts()
void sei(); //same as interrupts()
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
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

//bit manipulation macros
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))

#endif //WIN32ARDUINO_H
