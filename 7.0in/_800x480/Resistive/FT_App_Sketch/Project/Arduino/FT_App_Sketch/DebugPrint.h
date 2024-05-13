#include "FT_PLATFORM.h"

#ifndef _ARDUINO_PRINT_
#define _ARDUINO_PRINT_

void DebugInit(void);
void DebugPrint(const char *format, ...);

#endif /*_ARDUINO_PRINT_*/