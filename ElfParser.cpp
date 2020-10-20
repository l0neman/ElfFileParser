#include "ElfParser.h"
#include "type/elf.h"

ElfParser::ElfParser(char const* elf_file)
{
    this->elf_file_ = fopen(elf_file, "rb");
    printf("open elf file: %s\n\n", elf_file);
    if (this->elf_file_ == nullptr)
        printf("open elf file error: %s\n", elf_file);
}

ElfParser::~ElfParser()
{
    if(this->elf_file_ != nullptr)
        fclose(this->elf_file_);
}

void ElfParser::parse()
{
}
