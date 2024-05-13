#include "DebugPrint.h"

#define PRINT_BUFFER_SIZE			128
#define ARDUINO_SERIAL_SPEED		57600

void DebugInit(void)
{
#if defined(ARDUINO_PLATFORM) && defined(ARDUINO_SERIAL_DEBUG)
	Serial.begin(ARDUINO_SERIAL_SPEED);
#endif
}

void DebugPrint(const char *format, ...)
{
#if (defined(ARDUINO_PLATFORM_SPI) && defined(ARDUINO_SERIAL_DEBUG)) || defined(MSVC_FT800EMU)
	char buffer[PRINT_BUFFER_SIZE];
	va_list vl;
	va_start(vl, format);
	#ifdef MSVC_FT800EMU
		vprintf(format, vl);
	#else
		vsnprintf(buffer, PRINT_BUFFER_SIZE, format, vl);
		Serial.print(buffer);
	#endif
	va_end(vl);
#endif
}