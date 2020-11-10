#include "ElfParser.h"

#include <cstring>
#include <vector>


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

    this->symbol32_list_ = nullptr;
    this->symbol64_list_ = nullptr;

    this->string_table_ = nullptr;
    this->symbol_string_table_ = nullptr;

    this->relocation32_list_ = nullptr;
    this->relocation64_list_ = nullptr;

    if (elf_file == nullptr)
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

    if (this->section_header32_list_ != nullptr)
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

    if (this->string_table_ != nullptr)
    {
        printf("delete string table.\n");
        delete[] this->string_table_;
        this->string_table_ = nullptr;
    }

    if (this->symbol_string_table_ != nullptr)
    {
        printf("delete symbol string table.\n");
        delete[] this->symbol_string_table_;
        this->symbol_string_table_ = nullptr;
    }

    if (this->symbol32_list_ != nullptr)
    {
        printf("delete symbol 32 list.\n");
        delete[] this->symbol32_list_;
        this->symbol32_list_ = nullptr;
    }

    if (this->symbol64_list_ != nullptr)
    {
        printf("delete symbol 64 list.\n");
        delete[] this->symbol64_list_;
        this->symbol64_list_ = nullptr;
    }

    if (this->relocation32_list_ != nullptr)
    {
        printf("delete relocation 32 list.\n");
        delete[] this->relocation32_list_;
        this->relocation32_list_ = nullptr;
    }

    if (this->relocation64_list_ != nullptr)
    {
        printf("delete relocation 64 list.\n");
        delete[] this->relocation64_list_;
        this->relocation64_list_ = nullptr;
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
    parse_string_table();
    print_section_header_list();
    parse_symbol_string_table();
    parse_program_header_list();
    parse_section_list();
}

