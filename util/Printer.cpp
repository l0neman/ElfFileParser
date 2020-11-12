#include "Printer.h"
#include "Formatter.h"
#include <iostream>

void Printer::print_ushort_hex_array(uint16_t const* array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        printf("%.4x ", array[i]);

    printf("\n");
}

void Printer::print_int_array(int const* array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        printf("%d ", array[i]);

    printf("\n");
}

void Printer::print_hex_array(uint8_t const* array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        printf("%.2x ", array[i]);

    printf("\n");
}

void Printer::print_hex_array2(uint16_t const* array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        printf("%.4x ", array[i]);

    printf("\n");
}

void Printer::print_char_array(unsigned char const* array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        printf("%.2x ", array[i]);

    printf("\n");
}
