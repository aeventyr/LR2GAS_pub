#pragma once

#include <cstdio>

#ifdef _CONSOLE

#define console_log(...) fprintf(stderr, __VA_ARGS__)

#else

#define console_log(...) 

#endif

