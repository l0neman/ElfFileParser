#include "ElfParser.h"
#include "type/elf.h"
#include "util/Printer.h"

ElfParser::ElfParser(char const* elf_file)
{
    this->elf_file_ = nullptr;
    this->elf_header32_ = Elf32_Ehdr();
    this->elf_header64_ = Elf64_Ehdr();

    const auto f = fopen_s(&this->elf_file_, elf_file, "rb");
    printf("open elf file: %s\n\n", elf_file);
    if (f != 0 || this->elf_file_ == nullptr)
        printf("open elf file error: %s\n", elf_file);
}

ElfParser::~ElfParser()
{
    if (this->elf_file_ != nullptr)
        fclose(this->elf_file_);
}

void ElfParser::parse()
{
    printf(">>>>>>>>>>>> parse elf header <<<<<<<<<<<<\n\n");
    parse_elf_header();
    // todo: other part
}


inline void print_elf_header(Elf32_Ehdr* header) {
    printf("ident:\n");
    Printer::print_char_array(header->e_ident, 16);
    printf("type: %d\n", header->e_type);
    printf("machine: %d\n", header->e_machine);
    printf("version: %d\n", header->e_version);
    printf("entry: %d\n", header->e_entry);
    printf("phoff: %d\n", header->e_phoff);
    printf("shoff: %d\n", header->e_shoff);
    printf("flags: %d\n", header->e_flags);
    printf("ehsize: %d\n", header->e_ehsize);
    printf("phentsize: %d\n", header->e_phentsize);
    printf("phnum: %d\n", header->e_phnum);
    printf("shentsize: %d\n", header->e_shentsize);
    printf("shnum: %d\n", header->e_shnum);
    printf("shstrndx: %d\n", header->e_shstrndx);
}

inline void print_elf_header(Elf64_Ehdr* header) {}

void ElfParser::parse_elf_header()
{
    // todo: 解析 ELF 的 Class，判断是 32 位还是 64 位
    if (0 == fread(&this->elf_header32_, sizeof(Elf32_Ehdr), 1, this->elf_file_)) {
        printf("parse elf header error.\n");
        return;
    }

    print_elf_header(&this->elf_header32_);
}
