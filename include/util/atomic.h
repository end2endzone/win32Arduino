#include "arduino.h"

//Helper functions

static inline uint8_t __atomicNoInterrupts(void)
{
  noInterrupts();
  return 0; //initialize loop count to 0
}

static inline uint8_t __atomicInterrupts(void)
{
  interrupts();
  return 0; //initialize loop count to 0
}

static inline uint8_t __atomicRestore(const uint8_t *__s)
{
  SREG = *__s;
  return 1; //set loop count to 1
}

//ATOMIC_BLOCK macro support:

#define ATOMIC_RESTORESTATE uint8_t __atom_sreg_save = SREG
#define ATOMIC_FORCEON uint8_t __atom_sreg_save = 130
static uint8_t __atomic_loop_count = 0;

#define ATOMIC_BLOCK(type) for ( type, __atomic_loop_count = __atomicNoInterrupts(); __atomic_loop_count == 0; __atomic_loop_count = __atomicRestore(&__atom_sreg_save) )

//NONATOMIC_BLOCK macro support:

#define NONATOMIC_RESTORESTATE uint8_t __non_atom_sreg_save = SREG
#define NONATOMIC_FORCEOFF uint8_t __non_atom_sreg_save = 2
static uint8_t __non_atomic_loop_count = 0;

#define NONATOMIC_BLOCK(type) for ( type, __non_atomic_loop_count = __atomicInterrupts(); __non_atomic_loop_count == 0; __non_atomic_loop_count = __atomicRestore(&__non_atom_sreg_save) )
