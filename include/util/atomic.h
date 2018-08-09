#include "Arduino.h"

//Helper functions

uint8_t __tmp; //use for allowing __atomic.enter() and leave() calls within a for loop

//ATOMIC_BLOCK macro support:
#define ATOMIC_RESTORESTATE 0
#define ATOMIC_FORCEON      1
class __ATOMIC
{
public:
  uint8_t strategy;
  uint8_t loopCount;
  uint8_t save;

  uint8_t enter()
  {
    save = SREG;
    noInterrupts();
    return 0;
  }
  uint8_t leave()
  {
    if (strategy == ATOMIC_RESTORESTATE)
      SREG = save;
    else //ATOMIC_FORCEON
      interrupts();
    return 0;
  }
};
__ATOMIC __atomic;

//NONATOMIC_BLOCK macro support:
#define NONATOMIC_RESTORESTATE 0
#define NONATOMIC_FORCEOFF     1
class __NONATOMIC
{
public:
  uint8_t strategy;
  uint8_t loopCount;
  uint8_t save;

  uint8_t enter()
  {
    save = SREG;
    interrupts();
    return 0;
  }
  uint8_t leave()
  {
    if (strategy == NONATOMIC_RESTORESTATE)
      SREG = save;
    else //NONATOMIC_FORCEOFF
      noInterrupts();
    return 0;
  }
};
__NONATOMIC __non_atomic;


#define ATOMIC_BLOCK(type)    for ( __atomic.strategy = type,     __atomic.loopCount = 0,     __tmp = __atomic.enter();     __atomic.loopCount == 0;     __atomic.loopCount++,     __tmp = __atomic.leave() )
#define NONATOMIC_BLOCK(type) for ( __non_atomic.strategy = type, __non_atomic.loopCount = 0, __tmp = __non_atomic.enter(); __non_atomic.loopCount == 0; __non_atomic.loopCount++, __tmp = __non_atomic.leave() )