template <typename T = Elf32_Ehdr>
void print_elf_header(T* header, const uint8_t bit)
{
    printf("ident: \t\t");
    Printer::print_char_array(header->e_ident, 16);
    printf("type: \t\t%u\n", header->e_type);
    printf("machine: \t%u\n", header->e_machine);
    printf("version: \t%u\n", header->e_version);
    if (bit == 32)
    {
        printf("entry: \t\t%u\n", header->e_entry);
        printf("phoff: \t\t%u\n", header->e_phoff);
        printf("shoff: \t\t%u\n", header->e_shoff);
    }
    else // bit == 64
    {
        printf("entry: \t\t%llu\n", header->e_entry);
        printf("phoff: \t\t%llu\n", header->e_phoff);
        printf("shoff: \t\t%llu\n", header->e_shoff);
    }

    printf("flags: \t\t0x%x\n", header->e_flags);
    printf("ehsize: \t%u\n", header->e_ehsize);
    printf("phentsize: \t%u\n", header->e_phentsize);
    printf("phnum: \t\t%u\n", header->e_phnum);
    printf("shentsize: \t%u\n", header->e_shentsize);
    printf("shnum: \t\t%u\n", header->e_shnum);
    printf("shstrndx: \t%u\n", header->e_shstrndx);
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

    printf("Class: \t\t%s\n", elf_type);

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

    printf("Order: \t\t%s\n", elf_order);

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


// todo: pretty print
template <typename T = Elf32_Shdr>
static void print_section_header(T* header, const uint8_t bit)
{
#ifdef _PRINT_SECTION_HEADER_LIST_
    printf("sh_name: \t%u\n", header->sh_name);
    printf("sh_type: \t0x%x\n", header->sh_type);
    printf("sh_link: \t%u\n", header->sh_link);
    printf("sh_info: \t%u\n", header->sh_info);

    if (bit == 32)
    {
        printf("sh_flags: \t%u\n", header->sh_flags);
        printf("sh_offset: \t%u\n", header->sh_offset);
        printf("sh_size: \t%u\n", header->sh_size);
        printf("sh_addr: \t%u\n", header->sh_addr);
        printf("sh_addralign: \t%u\n", header->sh_addralign);
        printf("sh_entsize: \t%u\n", header->sh_entsize);
    }
    else // bit == 64
    {
        printf("sh_flags: \t%llu\n", header->sh_flags);
        printf("sh_offset: \t%llu\n", header->sh_offset);
        printf("sh_size: \t%llu\n", header->sh_size);
        printf("sh_addr: \t%llu\n", header->sh_addr);
        printf("sh_addralign: \t%llu\n", header->sh_addralign);
        printf("sh_entsize: %llu\n", header->sh_entsize);
    }
#endif // _PRINT_PROGRAM_HEADER_LIST_
}

void ElfParser::parse_section_header_list()
{
    printf("\n>>>>>>>>>>>> parse section header list <<<<<<<<<<<<\n\n");

    long section_header_offset = 0;
    size_t section_header_count = 0;
    size_t section_header_size = 0;
    void* section_header_list = nullptr;

    if (this->elf_bit_ == 32)
    {
        section_header_offset = this->elf_header32_.e_shoff;
        section_header_count = this->elf_header32_.e_shnum;
        section_header_size = sizeof(Elf32_Shdr);

        this->section_header32_list_ = new Elf32_Shdr[section_header_count];
        section_header_list = this->section_header32_list_;

        printf("section header offset: \t%u\n", this->elf_header32_.e_shoff);
        printf("section header size: \t%u\n", this->elf_header32_.e_shnum);
    }
    else // this->elf_bit_ == 64
    {
        section_header_offset = this->elf_header64_.e_shoff;
        section_header_count = this->elf_header64_.e_shnum;
        section_header_size = sizeof(Elf64_Shdr);

        this->section_header64_list_ = new Elf64_Shdr[section_header_count];
        section_header_list = this->section_header64_list_;

        printf("section header offset: \t%llu\n", this->elf_header64_.e_shoff);
        printf("section header size: \t%u\n", this->elf_header64_.e_shnum);
    }

    if (0 != fseek(this->elf_file_, section_header_offset, 0))
    {
        printf("#parse_section_header - seek file error.\n");
        return;
    }

    if (0 == fread(section_header_list, section_header_size, section_header_count, this->elf_file_))
    {
        printf("parse section header%d error.\n", this->elf_bit_);
        return;
    }
}

void ElfParser::parse_string_table()
{
    printf("\n>>>>>>>>>>>> parse string table <<<<<<<<<<<<\n\n");
    // for .shstrtab;

    size_t offset;
    size_t size;
    if (this->elf_bit_ == 32)
    {
        // ×Ö·û´®±íÏÂ±ê
        const auto str_table_index = this->elf_header32_.e_shstrndx;
        const auto& section_header = this->section_header32_list_[str_table_index];
        offset = section_header.sh_offset;
        size = section_header.sh_size;
    }
    else // this->elf_bit_ == 64
    {
        const auto str_table_index = this->elf_header64_.e_shstrndx;
        const auto& section_header = this->section_header64_list_[str_table_index];
        offset = section_header.sh_offset;
        size = section_header.sh_size;
    }

    if (0 != fseek(this->elf_file_, offset, 0))
    {
        printf("#parse_string_table - seek file error.\n");
        return;
    }

    this->string_table_ = new char[size];
    if (0 == fread(this->string_table_, size, 1, this->elf_file_))
    {
        printf("parse string table%d error.\n", this->elf_bit_);
        return;
    }

    size_t string_count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (this->string_table_[i] == 0 && i != (size - 1))
        {
            const auto off = i + 1;
            const auto* str = get_string_from_string_table(off);
            const auto len = strlen(str);

            printf("str[%llu] \tlen: %llu, s: %s\n", off, len, str);

            string_count++;
        }
    }

    printf("string count: %llu\n", string_count);
}

void ElfParser::print_section_header_list() const
{
#ifdef _PRINT_SECTION_HEADER_LIST_
    size_t section_header_count = 0;

    if (this->elf_bit_ == 32)
        section_header_count = this->elf_header32_.e_shnum;
    else // this->elf_bit_ == 64
        section_header_count = this->elf_header64_.e_shnum;

    for (size_t i = 0; i < section_header_count; i++)
    {
        printf("\n>>>>>>>>>>>> parse section header <<<<<<<<<<<<\n\n");

        printf("index: \t\t%llu\n", i);

        if (this->elf_bit_ == 32) 
        {
            printf("name: \t\t%s\n\n", get_string_from_string_table(this->section_header32_list_[i].sh_name));
            print_section_header(&this->section_header32_list_[i], this->elf_bit_);
        }
        else // this->elf_bit_ == 64 
        {
            printf("name: \t\t%s\n\n", get_string_from_string_table(this->section_header64_list_[i].sh_name));
            print_section_header(&this->section_header64_list_[i], this->elf_bit_);
        }
     }
#endif // _PRINT_SECTION_HEADER_LIST_
}

