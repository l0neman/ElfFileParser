#include "ElfParser.h"

#include <cstring>

#include "type/elf.h"
#include "util/Printer.h"

ElfParser::ElfParser() : ElfParser(nullptr) {}

ElfParser::ElfParser(char const* elf_file)
{
    this->elf_file_ = nullptr;
    this->elf_bit_ = 0;
    this->elf_header32_ = Elf32_Ehdr();
    this->elf_header64_ = Elf64_Ehdr();

    this->program_header32_list_ = nullptr;
    this->program_header64_list_ = nullptr;

    this->section_header32_list_ = nullptr;
    this->section_header64_list_ = nullptr;

    if(elf_file == nullptr)
        return;

    const auto f = fopen_s(&this->elf_file_, elf_file, "rb");
    printf("open elf file: %s\n\n", elf_file);
    if (f != 0 || this->elf_file_ == nullptr)
        printf("open elf file error: %s\n", elf_file);
}

ElfParser::~ElfParser()
{
    printf("\n>>>>>>>>>>>> release <<<<<<<<<<<<\n\n");

    if (this->elf_file_ != nullptr) {
        printf("close elf file.\n");
        fclose(this->elf_file_);
    }

    if(this->section_header32_list_ != nullptr)
    {
        printf("delete section header 32 array.\n");
        delete[] this->section_header32_list_;
        this->section_header32_list_ = nullptr;
    }

    if (this->section_header64_list_ != nullptr)
    {
        printf("delete section header 64 array.\n");
        delete[] this->section_header64_list_;
        this->section_header64_list_ = nullptr;
    }

    if (this->program_header32_list_ != nullptr)
    {
        printf("delete program header 32 array.\n");
        delete[] this->program_header32_list_;
        this->program_header32_list_ = nullptr;
    }

    if (this->program_header64_list_ != nullptr)
    {
        printf("delete program header 64 array.\n");
        delete[] this->program_header64_list_;
        this->program_header64_list_ = nullptr;
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
    parse_section_header_list();
    parse_program_header_list();
}

template <typename T = Elf32_Ehdr>
void print_elf_header(T* header, const uint8_t bit)
{
    printf("ident: ");
    Printer::print_char_array(header->e_ident, 16);
    printf("type: %u\n", header->e_type);
    printf("machine: %u\n", header->e_machine);
    printf("version: %u\n", header->e_version);
    if (bit == 32)
    {
        printf("entry: %u\n", header->e_entry);
        printf("phoff: %u\n", header->e_phoff);
        printf("shoff: %u\n", header->e_shoff);
    }
    else // bit == 64
    {
        printf("entry: %llu\n", header->e_entry);
        printf("phoff: %llu\n", header->e_phoff);
        printf("shoff: %llu\n", header->e_shoff);
    }

    printf("flags: %u\n", header->e_flags);
    printf("ehsize: %u\n", header->e_ehsize);
    printf("phentsize: %u\n", header->e_phentsize);
    printf("phnum: %u\n", header->e_phnum);
    printf("shentsize: %u\n", header->e_shentsize);
    printf("shnum: %u\n", header->e_shnum);
    printf("shstrndx: %u\n", header->e_shstrndx);
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
        print_elf_header(&this->elf_header32_, 32);
    else // this->elf_bit_ == 64
        print_elf_header(&this->elf_header64_, 64);
}

void ElfParser::parse_section_header_list()
{
    printf("\n>>>>>>>>>>>> parse section header list <<<<<<<<<<<<\n\n");

    long section_header_offset = 0;
    size_t section_header_count = 0;
    size_t section_header_size = 0;

    if(this->elf_bit_ == 32)
    {
        section_header_offset = this->elf_header32_.e_shoff;
        section_header_count = this->elf_header32_.e_shnum;
        section_header_size = sizeof(Elf32_Shdr);

        this->section_header32_list_ = new Elf32_Shdr[section_header_count];

        printf("section header offset: %u\n", this->elf_header32_.e_shoff);
        printf("section header size: %u\n", this->elf_header32_.e_shnum);
    }
    else // this->elf_bit_ == 64
    {
        section_header_offset = this->elf_header64_.e_shoff;
        section_header_count = this->elf_header64_.e_shnum;
        section_header_size = sizeof(Elf64_Shdr);

        this->section_header64_list_ = new Elf64_Shdr[section_header_count];

        printf("section header offset: %llu\n", this->elf_header64_.e_shoff);
        printf("section header size: %u\n", this->elf_header64_.e_shnum);
    }

    auto offset = section_header_offset;
    for (size_t i = 0; i < section_header_count; i++)
    {
        parse_section_header(offset, i);
        offset += section_header_size;
    }
}


template <typename T = Elf32_Shdr>
static void print_section_header(T* header, const uint8_t bit)
{
    printf("sh_name: %u\n", header->sh_name); // todo: to string
    printf("sh_type: %u\n", header->sh_type);
    printf("sh_link: %u\n", header->sh_link);
    printf("sh_info: %u\n", header->sh_info);
    
    if(bit == 32)
    {
        printf("sh_flags: %u\n", header->sh_flags);
        printf("sh_offset: %u\n", header->sh_offset);
        printf("sh_size: %u\n", header->sh_size);
        printf("sh_addr: %u\n", header->sh_addr);
        printf("sh_addralign: %u\n", header->sh_addralign);
        printf("sh_entsize: %u\n", header->sh_entsize);
    }
    else // bit == 64
    {
        printf("sh_flags: %u\n", header->sh_flags);
        printf("sh_offset: %llu\n", header->sh_offset);
        printf("sh_size: %u\n", header->sh_size);
        printf("sh_addr: %llu\n", header->sh_addr);
        printf("sh_addralign: %llu\n", header->sh_addralign);
        printf("sh_entsize: %llu\n", header->sh_entsize);
    }
}

void ElfParser::parse_section_header(const long offset, const size_t index)
{
    printf("\n>>>>>>>>>>>> parse section header <<<<<<<<<<<<\n\n");

    printf("index: %llu\n", index);
    printf("offset: %ld\n", offset);
    if (0 != fseek(this->elf_file_, offset, 0))
    {
        printf("#parse_section_header - seek file error.\n");
        return;
    }

    void* section_header = nullptr;
    size_t section_header_size = 0;

    if (this->elf_bit_ == 32)
    {
        auto& header = this->section_header32_list_[index];
        section_header = &header;

        section_header_size = sizeof(Elf32_Phdr);
    }
    else // this->elf_bit_ == 64
    {
        auto& header = this->section_header64_list_[index];
        section_header = &header;

        section_header_size = sizeof(Elf64_Phdr);
    }

    if (0 == fread(section_header, section_header_size, 1, this->elf_file_))
    {
        printf("parse section header%d error.\n", this->elf_bit_);
        return;
    }

    if (this->elf_bit_ == 32)
        print_section_header(&this->section_header32_list_[index], 32);
    else
        print_section_header(&this->section_header64_list_[index], 64);
}

void ElfParser::parse_program_header_list()
{
    printf("\n>>>>>>>>>>>> parse program header <<<<<<<<<<<<\n\n");

    long program_header_offset = 0;
    size_t program_header_count = 0;
    size_t program_header_size = 0;

    if (this->elf_bit_ == 32)
    {
        program_header_offset = this->elf_header32_.e_phoff;
        program_header_count = this->elf_header32_.e_phnum;
        program_header_size = sizeof(Elf32_Phdr);

        this->program_header32_list_ = new Elf32_Phdr[program_header_count];

        printf("program header offset: %u\n", this->elf_header32_.e_phoff);
        printf("program header size: %u\n", this->elf_header32_.e_phnum);
    }
    else // this->elf_bit_ == 64
    {
        program_header_offset = this->elf_header64_.e_phoff;
        program_header_count = this->elf_header64_.e_phnum;
        program_header_size = sizeof(Elf64_Phdr);

        this->program_header64_list_ = new Elf64_Phdr[program_header_count];

        printf("program header offset: %llu\n", this->elf_header64_.e_phoff);
        printf("program header size: %u\n", this->elf_header64_.e_phnum);
    }

    auto offset = program_header_offset;
    for (size_t i = 0; i < program_header_count; i++)
    {
        parse_program_header(offset, i);
        offset += program_header_size;
    }
}

template <typename T = Elf32_Phdr>
static void print_program_header(T* header, const uint8_t bit)
{
    printf("p_type: %8x\n", header->p_type);
    printf("p_flags: %u\n", header->p_flags);
    if (bit == 32) 
    {
        printf("p_offset: %u\n", header->p_offset);
        printf("p_vaddr: %u\n", header->p_vaddr);
        printf("p_paddr: %u\n", header->p_paddr);
        printf("p_filesz: %u\n", header->p_filesz);
        printf("p_memsz: %u\n", header->p_memsz);
        printf("p_align: %u\n", header->p_align);
    }
    else // bit == 64
    {
        printf("p_offset: %8x\n", header->p_offset);
        printf("p_vaddr: %llu\n", header->p_vaddr);
        printf("p_paddr: %llu\n", header->p_paddr);
        printf("p_filesz: %llu\n", header->p_filesz);
        printf("p_memsz: %llu\n", header->p_memsz);
        printf("p_align: %llu\n", header->p_align);
    }
}

void ElfParser::parse_program_header(const long offset, const size_t index)
{
    printf("\n>>>>>>>>>>>> parse program header <<<<<<<<<<<<\n\n");

    printf("index: %llu\n", index);
    printf("offset: %ld\n", offset);
    if (0 != fseek(this->elf_file_, offset, 0))
    {
        printf("#parse_program_header - seek file error.\n");
        return;
    }

    void* program_header = nullptr;
    size_t program_header_size = 0;

    if (this->elf_bit_ == 32)
    {
        auto& header = this->program_header32_list_[index];
        program_header = &header;

        program_header_size = sizeof(Elf32_Phdr);
    }
    else // this->elf_bit_ == 64
    {
        auto& header = this->program_header64_list_[index];
        program_header = &header;

        program_header_size = sizeof(Elf64_Phdr);
    }

    if (0 == fread(program_header, program_header_size, 1, this->elf_file_))
    {
        printf("parse program header%d error.\n", this->elf_bit_);
        return;
    }

    if (this->elf_bit_ == 32)
        print_program_header(&this->program_header32_list_[index], 32);
    else
        print_program_header(&this->program_header64_list_[index], 64);
}
