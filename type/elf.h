#ifndef ELF_H
#define ELF_H

#include <cstdint>

typedef uint32_t Elf32_Addr; // Program address
typedef uint32_t Elf32_Off;  // File offset
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef int32_t  Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

static constexpr char ElfMagic[] = { 0x7f, 'E', 'L', 'F', '\0' };

enum {
    EI_MAG0 = 0,           // 文件标识索引
    EI_MAG1 = 1,           // 文件标识索引
    EI_MAG2 = 2,           // 文件标识索引
    EI_MAG3 = 3,           // 文件标识索引
    EI_CLASS = 4,          // File class.
    EI_DATA = 5,           // Data encoding.
    EI_VERSION = 6,        // File version.
    EI_OSABI = 7,          // OS/ABI identification.
    EI_ABIVERSION = 8,     // ABI version.
    EI_PAD = 9,            // Start of padding bytes.
    EI_NIDENT = 16         // Number of bytes in e_ident.
};

constexpr char ELFMAG0 = ElfMagic[EI_MAG0];
constexpr char ELFMAG1 = ElfMagic[EI_MAG1];
constexpr char ELFMAG2 = ElfMagic[EI_MAG2];
constexpr char ELFMAG3 = ElfMagic[EI_MAG3];
constexpr char ELFMAG[] = "\177ELF";
constexpr int SELFMAG = 4;
constexpr int NT_PRSTATUS = 1;

struct Elf32_Ehdr {
    unsigned char e_ident[EI_NIDENT]; // 文件标识
    Elf32_Half    e_type;      // 文件类型
    Elf32_Half    e_machine;   // Required architecture for this file (see EM_*)
    Elf32_Word    e_version;   // Must be equal to 1
    Elf32_Addr    e_entry;     // Address to jump to in order to start program
    Elf32_Off     e_phoff;     // Program header table's file offset, in bytes
    Elf32_Off     e_shoff;     // Section header table's file offset, in bytes
    Elf32_Word    e_flags;     // Processor-specific flags
    Elf32_Half    e_ehsize;    // Size of ELF header, in bytes
    Elf32_Half    e_phentsize; // Size of an entry in the program header table
    Elf32_Half    e_phnum;     // Number of entries in the program header table
    Elf32_Half    e_shentsize; // Size of an entry in the section header table
    Elf32_Half    e_shnum;     // Number of entries in the section header table
    Elf32_Half    e_shstrndx;  // Sect hdr table index of sect name string table
};


struct Elf64_Ehdr {
    unsigned char e_ident[EI_NIDENT];
    Elf64_Half    e_type;
    Elf64_Half    e_machine;
    Elf64_Word    e_version;
    Elf64_Addr    e_entry;
    Elf64_Off     e_phoff;
    Elf64_Off     e_shoff;
    Elf64_Word    e_flags;
    Elf64_Half    e_ehsize;
    Elf64_Half    e_phentsize;
    Elf64_Half    e_phnum;
    Elf64_Half    e_shentsize;
    Elf64_Half    e_shnum;
    Elf64_Half    e_shstrndx;
};

#endif // !ELF_H
