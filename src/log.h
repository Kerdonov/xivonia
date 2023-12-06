#pragma once

#include "ansi_colors.h"
#include <stdio.h>

#define errorlog(...) { fprintf(stderr, ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET); fprintf(stderr, __VA_ARGS__); }
#define simplelog(...) { printf(ANSI_COLOR_CYAN "LOG: " ANSI_COLOR_RESET); printf(__VA_ARGS__); }
#define warninglog(...) { printf(ANSI_COLOR_YELLOW "WARNING: " ANSI_COLOR_RESET); printf(__VA_ARGS__); }