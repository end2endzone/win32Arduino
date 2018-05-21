//
//ATOMIC example #1 from https://www.arduino.cc/reference/en/language/variables/variable-scope--qualifiers/volatile/
//Note: The code was modified as the online example does not compile (defines for statements outside of a function)
//
//

// toggles LED when interrupt pin changes state

#include <util/atomic.h> // this library includes the ATOMIC_BLOCK macro.

int pin = 13;
volatile byte state = LOW;

void blink()
{
  state = !state;
}

void setup()
{
  pinMode(pin, OUTPUT);
  attachInterrupt(0, blink, CHANGE);
}

void loop()
{
  Serial.print(F("Beginning loop(). SREG set to "));
  Serial.println(SREG);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    Serial.print(F("Beginning ATOMIC_BLOCK. SREG set to "));
    Serial.println(SREG);

    // code with interrupts blocked (consecutive atomic operations will not get interrupted)
    digitalWrite(pin, state);
    Serial.print(state);
  }
  Serial.print(F("Left ATOMIC_BLOCK. SREG set to "));
  Serial.println(SREG);
}
