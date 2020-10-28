#include "ElfParser.h"

#include <cstring>

#include "type/elf.h"
#include "util/Printer.h"
#include <typeinfo>


ElfParser::ElfParser(char const* elf_file)
{
    this->elf_file_ = nullptr;
    this->elf_bit_ = 0;
    this->elf_header32_ = Elf32_Ehdr();
    this->elf_header64_ = Elf64_Ehdr();

    this->program_header32_array_ = nullptr;
    this->program_header64_array_ = nullptr;

    const auto f = fopen_s(&this->elf_file_, elf_file, "rb");
    printf("open elf file: %s\n\n", elf_file);
    if (f != 0 || this->elf_file_ == nullptr)
        printf("open elf file error: %s\n", elf_file);
}

ElfParser::~ElfParser()
{
    if (this->elf_file_ != nullptr) {
        printf("close elf file.\n");
        fclose(this->elf_file_);
    }

    if(this->program_header32_array_ != nullptr)
    {
        printf("delete program header 32 array.\n");
        delete[] this->program_header32_array_;
        this->program_header32_array_ = nullptr;
    }

    if (this->program_header64_array_ != nullptr)
    {
        printf("delete program header 64 array.\n");
        delete[] this->program_header64_array_;
        this->program_header64_array_ = nullptr;
    }
}

void ElfParser::parse()
{
    printf(">>>>>>>>>>>> parse elf header <<<<<<<<<<<<\n\n");
    uint8_t bit;
    if (!check_elf()) {
        printf("it is not a elf file.\n");
        return;
    }

    parse_elf_header();
    parse_program_header();
}

template <typename T = Elf32_Ehdr>
void print_elf_header(T* header)
{
    printf("ident: ");
    Printer::print_char_array(header->e_ident, 16);
    printf("type: %d\n", header->e_type);
    printf("machine: %d\n", header->e_machine);
    printf("version: %d\n", header->e_version);
    if (typeid(T) == typeid(Elf32_Ehdr))
    {
        printf("entry: %u\n", header->e_entry);
        printf("phoff: %u\n", header->e_phoff);
        printf("shoff: %u\n", header->e_shoff);
    }
    else // typeid(T) == typeid(Elf64_Ehdr)
    {
        printf("entry: %llu\n", header->e_entry);
        printf("phoff: %llu\n", header->e_phoff);
        printf("shoff: %llu\n", header->e_shoff);
    }

    printf("flags: %d\n", header->e_flags);
    printf("ehsize: %d\n", header->e_ehsize);
    printf("phentsize: %d\n", header->e_phentsize);
    printf("phnum: %d\n", header->e_phnum);
    printf("shentsize: %d\n", header->e_shentsize);
    printf("shnum: %d\n", header->e_shnum);
    printf("shstrndx: %d\n", header->e_shstrndx);
}

bool ElfParser::check_elf()
{
    unsigned char elf_ident[16] = { 0 };
    if (0 == fread(elf_ident, sizeof(char), 16, this->elf_file_))
    {
        printf("check elf error: read error");
        return false;
    }

    if (memcmp(elf_ident, ElfMagic, strlen(ElfMagic)) != 0)
        return false;

    char elf_type[10] = "ERROR";
    switch (elf_ident[4])
    {
    case ELFCLASSNONE:
        strcpy_s(elf_type, "invalid");
        break;
    case ELFCLASS32:
        strcpy_s(elf_type, "ELF32");
        this->elf_bit_ = 32;
        break;
    case ELFCLASS64:
        strcpy_s(elf_type, "ELF64");
        this->elf_bit_ = 64;
        break;
    default:
        break;
    }

    printf("Class: %s\n", elf_type);

    char elf_order[15] = "ERROR";
    switch (elf_ident[5])
    {
    case ELFDATANONE:
        strcpy_s(elf_order, "invalid");
        break;
    case ELFDATA2LSB:
        strcpy_s(elf_order, "little endian");
        break;
    case ELFDATA2MSB:
        strcpy_s(elf_order, "big endian");
        break;
    default:
        break;
    }

    printf("Order: %s\n", elf_order);

    return true;
}

void ElfParser::parse_elf_header()
{
    if (0 != fseek(this->elf_file_, 0, 0))
    {
        printf("#parse_elf_header - seek file error.\n");
        return;
    }

    void* elf_header = nullptr;
    size_t elf_header_size = 0;
    if (this->elf_bit_ == 32)
    {
        elf_header = &this->elf_header32_;
        elf_header_size = sizeof(Elf32_Ehdr);
    }
    else  // this->elf_bit_ == 64
    {
        elf_header = &this->elf_header64_;
        elf_header_size = sizeof(Elf64_Ehdr);
    }

    if (0 == fread(elf_header, elf_header_size, 1, this->elf_file_))
    {
        printf("parse elf header%d error.\n", this->elf_bit_);
        return;
    }

    if (this->elf_bit_ == 32)
        print_elf_header(&this->elf_header32_);
    else // this->elf_bit_ == 64
        print_elf_header(&this->elf_header64_);
}

void ElfParser::parse_program_header()
{
    printf("\n>>>>>>>>>>>> parse program header <<<<<<<<<<<<\n\n");

    long program_header_offset = 0;
    size_t program_header_size = 0;

    if (this->elf_bit_ == 32) 
    {
        program_header_offset = this->elf_header32_.e_phoff;
        program_header_size = this->elf_header32_.e_phnum;
        printf("program header offset: %u\n", this->elf_header32_.e_phoff);
        printf("program header size: %u\n", this->elf_header32_.e_phnum);
    }
    else // this->elf_bit_ == 64
    {
        program_header_offset = this->elf_header64_.e_phoff;
        program_header_size = this->elf_header64_.e_phnum;
        printf("program header offset: %llu\n", this->elf_header64_.e_phoff);
        printf("program header size: %u\n", this->elf_header64_.e_phnum);
    }

    if (0 != fseek(this->elf_file_, program_header_offset, 0))
    {
        printf("#parse_program_header - seek file error.\n");
        return;
    }

    // todo:
    printf("todo...\n");

    for(size_t i = 0; i < program_header_size; i++)
    {
        
    }
}
