#ifndef ARDUINO_PGMSPACE_H
#define ARDUINO_PGMSPACE_H

#include <string>
#include <cstring> //for strlen()
#include <string.h> //for memcmp()

//
// Handle functions and macros that allows storing data in flash (program) memory instead of SRAM.
// See http://www.arduino.cc/reference/en/language/variables/utilities/progmem/ for details
//
 
#define PROGMEM
 
#define F(expr) expr
#define PROGMEM
#define PGM_P const char *
#define PGM_VOID_P const void *
#define PSTR(s) (s)
 
#define memccpy_P      memccpy
#define memcmp_P       memcmp
#define memcpy_P       memcpy
#define memmem_P       memmem
#define printf_P       printf
#define snprintf_P     snprintf
#define sprintf_P      sprintf
#define strcasecmp_P   strcasecmp
#define strcat_P       strcat
#define strcmp_P       strcmp
#define strcpy_P       strcpy
#define strlen_P       strlen
#define strncasecmp_P  strncasecmp
#define strncat_P      strncat
#define strncmp_P      strncmp
#define strncpy_P      strncpy
#define strnlen_P      strnlen
#define strstr_P       strstr
#define vsnprintf_P    vsnprintf
 
template <typename T>
uint8_t pgm_read_byte(const T * pointer)
{
  const uint8_t * buffer = reinterpret_cast<const uint8_t *>(pointer);
  return buffer[0];
}
 
template <typename T>
uint16_t pgm_read_word(const T * pointer)
{
  const uint16_t * buffer = reinterpret_cast<const uint16_t *>(pointer);
  return buffer[0];
}
 
template <typename T>
uint32_t pgm_read_dword(const T * pointer)
{
  const uint32_t * buffer = reinterpret_cast<const uint32_t *>(pointer);
  return buffer[0];
}
 
template <typename T>
float pgm_read_float(const T * pointer)
{
  const float * buffer = reinterpret_cast<const float *>(pointer);
  return buffer[0];
}
 
//All *_near and *_far functions provides the same functionnality as their counterpart.
#define pgm_read_byte_near(expr)  pgm_read_byte(expr)
#define pgm_read_word_near(expr)  pgm_read_word(expr)
#define pgm_read_dword_near(expr) pgm_read_dword(expr)
#define pgm_read_float_near(expr) pgm_read_float(expr)
#define pgm_read_ptr_near(expr)   pgm_read_ptr(expr)
#define pgm_read_byte_far(expr)   pgm_read_byte(expr)
#define pgm_read_word_far(expr)   pgm_read_word(expr)
#define pgm_read_dword_far(expr)  pgm_read_dword(expr)
#define pgm_read_float_far(expr)  pgm_read_float(expr)
#define pgm_read_ptr_far(expr)    pgm_read_ptr(expr)

#endif //ARDUINO_PGMSPACE_H
