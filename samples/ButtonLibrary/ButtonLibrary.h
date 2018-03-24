#ifndef BUTTONLIBRARY_H
#define BUTTONLIBRARY_H

#include "Arduino.h"

#define BUTTON_LIBRARY_VERSION "1.0.0"

/********************************************************************************
* Description:
*   Wait for the user to press a button (wait for a pin to go from HIGH to LOW)
*   The function times out if the button is never pressed.
*
* Parameters:
*   buttonPin:  The pin to watch
*   timeout:    The maximum time to wait in milliseconds before the function time out
*
* Returns:
*   The function return true if a button is pressed. Returns false otherwise.
********************************************************************************/
bool waitForButtonPress(uint8_t buttonPin, uint32_t timeout);

#endif
