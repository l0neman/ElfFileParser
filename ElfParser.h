#ifndef ELF_PARSER_H
#define ELF_PARSER_H
#include <cstdio>
#include "type/elf.h"

class ElfParser
{
public:
    ElfParser();
    explicit ElfParser(char const* elf_file);
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

    char* string_table_;
    char* symbol_string_table_;

    Elf32_Sym* symbol32_list_;
    Elf64_Sym* symbol64_list_;

    Elf32_Rel* relocation32_list_;
    Elf64_Rel* relocation64_list_;

    bool check_elf();
    void parse_elf_header();
    void parse_section_header_list();
    void parse_string_table();
    void print_section_header_list() const;
    void parse_symbol_string_table();
    void parse_program_header_list();
    void parse_section_list();
    void parse_symbol_table(long offset, size_t size);
    void parse_relocation_table(long offset, size_t size);

    const char* get_string_from_string_table(size_t offset) const;
    const char* get_string_from_symbol_string_table(size_t offset) const;
};

#endif // ELF_PARSER_H