void ElfParser::parse_symbol_string_table()
{
    printf("\n>>>>>>>>>>>> parse symbol string table <<<<<<<<<<<<\n\n");
    // for .dynstr

    size_t offset = 0;
    size_t size = 0;
    if(this->elf_bit_ == 32)
    {
        for (size_t i = 0; i < this->elf_header32_.e_shnum; i++)
        {
            auto& section_header = this->section_header32_list_[i];
            const auto* section_name = get_string_from_string_table(section_header.sh_name);
            if(section_header.sh_type == SHT_STRTAB && strcmp(section_name, ".dynstr") == 0)
            {
                offset = section_header.sh_offset;
                size = section_header.sh_size;
                break;
            }
        }
    }
    else // this->elf_bit_ == 32
    {
        for (size_t i = 0; i < this->elf_header64_.e_shnum; i++)
        {
            auto& section_header = this->section_header64_list_[i];
            const auto* section_name = get_string_from_string_table(section_header.sh_name);
            if (section_header.sh_type == SHT_STRTAB && strcmp(section_name, ".dynstr") == 0)
            {
                offset = section_header.sh_offset;
                size = section_header.sh_size;
                break;
            }
        }
    }

    if(offset == 0 || size == 0)
    {
        printf("error: not found section .dynstr\n");
        return;
    }

    if (0 != fseek(this->elf_file_, offset, 0))
    {
        printf("#parse_symbol_string_table - seek file error.\n");
        return;
    }

    this->symbol_string_table_ = new char[size];
    if (0 == fread(this->symbol_string_table_, size, 1, this->elf_file_))
    {
        printf("parse symbol string table%d error.\n", this->elf_bit_);
        return;
    }

    size_t string_count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (this->symbol_string_table_[i] == 0 && i != (size - 1))
        {
            const auto off = i + 1;
            const auto* str = get_string_from_symbol_string_table(off);
            const auto len = strlen(str);

            printf("str[%llu] \tlen: %llu, s: %s\n", off, len, str);

            string_count++;
        }
    }

    printf("string count: %llu\n", string_count);
}

const char* ElfParser::get_string_from_string_table(const size_t offset) const
{
    return &this->string_table_[offset];
}


inline const char* ElfParser::get_string_from_symbol_string_table(size_t offset) const
{
    return &this->symbol_string_table_[offset];
}

// todo: pretty print
template <typename T = Elf32_Phdr>
static void print_program_header(T* header, const uint8_t bit)
{
#ifdef _PRINT_PROGRAM_HEADER_LIST_
    printf("p_type: \t0x%x\n", header->p_type);
    printf("p_flags: \t%u\n", header->p_flags);
    if (bit == 32)
    {
        printf("p_offset: \t%u\n", header->p_offset);
        printf("p_vaddr: \t%u\n", header->p_vaddr);
        printf("p_paddr: \t%u\n", header->p_paddr);
        printf("p_filesz: \t%u\n", header->p_filesz);
        printf("p_memsz: \t%u\n", header->p_memsz);
        printf("p_align: \t%u\n", header->p_align);
    }
    else // bit == 64
    {
        printf("p_offset: \t0x%x\n", header->p_offset);
        printf("p_vaddr: \t%llu\n", header->p_vaddr);
        printf("p_paddr: \t%llu\n", header->p_paddr);
        printf("p_filesz: \t%llu\n", header->p_filesz);
        printf("p_memsz: \t%llu\n", header->p_memsz);
        printf("p_align: \t%llu\n", header->p_align);
    }
#endif // _PRINT_PROGRAM_HEADER_LIST_
}

