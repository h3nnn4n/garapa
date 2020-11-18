#pragma once

#include "types.h"

void      emulation_loop(_context *context);
_context *build_emulation_context(int argc, char **argv);
