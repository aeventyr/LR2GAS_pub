#pragma once

#include <cstdio>

#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING

#define console_log(...) fprintf(stderr, __VA_ARGS__)

#else

#define console_log(...) 

#endif

