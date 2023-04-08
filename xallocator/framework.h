#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
// Pliki nagłówkowe systemu Windows
#include <windows.h>
#endif
#ifdef XALLOCATOR_EXPORTS
#define XALLOCATOR_API __declspec(dllexport)
#else
#define XALLOCATOR_API __declspec(dllimport)
#endif