#ifndef ELF_PARSER_H
#define ELF_PARSER_H
#include <cstdio>
#include "type/elf.h"


class ElfParser {
public:
    ElfParser(char const* elf_file);
    ~ElfParser();
    void parse();
private:
    FILE* elf_file_;
    Elf32_Ehdr elf_header32_;
    Elf64_Ehdr elf_header64_;
    void parse_elf_header();
};

#endif // ELF_PARSER_H