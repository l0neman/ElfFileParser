#ifndef ELF_PARSER_H
#define ELF_PARSER_H
#include <cstdio>
#include "type/elf.h"


class ElfParser
{
public:
    ElfParser();
    ElfParser(char const* elf_file);
    ~ElfParser(); 
    void parse();
private:
    FILE* elf_file_;
    uint8_t elf_bit_;

    Elf32_Ehdr elf_header32_{};
    Elf64_Ehdr elf_header64_{};

    Elf32_Phdr* program_header32_list_;
    Elf64_Phdr* program_header64_list_;

    Elf32_Shdr* section_header32_list_;
    Elf64_Shdr* section_header64_list_;
     
    bool check_elf();
    void parse_elf_header();
    void parse_section_header_list();
    void parse_section_header(long offset, size_t index);
    void parse_program_header_list();
    void parse_program_header(long offset, size_t index);
};

#endif // ELF_PARSER_H