//
//PROGMEM example #3 from https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
//
//

const static char long_str[] PROGMEM = "Hi, I would like to tell you a bit about myself.\n";

void setup()
{
  Serial.begin(9600);
  Serial.print("Write something on the Serial Monitor");
  Serial.print(F("Write something on the Serial Monitor that is stored in FLASH"));
}

void loop()
{
}

