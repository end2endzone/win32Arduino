#include <vector>

#include <stdarg.h> //for ..., Sleep()
#include <shlobj.h> //for ..., Sleep()
#undef max
#undef min

#include "arduino.h"
#include "IncrementalClockStrategy.h"
#include "RealtimeClockStrategy.h"

static const uint8_t DEFAULT_STATUS_REGISTER = 130;
static const uint8_t DEFAULT_NO_INTERRUPTS_STATUS_REGISTER = 2;
uint8_t SREG = DEFAULT_STATUS_REGISTER;

namespace testarduino
{
  static const uint16_t MAX_PIN_VALUE = 1023;

  //pins data
  struct Interrupt
  {
    ISR func;
    uint8_t mode; //CHANGE, FALLING, RISING
  };

  //define PIN
  struct PIN_REGISTRY
  {
    uint16_t value; //10 bits to support analog values
    Interrupt interrupt;
  };

  static const size_t NUM_PINS = 256;
  static PIN_REGISTRY pins[NUM_PINS] = {0};

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
  void _log(const char * iValue)
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
  void log(const char * iFormat, ...)
  {
    std::string formattedStr;

    va_list args;
    va_start(args, iFormat);

    static const int BUFFER_SIZE = 10240;
    char buffer[BUFFER_SIZE];
    buffer[0] = '\0';
    vsprintf_s(buffer, BUFFER_SIZE, iFormat, args);
    formattedStr = buffer;

    va_end (args);

    _log(formattedStr.c_str());
  }

  //clock handling
  static IClockStrategy * gClockStrategy = &IncrementalClockStrategy::getInstance();
  void setClockStrategy(IClockStrategy * iClockStrategy)
  {
    gClockStrategy = iClockStrategy;
  }

  IClockStrategy * getClockStrategy()
  {
    return gClockStrategy;
  }

  void setPinAnalogValue(const uint8_t & pin, const uint16_t & value)
  {
    uint16_t newValue = value;
    newValue = (newValue)%(1<<10);  //limit pin values to 10 bits

    uint16_t oldDigitalValue = getPinDigitalValue(pin);
    pins[pin].value = newValue; //update existing value
    uint16_t newDigitalValue = getPinDigitalValue(pin);

    //process interrupts
    if (SREG == DEFAULT_STATUS_REGISTER && pins[pin].interrupt.func)
    {
      bool low2high = (oldDigitalValue == LOW && newDigitalValue == HIGH);
      bool high2low = (oldDigitalValue == HIGH && newDigitalValue == LOW);
      if (pins[pin].interrupt.mode == CHANGE && (low2high || high2low))
        pins[pin].interrupt.func();
      else if (pins[pin].interrupt.mode == RISING && low2high)
        pins[pin].interrupt.func();
      else if (pins[pin].interrupt.mode == FALLING && high2low)
        pins[pin].interrupt.func();
    }
  }

  uint16_t getPinAnalogValue(const uint8_t & pin)
  {
    return pins[pin].value;
  }

  void setPinDigitalValue(const uint8_t & pin, const uint16_t & value)
  {
    if (value == LOW)
      setPinAnalogValue(pin, 0);
    else
      setPinAnalogValue(pin, MAX_PIN_VALUE);
  }

  uint16_t getPinDigitalValue(const uint8_t & pin)
  {
    uint16_t value = getPinAnalogValue(pin);
    if (value == 0)
      return LOW;
    else
      return HIGH;
  }

  enum FunctionAttachMode
  {
    PRECALL,
    POSTCALL,
  };
 
  struct FunctionCallback
  {
    std::string functionName;
    Callback func;
    FunctionAttachMode mode;
  };
  typedef std::vector<FunctionCallback> FunctionCallbackList;
  FunctionCallbackList gFunctionCallbacks;
 static const size_t INVALID_FUNCTIONCALLBACK_INDEX = (size_t)-1;
 
  size_t findFunctionCallback(const std::string & iFunctionName)
  {
    for(size_t i=0; i<gFunctionCallbacks.size(); i++)
    {
      FunctionCallback & fCallback = gFunctionCallbacks[i];
      if (fCallback.functionName == iFunctionName)
        return i;
    }
    return INVALID_FUNCTIONCALLBACK_INDEX;
  }
 
  size_t findFunctionCallback(const std::string & iFunctionName, FunctionAttachMode mode)
  {
    size_t index = findFunctionCallback(iFunctionName);
    if (index != INVALID_FUNCTIONCALLBACK_INDEX)
    {
      FunctionCallback & fCallback = gFunctionCallbacks[index];
      if (fCallback.mode == mode)
        return index;
    }
    return INVALID_FUNCTIONCALLBACK_INDEX;
  }
 
