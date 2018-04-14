#include <vector>

#include <stdarg.h> //for functions with "..." arguments

#include "arduino.h"
#include "IncrementalClockStrategy.h"
#include "RealtimeClockStrategy.h"
#include "stringfunc.h"
#include "nativefunc.h"

static const uint8_t DEFAULT_STATUS_REGISTER = 130;
static const uint8_t DEFAULT_NO_INTERRUPTS_STATUS_REGISTER = 2;
uint8_t SREG = DEFAULT_STATUS_REGISTER;

namespace testarduino
{
  //---------------------------------------------------------------------------
  //                              PINS DATA
  //---------------------------------------------------------------------------

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

  //---------------------------------------------------------------------------
  //                              LOGGING SUPPORT
  //---------------------------------------------------------------------------

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
      fputs("\n", f);
      fclose(f);
    }
    else
    {
      //that is a new log file
      FILE * f = fopen(gLogFile.c_str(), "w");
      fputs(iValue, f);
      fputs("\n", f);
      fclose(f);
    }

    //remember last output file
    gPreviousLogFile = gLogFile;

    //remember last command
    gLastCommand = iValue;
    gLastCommand += "\n";
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

  //---------------------------------------------------------------------------
  //                              CLOCK SUPPORT
  //---------------------------------------------------------------------------

  static IClockStrategy * gClockStrategy = &IncrementalClockStrategy::getInstance();
  void setClockStrategy(IClockStrategy * iClockStrategy)
  {
    gClockStrategy = iClockStrategy;
  }

  IClockStrategy * getClockStrategy()
  {
    return gClockStrategy;
  }

  //---------------------------------------------------------------------------
  //                              ENUM TO STRING
  //---------------------------------------------------------------------------

  const char * toDigitalPinString(uint8_t value)
  {
    if (value == HIGH)
      return "HIGH";
    else if (value == LOW)
      return "LOW";
    return "EXPECTING_HIGH_OR_LOW";
  }

  //pin modes
  const char * toPinModeString(uint8_t value)
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
  const char * toBitOrderString(uint8_t value)
  {
    if (value == MSBFIRST)
      return "MSBFIRST";
    else if (value == LSBFIRST)
      return "LSBFIRST";
    return "EXPECTING_MSBFIRST_OR_LSBFIRST";
  }

  //interrupt modes
  const char * toInterruptModeString(uint8_t value)
  {
    if (value == CHANGE)
      return "CHANGE";
    else if (value == RISING)
      return "RISING";
    else if (value == FALLING)
      return "FALLING";
    return "EXPECTING_CHANGE_RISING_OR_FALLING";
  }

  //---------------------------------------------------------------------------
  //                              PINS VALUE CHANGE
  //---------------------------------------------------------------------------

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

  //---------------------------------------------------------------------------
  //                        FUNCTION CALLBACK SUPPORT
  //---------------------------------------------------------------------------

  struct FunctionCallback
  {
    std::string functionName;
    Callback enter;
    Callback leave;
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

  void attachFunctionCallback(const char * name, Callback enter, Callback leave)
  {
    //allow only 1 callback per function name
    detachFunctionCallback(name);
 
    //add a new callback to the list
    FunctionCallback fCallback;
    fCallback.functionName = name;
    fCallback.enter = enter;
    fCallback.leave = leave;
    gFunctionCallbacks.push_back(fCallback);
  }

  void attachFunctionCallback(const char * name, Callback func)
  {
    attachFunctionCallback(name, func, NULL);
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
      //invoke enter callback
      size_t existingCallback = findFunctionCallback(functionName);
      if (existingCallback != INVALID_FUNCTIONCALLBACK_INDEX)
      {
        FunctionCallback & callback = gFunctionCallbacks[existingCallback];
        if (callback.enter)
        {
          callback.enter();
        }
      }
    }
    ~FunctionCallbackHandler()
    {
      //invoke leave callback
      size_t existingCallback = findFunctionCallback(functionName);
      if (existingCallback != INVALID_FUNCTIONCALLBACK_INDEX)
      {
        FunctionCallback & callback = gFunctionCallbacks[existingCallback];
        if (callback.leave)
        {
          callback.leave();
        }
      }
    }
  private:
    std::string functionName;
  };

  //---------------------------------------------------------------------------
  //                           TIME CALLBACK SUPPORT
  //---------------------------------------------------------------------------
 
  struct TimeCallback
  {
    uint32_t time;
    Callback func;
  };
  typedef std::vector<TimeCallback> TimeCallbackList;
 
  TimeCallbackList gMillisecondsCallbacks;
  TimeCallbackList gMicrosecondsCallbacks;
  static const size_t INVALID_TIMECALLBACK_INDEX = (size_t)-1;

  size_t findTimeCallback(const TimeCallbackList & iList, Callback func)
  {
    for(size_t i=0; i<iList.size(); i++)
    {
      const TimeCallback & fCallback = iList[i];
      if (fCallback.func == func)
        return i;
    }
    return INVALID_TIMECALLBACK_INDEX;
  }

  size_t findTimeCallback(const TimeCallbackList & iList, uint32_t min, uint32_t max)
  {
    for(size_t i=0; i<iList.size(); i++)
    {
      const TimeCallback & fCallback = iList[i];
      if (min <= fCallback.time && fCallback.time <= max)
        return i;
    }
    return INVALID_TIMECALLBACK_INDEX;
  }

  TimeCallbackList findTimeCallbacks(const TimeCallbackList & iList, uint32_t min, uint32_t max)
  {
    TimeCallbackList callbacks;
    for(size_t i=0; i<iList.size(); i++)
    {
      const TimeCallback & fCallback = iList[i];
      if (min <= fCallback.time && fCallback.time <= max)
      {
        callbacks.push_back(fCallback);
      }
    }
    return callbacks;
  }

  void attachMillisecondsCallback(uint32_t time, Callback func)
  {
    TimeCallback c;
    c.time = time;
    c.func = func;
    gMillisecondsCallbacks.push_back(c);
  }
 
  void attachMicrosecondsCallback(uint32_t time, Callback func)
  {
    TimeCallback c;
    c.time = time;
    c.func = func;
    gMicrosecondsCallbacks.push_back(c);
  }
 
  void detachTimeCallback(TimeCallbackList & iList, Callback func)
  {
    size_t index = findTimeCallback(iList, func);
    if (index != INVALID_TIMECALLBACK_INDEX)
    {
      iList.erase(iList.begin()+index);
    }
  }
 
  void detachMillisecondsCallback(Callback func)
  {
    detachTimeCallback(gMillisecondsCallbacks, func);
  }
 
  void detachMicrosecondsCallback(Callback func)
  {
    detachTimeCallback(gMicrosecondsCallbacks, func);
  }
 
  class TimeCallbackHandler
  {
  public:
    TimeCallbackHandler(TimeCallbackList * iList)
    {
      mList = iList;
      mPrevious = 0;
    }
    ~TimeCallbackHandler()
    {
    }
    void invoke(uint32_t newTime)
    {
      //invoke all callback that are configured between mPrevious and newTime
      TimeCallbackList runnableCallbacks = findTimeCallbacks(*mList, mPrevious, newTime);
      mPrevious = newTime; //prevents any recursive calls to invoke()
 
      //remove all runnable callbacks from the list
      for(size_t i=0; i<runnableCallbacks.size(); i++)
      {
        TimeCallback & c = runnableCallbacks[i];
        detachTimeCallback(*mList, c.func);
      }
 
      //now run all callbacks
      for(size_t i=0; i<runnableCallbacks.size(); i++)
      {
        TimeCallback & c = runnableCallbacks[i];
        if (c.func)
          c.func();
      }
    }
  private:
    TimeCallbackList * mList;
    uint32_t mPrevious;
  };
 
  //---------------------------------------------------------------------------

  void reset()
  {
    gLogFile = ""; //logging is disable by default for performance reasons

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

    //callback functions
    gFunctionCallbacks.clear();
    gMillisecondsCallbacks.clear();
    gMicrosecondsCallbacks.clear();
  }

  //force a reset of the library at load time
  bool init()
  {
    reset();
    return true;
  }
  static bool gLibraryInitialized = init();

}

