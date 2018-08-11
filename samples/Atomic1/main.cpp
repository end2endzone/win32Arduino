#include "Arduino.h"

//include arduino source code
#include "Atomic1.ino"

int main(int argc, char* argv[])
{
  setup();
  loop(); //loop only once, not infinitely

  return 0;
}
