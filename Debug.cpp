#include "Debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

void DebugOut(const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf_s(buffer, ARRAYSIZE(buffer), format, args);
    OutputDebugStringA(buffer);
    va_end(args);
}

void DebugOut(const wchar_t* format, ...)
{
    wchar_t buffer[1024];
    va_list args;
    va_start(args, format);
    _vsnwprintf_s(buffer, ARRAYSIZE(buffer), format, args);
    OutputDebugStringW(buffer);
    va_end(args);
}