void tone(byte iPin, uint16_t freq, uint32_t duration)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << iPin << "," << freq << "," << duration << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
}

void noTone(byte iPin)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << iPin << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
}

//declare global Serial object
testarduino::SerialPrinter Serial;

//----------------------------------------------------------------------

//https://www.arduino.cc/en/Reference/HomePage

void pinMode(uint8_t pin, uint8_t mode)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << "," << testarduino::toPinModeString(mode) << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << "," << testarduino::toDigitalPinString(value) << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  //update pin state
  if (value == LOW)
    testarduino::setPinDigitalValue(pin, LOW);
  else
    testarduino::setPinDigitalValue(pin, HIGH);
}

uint8_t digitalRead(uint8_t pin)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

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
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << "," << value << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  //update pin state
  testarduino::setPinAnalogValue(pin, value);
}

uint16_t analogRead(uint8_t pin)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  //update pin state
  return testarduino::getPinAnalogValue(pin);
}

void analogReadResolution(uint8_t bits)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << bits << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
}

void analogWriteResolution(uint8_t bits)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << bits << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t data)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << dataPin << "," << clockPin << "," << testarduino::toBitOrderString(bitOrder) << "," << data << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << dataPin << "," << clockPin << "," << testarduino::toBitOrderString(bitOrder) << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  return 0;
}

