#pragma once

#include <iostream>

// Comment this out if you don't want any output.
#define OUTPUT_ENABLED

#ifdef OUTPUT_ENABLED
#define OUTPUT(out) (std::cout << out << std::endl)
#else
#define OUTPUT(out)
#endif