void ElfParser::parse_program_header_list()
{
    printf("\n>>>>>>>>>>>> parse program list <<<<<<<<<<<<\n\n");

    long program_header_list_offset = 0;
    size_t program_header_count = 0;
    size_t program_header_size = 0;
    void* program_header_list = nullptr;

    if (this->elf_bit_ == 32)
    {
        program_header_list_offset = this->elf_header32_.e_phoff;
        program_header_count = this->elf_header32_.e_phnum;
        program_header_size = sizeof(Elf32_Phdr);

        this->program_header32_list_ = new Elf32_Phdr[program_header_count];
        program_header_list = this->program_header32_list_;

        printf("program header offset: \t%u\n", this->elf_header32_.e_phoff);
        printf("program header size: \t%u\n", this->elf_header32_.e_phnum);
    }
    else // this->elf_bit_ == 64
    {
        program_header_list_offset = this->elf_header64_.e_phoff;
        program_header_count = this->elf_header64_.e_phnum;
        program_header_size = sizeof(Elf64_Phdr);

        this->program_header64_list_ = new Elf64_Phdr[program_header_count];
        program_header_list = this->program_header64_list_;

        printf("program header offset: \t%llu\n", this->elf_header64_.e_phoff);
        printf("program header size: \t%u\n", this->elf_header64_.e_phnum);
    }

    if (0 != fseek(this->elf_file_, program_header_list_offset, 0))
    {
        printf("#parse_program_header_list - seek file error.\n");
        return;
    }

    if (0 == fread(program_header_list, program_header_size, program_header_count, this->elf_file_))
    {
        printf("parse program header%d error.\n", this->elf_bit_);
        return;
    }

#ifdef _PRINT_PROGRAM_HEADER_LIST_
    for (size_t i = 0; i < program_header_count; i++)
    {
        printf("\n>>>>>>>>>>>> parse program header <<<<<<<<<<<<\n\n");
        printf("index: \t\t%llu\n\n", i);

        if (this->elf_bit_ == 32)
            print_program_header(&this->program_header32_list_[i], this->elf_bit_);
        else // this->elf_bit_ == 64
            print_program_header(&this->program_header64_list_[i], this->elf_bit_);
    }
#endif // _PRINT_PROGRAM_HEADER_LIST_
}

void ElfParser::parse_section_list()
{
    printf("\n>>>>>>>>>>>> parse section list <<<<<<<<<<<<\n\n");

    size_t list_len = 0;
    if (this->elf_bit_ == 32)
        list_len = this->elf_header32_.e_shnum;
    else // this->elf_bit_ == 64
        list_len = this->elf_header64_.e_shnum;

    if (this->elf_bit_ == 32) 
    {
        for (size_t i = 0; i < list_len; i++)
        {
            auto& section_header = this->section_header32_list_[i];
            printf("parse section: %s\n", get_string_from_string_table(section_header.sh_name));

            switch (section_header.sh_type)
            {
            case SHT_SYMTAB:
                break;
            case SHT_DYNSYM:
                parse_symbol_table(section_header.sh_offset, section_header.sh_size);
                break;
            case SHT_REL:
                parse_relocation_table(section_header.sh_offset, section_header.sh_size);
                break;
            default:
                printf("ignored.\n");
                break;
            }
        }
    }
    else // this->elf_bit_ == 64
    {
        for (size_t i = 0; i < list_len; i++)
        {
            auto& section_header = this->section_header64_list_[i];
            printf("parse section: %s\n", get_string_from_string_table(section_header.sh_name));

            switch (section_header.sh_type)
            {
            case SHT_SYMTAB:
                break;
            case SHT_DYNSYM:
                parse_symbol_table(section_header.sh_offset, section_header.sh_size);
                break;
            default:
                printf("ignored.\n");
                break;
            }
        }
    }
}


template <typename T = Elf32_Sym>
static void print_symbol(T* symbol, const uint8_t bit)
{
    if (bit == 32)
    {
        printf("st_name: \t%u\n", symbol->st_name);
        printf("st_value: \t%u\n", symbol->st_value);
        printf("st_size: \t%u\n", symbol->st_size);
        printf("st_info: \t%u\n", symbol->st_info);
        printf("st_other: \t%u\n", symbol->st_other);
        printf("st_shndx: \t%u\n", symbol->st_shndx);
    }
    else // bit == 64
    {
        printf("st_name: \t%llu\n", symbol->st_name);
        printf("st_value: \t%llu\n", symbol->st_value);
        printf("st_size: \t%llu\n", symbol->st_size);
        printf("st_info: \t%llu\n", symbol->st_info);
        printf("st_other: \t%llu\n", symbol->st_other);
        printf("st_shndx: \t%llu\n", symbol->st_shndx);
    }
}