uint32_t pulseIn(uint8_t pin, uint8_t digitalState, uint32_t timeout)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << "," << testarduino::toDigitalPinString(digitalState) << "," << timeout << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  return 200; //200 usec
}

uint32_t pulseIn(uint8_t pin, uint8_t digitalState)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << "," << testarduino::toDigitalPinString(digitalState) << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  return 200; //200 usec
}

uint32_t micros()
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "();";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
 
  //get current time
  uint32_t time = testarduino::gClockStrategy->micros();
 
  //process any time callbacks attached
  static testarduino::TimeCallbackHandler timeHandler(&testarduino::gMicrosecondsCallbacks);
  timeHandler.invoke(time);
 
  return time;
}
 
uint32_t millis()
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "();";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);
 
  //get current time
  uint32_t time = testarduino::gClockStrategy->millis();
 
  //process any time callbacks attached
  static testarduino::TimeCallbackHandler timeHandler(&testarduino::gMillisecondsCallbacks);
  timeHandler.invoke(time);
 
  return time;
}

void delay(uint32_t value)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << value << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  //based on millis() timing
  uint32_t startTime = millis();
  uint32_t endTime = startTime + value;
  while( millis() < endTime )
  {
  }
}

void delayMicroseconds(uint16_t value)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << value << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  //based on micros() timing
  uint32_t startTime = micros();
  uint32_t endTime = startTime + value;
  while( micros() < endTime )
  {
  }
}

//pow(base, exponent)
//sqrt(x)

void attachInterrupt(uint8_t pin, ISR func, uint8_t mode)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << "," << (void*)func << "," << testarduino::toInterruptModeString(mode) << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  testarduino::pins[pin].interrupt.func = func;
  testarduino::pins[pin].interrupt.mode = mode;
}

void detachInterrupt(uint8_t pin)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << pin << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  testarduino::pins[pin].interrupt.func = NULL;
  testarduino::pins[pin].interrupt.mode = 0;
}

void cli()
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "();";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  noInterrupts();
}

void noInterrupts()
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "();";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  SREG = DEFAULT_NO_INTERRUPTS_STATUS_REGISTER;
}

void interrupts()
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "();";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

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
  return value >= 0 && value <= 31;
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
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << value << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  ::srand(value);
}
void randomSeed(int32_t value)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << value << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  ::srand(value);
}

int32_t random(int32_t min, int32_t max)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << min << "," << max << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  int systemMax = RAND_MAX;
  int value = rand(); //between 0 (inclusive) and RAND_MAX (exclusive).
  int32_t width = max-min;
  value = value%width; //from 0 (inclusive) to width (exclusive)
  value += min;
  return value;
}
int32_t random(int32_t max)
{
  //log function call
  const std::string funcArgs = std::string(__FUNCTION__) << "(" << max << ");";
  testarduino::log(funcArgs.c_str());
 
  //add function callback handler
  testarduino::FunctionCallbackHandler fHandler(__FUNCTION__);

  return random(0, max);
}
