//
//PROGMEM example #2 from https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
//
//

/*
 PROGMEM string demo
 How to store a table of strings in program memory (flash),
 and retrieve them.

 Information summarized from:
 http://www.nongnu.org/avr-libc/user-manual/pgmspace.html

 Setting up a table (array) of strings in program memory is slightly complicated, but
 here is a good template to follow.

 Setting up the strings is a two-step process. First define the strings.
*/

#include <avr/pgmspace.h>
const char string_0[] PROGMEM = "String 0";   // "String 0" etc are strings to store - change to suit.
const char string_1[] PROGMEM = "String 1";
const char string_2[] PROGMEM = "String 2";
const char string_3[] PROGMEM = "String 3";
const char string_4[] PROGMEM = "String 4";
const char string_5[] PROGMEM = "String 5";


// Then set up a table to refer to your strings.

const char* const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5};

char buffer[30];    // make sure this is large enough for the largest string it must hold

void setup()
{
  Serial.begin(9600);
  while(!Serial); // wait for serial port to connect. Needed for native USB
  Serial.println("OK");
}


void loop()
{
  /* Using the string table in program memory requires the use of special functions to retrieve the data.
     The strcpy_P function copies a string from program space to a string in RAM ("buffer").
     Make sure your receiving string in RAM  is large enough to hold whatever
     you are retrieving from program space. */


  for (int i = 0; i < 6; i++)
  {
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i]))); // Necessary casts and dereferencing, just copy.
    Serial.println(buffer);
    delay( 500 );
  }
}

