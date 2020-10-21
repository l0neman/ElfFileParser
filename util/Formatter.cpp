#include "Formatter.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

// TODO: 念中野割 0。

char const * Formatter::binary_8_bit(uint8_t n)
{
    char buffer[8 + 1];
    memset(buffer, 0, 8 + 1);
    _itoa_s(n, buffer, 2);
    return buffer;
}

char const* Formatter::binary_16_bit(uint16_t n)
{
    char buffer[16 + 1];
    memset(buffer, 0, 16 + 1);
    _itoa_s(n, buffer, 2);
    return buffer;
}

char const* Formatter::binary_32_bit(uint32_t n)
{
    char buffer[32 + 1];
    memset(buffer, 0, 32 + 1);
    _itoa_s(n, buffer, 2);
    return buffer;
}

char const* Formatter::binary_64_bit(uint64_t n)
{
    char buffer[64 + 1];
    memset(buffer, 0, 64 + 1);
    _itoa_s(n, buffer, 2);
    return buffer;
}
 