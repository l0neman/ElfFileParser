#ifndef PRINTER_H
#define PRINTER_H
#include <cstdint>

class Printer
{
public:
    static void print_ushort_hex_array(uint16_t const* array, const unsigned int size);
    static void print_int_array(int const* array, const unsigned int size);
    static void print_hex_array(uint8_t const* array, const unsigned int size);
    static void print_hex_array2(uint16_t const* array, const unsigned int size);
    static void print_char_array(unsigned char const* array, const unsigned int size);
};

#endif // !PRINTER_H