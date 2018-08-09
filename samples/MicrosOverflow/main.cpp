#include "Arduino.h"

//include arduino source code
#include "MicrosOverflow.ino"

int main(int argc, char* argv[])
{
  setup();
  loop(); //loop only once, not infinitely

  return 0;
}
