#ifndef ELF_PARSER_H
#define ELF_PARSER_H
#include <cstdio>
#include "type/elf.h"


class ElfParser
{
public:
    ElfParser(char const* elf_file);
    ~ElfParser(); 
    void parse();
private:
    FILE* elf_file_;
    uint8_t elf_bit_;
    Elf32_Ehdr elf_header32_{};
    Elf64_Ehdr elf_header64_{};

    Elf32_Phdr* program_header32_array_;
    Elf64_Ehdr* program_header64_array_;
     
    bool check_elf();
    void parse_elf_header();
    void parse_program_header();
};

#endif // ELF_PARSER_H