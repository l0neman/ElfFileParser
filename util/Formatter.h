#ifndef FORMAT_H_
#define FORMAT_H_

#include <cstdint>

class Formatter
{
public:
    static char const* binary_8_bit(uint8_t n);
    static char const* binary_16_bit(uint16_t n);
    static char const* binary_32_bit(uint32_t n);
    static char const* binary_64_bit(uint64_t n);
};

#endif // !FORMAT_H_
