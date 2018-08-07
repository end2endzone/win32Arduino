# ButtonLibrary

This is a sample arduino library designed to show how to use the [win32Arduino](http://github.com/end2endzone/win32Arduino) library.

The library does not offer any special functionality and only define a single function:

```cpp
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
```

# Usage

The following section shows how to use the library with a code sample:

```cpp
/*
Description:
  Wait for the user to press a button.
  If the button is pressed, the LED turns ON for 1 second and then turn off and the loop continues.
  If the button is not pressed, the LED will 'blip' every 5 seconds.
*/

const uint8_t  BUTTON_PIN = 2;
const uint8_t  LED_PIN = 13;
const uint32_t BUTTON_TIMEOUT = 5000;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  bool pressed = waitForButtonPress(BUTTON_PIN, BUTTON_TIMEOUT);
  if (pressed) {
    //button was pressed
    digitalWrite(LED_PIN, HIGH); //turn on
    delay(1000);
    digitalWrite(LED_PIN, LOW); //turn off
  }
  else {
    //button not pressed (function has timed out after BUTTON_TIMEOUT milliseconds)
    //blip the LED
    digitalWrite(LED_PIN, HIGH); //turn on
    delay(100);
    digitalWrite(LED_PIN, LOW); //turn off
  }
}
```

# Install

N/A

# Testing

The library comes with unit tests for demonstration purpose.

Test are build using the Google Test v1.8.0 framework. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md).  

Test are found in file [TestButtonLibrary.cpp](TestButtonLibrary.cpp).

# Versioning

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/win32Arduino/tags).

# Authors

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/win32Arduino/blob/master/AUTHORS) who participated in this project.

# License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details