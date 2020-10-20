#include <cstdio>
#ifndef ELF_PARSER_H
#define ELF_PARSER_H

class ElfParser {
public:
    ElfParser(char const* elf_file);
    ~ElfParser();
    void parse();
private:
    FILE* elf_file_;
};

#endif // ELF_PARSER_H