void ElfParser::parse_symbol_table(const long offset, const size_t size)
{
    printf("\n>>>>>>>>>>>> parse symbol table <<<<<<<<<<<<\n\n");

    if (0 != fseek(this->elf_file_, offset, 0))
    {
        printf("#parse_symbol_table - seek file error.\n");
        return;
    }

    size_t sym_size = 0;
    size_t sym_count = 0;
    void* symbol_buffer = nullptr;
    if (this->elf_bit_ == 32)
    {
        sym_size = sizeof(Elf32_Sym);
        sym_count = size / sym_size;

        this->symbol32_list_ = new Elf32_Sym[sym_count];
        symbol_buffer = this->symbol32_list_;
    }
    else // this->elf_bit_ == 64
    {
        sym_size = sizeof(Elf64_Sym);
        sym_count = size / sym_size;

        this->symbol64_list_ = new Elf64_Sym[sym_count];
        symbol_buffer = this->symbol64_list_;
    }

    printf("symbol count: %llu\n", sym_count);

    if (0 == fread(symbol_buffer, sym_size, sym_count, this->elf_file_))
    {
        printf("parse symbol table%d error.\n", this->elf_bit_);
        return;
    }

#ifdef _PRINT_SYMBOL_TABLE_
    for (size_t i = 0; i < sym_count; i++)
    {
        printf("\n>>>>>>>>>>>> parse symbol <<<<<<<<<<<<\n\n");

        printf("index: %llu\n", i);

        if (this->elf_bit_ == 32) 
        {
            auto& symbol  = this->symbol32_list_[i];
            printf("symbol name: %s\n\n", get_string_from_symbol_string_table(symbol.st_name));
            print_symbol(&symbol, this->elf_bit_);
        }
        else // this-elf_bit_ == 64
        {
            auto& symbol = this->symbol64_list_[i];
            printf("symbol name: %s\n\n", get_string_from_symbol_string_table(symbol.st_name));
            print_symbol(&symbol, this->elf_bit_);
        }
    }
#endif // _PRINT_SYMBOL_TABLE_
}

void ElfParser::parse_relocation_table(const long offset, const size_t size)
{
    printf("\n>>>>>>>>>>>> parse relocation table <<<<<<<<<<<<\n\n");

    if (0 != fseek(this->elf_file_, offset, 0))
    {
        printf("#parse_relocation_table - seek file error.\n");
        return;
    }

    size_t rel_size = 0;
    size_t rel_count = 0;
    void* rel_buffer = nullptr;
    if (this->elf_bit_ == 32)
    {
        rel_size = sizeof(Elf32_Rel);
        rel_count = size / rel_size;

        this->relocation32_list_ = new Elf32_Rel[rel_count];
        rel_buffer = this->relocation32_list_;
    }
    else // this->elf_bit_ == 64
    {
        rel_size = sizeof(Elf64_Rel);
        rel_count = size / rel_size;

        this->relocation64_list_ = new Elf64_Rel[rel_count];
        rel_buffer = this->relocation64_list_;
    }

    printf("relocation entries count: %llu\n", rel_count);

    if (0 == fread(rel_buffer, rel_size, rel_count, this->elf_file_))
    {
        printf("parse relocation table%d error.\n", this->elf_bit_);
        return;
    }

#ifdef _PRINT_RELOCATION_TABLE
    for (size_t i = 0; i < rel_count; i++)
    {
        printf("\n>>>>>>>>>>>> parse relocation entry <<<<<<<<<<<<\n\n");

        printf("index: %llu\n\n", i);

        if (this->elf_bit_ == 32)
        {
            auto& relocation = this->relocation32_list_[i];
            printf("r_offset: \t%u\n", relocation.r_offset);
            printf("r_info: \t%u\n", relocation.r_info);
        }
        else // this-elf_bit_ == 64
        {
            auto& relocation = this->relocation64_list_[i];
            printf("r_offset: \t%llu\n", relocation.r_offset);
            printf("r_info: \t%llu\n", relocation.r_info);
        }
    }
#endif // _PRINT_RELOCATION_TABLE
}