  void invokeFunctionCallback(const std::string & iFunctionName, FunctionAttachMode mode)
  {
    size_t existingCallback = findFunctionCallback(iFunctionName, mode);
    if (existingCallback != INVALID_FUNCTIONCALLBACK_INDEX)
    {
      FunctionCallback & fCallback = gFunctionCallbacks[existingCallback];
      fCallback.func();
    }
  }
 
  void attachFunctionCallback(const char * name, Callback func, FunctionAttachMode mode)
  {
    //allow only 1 callback per function name
   size_t existingCallback = findFunctionCallback(name);
    if (existingCallback != INVALID_FUNCTIONCALLBACK_INDEX)
    {
      FunctionCallback & fCallback = gFunctionCallbacks[existingCallback];
     
      //replace old function by new one
      fCallback.func = func;
      fCallback.mode = mode;
    }
    else
    {
      //add a new callback to the list
      FunctionCallback fCallback;
      fCallback.functionName = name;
      fCallback.func = func;
      fCallback.mode = mode;
      gFunctionCallbacks.push_back(fCallback);
    }
  }
 
  void attachPreFunctionCallback(const char * name, Callback func)
  {
    attachFunctionCallback(name, func, PRECALL);
  }
 
  void attachPostFunctionCallback(const char * name, Callback func)
  {
    attachFunctionCallback(name, func, POSTCALL);
  }
 
  void detachFunctionCallback(const char * name)
  {
    size_t existingCallback = findFunctionCallback(name);
    if (existingCallback != INVALID_FUNCTIONCALLBACK_INDEX)
    {
      gFunctionCallbacks.erase(gFunctionCallbacks.begin() + existingCallback);
    }
  }
 
  class FunctionCallbackHandler
  {
  public:
    FunctionCallbackHandler(const char * iFunctionName)
    {
      functionName = iFunctionName;
 
      //invoke PRECALL mode
      invokeFunctionCallback(functionName, PRECALL);
    }
    ~FunctionCallbackHandler()
    {
      //destructor. must invoke POSTCALL mode
      invokeFunctionCallback(functionName, POSTCALL);
    }
  private:
    std::string functionName;
  };
 
  void reset()
  {
    gLogFile = "arduino.log";

    //reset clock
    IncrementalClockStrategy & clock = IncrementalClockStrategy::getInstance();
    clock.setMicrosecondsResolution(1);
    clock.setMicrosecondsCounter(0);
    gClockStrategy = &clock;

    //reset pins
    for(size_t i=0; i<NUM_PINS; i++)
    {
      pins[i].value = 0;
      pins[i].interrupt.func = 0;
      pins[i].interrupt.mode = 0;
    }
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
    testarduino::setPinDigitalValue(pin, LOW);
  else
    testarduino::setPinDigitalValue(pin, HIGH);
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
  if (testarduino::getPinDigitalValue(pin) == 0)
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
  testarduino::setPinAnalogValue(pin, value);
}

uint16_t analogRead(uint8_t pin)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, pin);
  testarduino::log(buffer);

  //update pin state
  return testarduino::getPinAnalogValue(pin);
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

uint32_t micros()
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "micros");
  testarduino::log(buffer);

  return testarduino::gClockStrategy->micros();
}

uint32_t millis()
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s();\n", "millis");
  testarduino::log(buffer);

  return testarduino::gClockStrategy->millis();
}

void delay(uint32_t value)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", "delay", value);
  testarduino::log(buffer);

  //based on millis() timing
  uint32_t startTime = testarduino::gClockStrategy->millis();
  uint32_t endTime = startTime + value;
  while( testarduino::gClockStrategy->millis() != endTime )
  {
  }
}

void delayMicroseconds(uint16_t value)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", "delayMicroseconds", value);
  testarduino::log(buffer);

  //based on micros() timing
  uint32_t startTime = testarduino::gClockStrategy->micros();
  uint32_t endTime = startTime + value;
  while( testarduino::gClockStrategy->micros() != endTime )
  {
  }
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

  testarduino::pins[pin].interrupt.func = func;
  testarduino::pins[pin].interrupt.mode = mode;
}

void detachInterrupt(uint8_t pin)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%s(%d);\n", __FUNCTION__, pin);
  testarduino::log(buffer);

  testarduino::pins[pin].interrupt.func = NULL;
  testarduino::pins[pin].interrupt.mode = 0;
}

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
� 124
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
