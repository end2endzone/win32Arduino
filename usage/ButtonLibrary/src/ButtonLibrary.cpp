#include "Arduino.h"
#include "ButtonLibrary.h"

bool waitForButtonPress(uint8_t buttonPin, uint32_t timeout) {
  unsigned long start = millis();
  while( millis() - start < timeout )
  {
    //look for button state
    int buttonValue = digitalRead(buttonPin);
    if (buttonValue == LOW)
      return true;
  }
  //timeout
  return false;
}
