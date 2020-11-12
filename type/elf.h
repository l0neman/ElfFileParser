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

// e_ident size and indices.
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

// Elf 文件头
struct Elf32_Ehdr {
    unsigned char e_ident[EI_NIDENT]; // 文件标识
    Elf32_Half    e_type;      // 文件类型
    Elf32_Half    e_machine;   // ELF 文件的 CPU 平台属性，相关常量以 EM_ 开头
    Elf32_Word    e_version;   // ELF 版本号，一般为常数 1
    Elf32_Addr    e_entry;     // 入口地址，规定 ELF 程序的入口虚拟地址，操作系统在加载完该程序后从这个地址开始执行进程的指令
    Elf32_Off     e_phoff;     // Program header 表的文件偏移字节
    Elf32_Off     e_shoff;     // 段表在文件中的偏移
    Elf32_Word    e_flags;     // LF 标志位，用来标识一些 ELF 文件平台相关的属性。相关常量格式一般为 EF_machine_flag，machine 为平台，flag 为标志
    Elf32_Half    e_ehsize;    // ELF 文件头本身的大小
    Elf32_Half    e_phentsize; // Program header 表的大小
    Elf32_Half    e_phnum;     // Program header 表的数量
    Elf32_Half    e_shentsize; // 段表描述符的大小，这个一般等于一节
    Elf32_Half    e_shnum;     // 段表描述符数量。这个值等于 ELF 文件中拥有段的数量
    Elf32_Half    e_shstrndx;  // 段表字符串表所在的段在段表中的下标
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

// 程序头表，表示装载后的 Segment 结构
struct Elf32_Phdr {
    Elf32_Word p_type;   // 段类型
    Elf32_Off  p_offset; // 段在文件中的偏移
    Elf32_Addr p_vaddr;  // 段的第一个字节在虚拟地址空间的起始位置，整个程序表头中
    Elf32_Addr p_paddr;  // 段的物理装载地址，即 LMA（Load Memory Address），一般情况下 p_paddr 和 p_vaddr 是相同的
    Elf32_Word p_filesz; // 段在 ELF 文件中所占空间的长度，可能为 0
    Elf32_Word p_memsz;  // 段在进程虚拟空间中所占空间的长度，可能为 0
    Elf32_Word p_flags;  // 段的权限属性，比如可读 "R"，可写 "W" 和可执行 "X"
    Elf32_Word p_align;  // 段的对齐属性，实际对齐字节等于 2 的 p_align 次方
};


struct Elf64_Phdr {
    Elf64_Word  p_type;  
    Elf64_Word  p_flags; 
    Elf64_Off   p_offset;
    Elf64_Addr  p_vaddr; 
    Elf64_Addr  p_paddr; 
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz; 
    Elf64_Xword p_align; 
};

// File types
enum {
    ET_NONE = 0,        // No file type
    ET_REL = 1,         // Relocatable file
    ET_EXEC = 2,        // Executable file
    ET_DYN = 3,         // Shared object file
    ET_CORE = 4,        // Core file
    ET_LOPROC = 0xff00, // Beginning of processor-specific codes
    ET_HIPROC = 0xffff  // Processor-specific
};

// Versioning
enum {
    EV_NONE = 0,
    EV_CURRENT = 1
};

// Machine architectures
enum {
    EM_NONE          = 0, // No machine
    EM_M32           = 1, // AT&T WE 32100
    EM_SPARC         = 2, // SPARC
    EM_386           = 3, // Intel 386
    EM_68K           = 4, // Motorola 68000
    EM_88K           = 5, // Motorola 88000
    EM_486           = 6, // Intel 486 (deprecated)
    EM_860           = 7, // Intel 80860
    EM_MIPS          = 8, // MIPS R3000
    EM_S370          = 9, // IBM System/370
    EM_MIPS_RS3_LE   = 10, // MIPS RS3000 Little-endian
    EM_PARISC        = 15, // Hewlett-Packard PA-RISC
    EM_VPP500        = 17, // Fujitsu VPP500
    EM_SPARC32PLUS   = 18, // Enhanced instruction set SPARC
    EM_960           = 19, // Intel 80960
    EM_PPC           = 20, // PowerPC
    EM_PPC64         = 21, // PowerPC64
    EM_S390          = 22, // IBM System/390
    EM_SPU           = 23, // IBM SPU/SPC
    EM_V800          = 36, // NEC V800
    EM_FR20          = 37, // Fujitsu FR20
    EM_RH32          = 38, // TRW RH-32
    EM_RCE           = 39, // Motorola RCE
    EM_ARM           = 40, // ARM
    EM_ALPHA         = 41, // DEC Alpha
    EM_SH            = 42, // Hitachi SH
    EM_SPARCV9       = 43, // SPARC V9
    EM_TRICORE       = 44, // Siemens TriCore
    EM_ARC           = 45, // Argonaut RISC Core
    EM_H8_300        = 46, // Hitachi H8/300
    EM_H8_300H       = 47, // Hitachi H8/300H
    EM_H8S           = 48, // Hitachi H8S
    EM_H8_500        = 49, // Hitachi H8/500
    EM_IA_64         = 50, // Intel IA-64 processor architecture
    EM_MIPS_X        = 51, // Stanford MIPS-X
    EM_COLDFIRE      = 52, // Motorola ColdFire
    EM_68HC12        = 53, // Motorola M68HC12
    EM_MMA           = 54, // Fujitsu MMA Multimedia Accelerator
    EM_PCP           = 55, // Siemens PCP
    EM_NCPU          = 56, // Sony nCPU embedded RISC processor
    EM_NDR1          = 57, // Denso NDR1 microprocessor
    EM_STARCORE      = 58, // Motorola Star*Core processor
    EM_ME16          = 59, // Toyota ME16 processor
    EM_ST100         = 60, // STMicroelectronics ST100 processor
    EM_TINYJ         = 61, // Advanced Logic Corp. TinyJ embedded processor family
    EM_X86_64        = 62, // AMD x86-64 architecture
    EM_PDSP          = 63, // Sony DSP Processor
    EM_PDP10         = 64, // Digital Equipment Corp. PDP-10
    EM_PDP11         = 65, // Digital Equipment Corp. PDP-11
    EM_FX66          = 66, // Siemens FX66 microcontroller
    EM_ST9PLUS       = 67, // STMicroelectronics ST9+ 8/16 bit microcontroller
    EM_ST7           = 68, // STMicroelectronics ST7 8-bit microcontroller
    EM_68HC16        = 69, // Motorola MC68HC16 Microcontroller
    EM_68HC11        = 70, // Motorola MC68HC11 Microcontroller
    EM_68HC08        = 71, // Motorola MC68HC08 Microcontroller
    EM_68HC05        = 72, // Motorola MC68HC05 Microcontroller
    EM_SVX           = 73, // Silicon Graphics SVx
    EM_ST19          = 74, // STMicroelectronics ST19 8-bit microcontroller
    EM_VAX           = 75, // Digital VAX
    EM_CRIS          = 76, // Axis Communications 32-bit embedded processor
    EM_JAVELIN       = 77, // Infineon Technologies 32-bit embedded processor
    EM_FIREPATH      = 78, // Element 14 64-bit DSP Processor
    EM_ZSP           = 79, // LSI Logic 16-bit DSP Processor
    EM_MMIX          = 80, // Donald Knuth's educational 64-bit processor
    EM_HUANY         = 81, // Harvard University machine-independent object files
    EM_PRISM         = 82, // SiTera Prism
    EM_AVR           = 83, // Atmel AVR 8-bit microcontroller
    EM_FR30          = 84, // Fujitsu FR30
    EM_D10V          = 85, // Mitsubishi D10V
    EM_D30V          = 86, // Mitsubishi D30V
    EM_V850          = 87, // NEC v850
    EM_M32R          = 88, // Mitsubishi M32R
    EM_MN10300       = 89, // Matsushita MN10300
    EM_MN10200       = 90, // Matsushita MN10200
    EM_PJ            = 91, // picoJava
    EM_OPENRISC      = 92, // OpenRISC 32-bit embedded processor
    EM_ARC_COMPACT   = 93, // ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5)
    EM_XTENSA        = 94, // Tensilica Xtensa Architecture
    EM_VIDEOCORE     = 95, // Alphamosaic VideoCore processor
    EM_TMM_GPP       = 96, // Thompson Multimedia General Purpose Processor
    EM_NS32K         = 97, // National Semiconductor 32000 series
    EM_TPC           = 98, // Tenor Network TPC processor
    EM_SNP1K         = 99, // Trebia SNP 1000 processor
    EM_ST200         = 100, // STMicroelectronics (www.st.com) ST200
    EM_IP2K          = 101, // Ubicom IP2xxx microcontroller family
    EM_MAX           = 102, // MAX Processor
    EM_CR            = 103, // National Semiconductor CompactRISC microprocessor
    EM_F2MC16        = 104, // Fujitsu F2MC16
    EM_MSP430        = 105, // Texas Instruments embedded microcontroller msp430
    EM_BLACKFIN      = 106, // Analog Devices Blackfin (DSP) processor
    EM_SE_C33        = 107, // S1C33 Family of Seiko Epson processors
    EM_SEP           = 108, // Sharp embedded microprocessor
    EM_ARCA          = 109, // Arca RISC Microprocessor
    EM_UNICORE       = 110, // Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University
    EM_EXCESS        = 111, // eXcess: 16/32/64-bit configurable embedded CPU
    EM_DXP           = 112, // Icera Semiconductor Inc. Deep Execution Processor
    EM_ALTERA_NIOS2  = 113, // Altera Nios II soft-core processor
    EM_CRX           = 114, // National Semiconductor CompactRISC CRX
    EM_XGATE         = 115, // Motorola XGATE embedded processor
    EM_C166          = 116, // Infineon C16x/XC16x processor
    EM_M16C          = 117, // Renesas M16C series microprocessors
    EM_DSPIC30F      = 118, // Microchip Technology dsPIC30F Digital Signal Controller
    EM_CE            = 119, // Freescale Communication Engine RISC core
    EM_M32C          = 120, // Renesas M32C series microprocessors
    EM_TSK3000       = 131, // Altium TSK3000 core
    EM_RS08          = 132, // Freescale RS08 embedded processor
    EM_SHARC         = 133, // Analog Devices SHARC family of 32-bit DSP processors
    EM_ECOG2         = 134, // Cyan Technology eCOG2 microprocessor
    EM_SCORE7        = 135, // Sunplus S+core7 RISC processor
    EM_DSP24         = 136, // New Japan Radio (NJR) 24-bit DSP Processor
    EM_VIDEOCORE3    = 137, // Broadcom VideoCore III processor
    EM_LATTICEMICO32 = 138, // RISC processor for Lattice FPGA architecture
    EM_SE_C17        = 139, // Seiko Epson C17 family
    EM_TI_C6000      = 140, // The Texas Instruments TMS320C6000 DSP family
    EM_TI_C2000      = 141, // The Texas Instruments TMS320C2000 DSP family
    EM_TI_C5500      = 142, // The Texas Instruments TMS320C55x DSP family
    EM_MMDSP_PLUS    = 160, // STMicroelectronics 64bit VLIW Data Signal Processor
    EM_CYPRESS_M8C   = 161, // Cypress M8C microprocessor
    EM_R32C          = 162, // Renesas R32C series microprocessors
    EM_TRIMEDIA      = 163, // NXP Semiconductors TriMedia architecture family
    EM_HEXAGON       = 164, // Qualcomm Hexagon processor
    EM_8051          = 165, // Intel 8051 and variants
    EM_STXP7X        = 166, // STMicroelectronics STxP7x family of configurable and extensible RISC processors
    EM_NDS32         = 167, // Andes Technology compact code size embedded RISC processor family
    EM_ECOG1         = 168, // Cyan Technology eCOG1X family
    EM_ECOG1X        = 168, // Cyan Technology eCOG1X family
    EM_MAXQ30        = 169, // Dallas Semiconductor MAXQ30 Core Micro-controllers
    EM_XIMO16        = 170, // New Japan Radio (NJR) 16-bit DSP Processor
    EM_MANIK         = 171, // M2000 Reconfigurable RISC Microprocessor
    EM_CRAYNV2       = 172, // Cray Inc. NV2 vector architecture
    EM_RX            = 173, // Renesas RX family
    EM_METAG         = 174, // Imagination Technologies META processor architecture
    EM_MCST_ELBRUS   = 175, // MCST Elbrus general purpose hardware architecture
    EM_ECOG16        = 176, // Cyan Technology eCOG16 family
    EM_CR16          = 177, // National Semiconductor CompactRISC CR16 16-bit microprocessor
    EM_ETPU          = 178, // Freescale Extended Time Processing Unit
    EM_SLE9X         = 179, // Infineon Technologies SLE9X core
    EM_L10M          = 180, // Intel L10M
    EM_K10M          = 181, // Intel K10M
    EM_AARCH64       = 183, // ARM AArch64
    EM_AVR32         = 185, // Atmel Corporation 32-bit microprocessor family
    EM_STM8          = 186, // STMicroeletronics STM8 8-bit microcontroller
    EM_TILE64        = 187, // Tilera TILE64 multicore architecture family
    EM_TILEPRO       = 188, // Tilera TILEPro multicore architecture family
    EM_CUDA          = 190, // NVIDIA CUDA architecture
    EM_TILEGX        = 191, // Tilera TILE-Gx multicore architecture family
    EM_CLOUDSHIELD   = 192, // CloudShield architecture family
    EM_COREA_1S      = 193, // KIPO-KAIST Core-A 1st generation processor family
    EM_COREA_2ND     = 194, // KIPO-KAIST Core-A 2nd generation processor family
    EM_ARC_COMPACT2  = 195, // Synopsys ARCompact V2
    EM_OPEN8         = 196, // Open8 8-bit RISC soft processor core
    EM_RL78          = 197, // Renesas RL78 family
    EM_VIDEOCORE5    = 198, // Broadcom VideoCore V processor
    EM_78KOR         = 199, // Renesas 78KOR family
    EM_56800EX       = 200  // Freescale 56800EX Digital Signal Controller (DSC)
};

// Object file classes.
enum {
    ELFCLASSNONE = 0,
    ELFCLASS32 = 1, // 32-bit object file
    ELFCLASS64 = 2  // 64-bit object file
};

// Object file byte orderings.
enum {
    ELFDATANONE = 0, // Invalid data encoding.
    ELFDATA2LSB = 1, // Little-endian object file
    ELFDATA2MSB = 2  // Big-endian object file
};

// OS ABI identification.
enum {
    ELFOSABI_NONE = 0,          // UNIX System V ABI
    ELFOSABI_HPUX = 1,          // HP-UX operating system
    ELFOSABI_NETBSD = 2,        // NetBSD
    ELFOSABI_GNU = 3,           // GNU/Linux
    ELFOSABI_LINUX = 3,         // Historical alias for ELFOSABI_GNU.
    ELFOSABI_HURD = 4,          // GNU/Hurd
    ELFOSABI_SOLARIS = 6,       // Solaris
    ELFOSABI_AIX = 7,           // AIX
    ELFOSABI_IRIX = 8,          // IRIX
    ELFOSABI_FREEBSD = 9,       // FreeBSD
    ELFOSABI_TRU64 = 10,        // TRU64 UNIX
    ELFOSABI_MODESTO = 11,      // Novell Modesto
    ELFOSABI_OPENBSD = 12,      // OpenBSD
    ELFOSABI_OPENVMS = 13,      // OpenVMS
    ELFOSABI_NSK = 14,          // Hewlett-Packard Non-Stop Kernel
    ELFOSABI_AROS = 15,         // AROS
    ELFOSABI_FENIXOS = 16,      // FenixOS
    ELFOSABI_C6000_ELFABI = 64, // Bare-metal TMS320C6000
    ELFOSABI_C6000_LINUX = 65,  // Linux TMS320C6000
    ELFOSABI_ARM = 97,          // ARM
    ELFOSABI_STANDALONE = 255   // Standalone (embedded) application
};


// 段表
struct Elf32_Shdr {
    Elf32_Word sh_name;      // 段名，位于 .shstrtab 的字符串表。sh_name 是段名在其中的偏移
    Elf32_Word sh_type;      // 段类型（SHT_*）
    Elf32_Word sh_flags;     // 段标志位（SHF_*）
    Elf32_Addr sh_addr;      // 段的虚拟地址，前提是该段可被加载，否则为 0
    Elf32_Off  sh_offset;    // 段偏移，前提是该段存在于文件中，否则无意义
    Elf32_Word sh_size;      // 段的长度
    Elf32_Word sh_link;      // 段的链接信息
    Elf32_Word sh_info;      // 段的额外信息
    Elf32_Word sh_addralign; // 段地址对齐
    Elf32_Word sh_entsize;   // 项的长度
};


struct Elf64_Shdr {
    Elf64_Word  sh_name;
    Elf64_Word  sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr  sh_addr;
    Elf64_Off   sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word  sh_link;
    Elf64_Word  sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
};

// Special section indices.
enum {
    SHN_UNDEF = 0,          // Undefined, missing, irrelevant, or meaningless
    SHN_LORESERVE = 0xff00, // Lowest reserved index
    SHN_LOPROC = 0xff00,    // Lowest processor-specific index
    SHN_HIPROC = 0xff1f,    // Highest processor-specific index
    SHN_LOOS = 0xff20,      // Lowest operating system-specific index
    SHN_HIOS = 0xff3f,      // Highest operating system-specific index
    SHN_ABS = 0xfff1,       // Symbol has absolute value; does not need relocation
    SHN_COMMON = 0xfff2,    // FORTRAN COMMON or C external global variables
    SHN_XINDEX = 0xffff,    // Mark that the index is >= SHN_LORESERVE
    SHN_HIRESERVE = 0xffff  // Highest reserved index
};
 
 // Section types.
 enum : unsigned {
     SHT_NULL = 0,                     // No associated section (inactive entry).
     SHT_PROGBITS = 1,                 // Program-defined contents.
     SHT_SYMTAB = 2,                   // Symbol table.
     SHT_STRTAB = 3,                   // String table.
     SHT_RELA = 4,                     // Relocation entries; explicit addends.
     SHT_HASH = 5,                     // Symbol hash table.
     SHT_DYNAMIC = 6,                  // Information for dynamic linking.
     SHT_NOTE = 7,                     // Information about the file.
     SHT_NOBITS = 8,                   // Data occupies no space in the file.
     SHT_REL = 9,                      // Relocation entries; no explicit addends.
     SHT_SHLIB = 10,                   // Reserved.
     SHT_DYNSYM = 11,                  // Symbol table.
     SHT_INIT_ARRAY = 14,              // Pointers to initialization functions.
     SHT_FINI_ARRAY = 15,              // Pointers to termination functions.
     SHT_PREINIT_ARRAY = 16,           // Pointers to pre-init functions.
     SHT_GROUP = 17,                   // Section group.
     SHT_SYMTAB_SHNDX = 18,            // Indices for SHN_XINDEX entries.
     SHT_LOOS = 0x60000000,            // Lowest operating system-specific type.
     SHT_GNU_ATTRIBUTES = 0x6ffffff5,  // Object attributes.
     SHT_GNU_HASH = 0x6ffffff6,        // GNU-style hash table.
     SHT_GNU_verdef = 0x6ffffffd,      // GNU version definitions.
     SHT_GNU_verneed = 0x6ffffffe,     // GNU version references.
     SHT_GNU_versym = 0x6fffffff,      // GNU symbol versions table.
     SHT_HIOS = 0x6fffffff,            // Highest operating system-specific type.
     SHT_LOPROC = 0x70000000,          // Lowest processor arch-specific type.
     SHT_ARM_EXIDX = 0x70000001U,      // Exception Index table
     SHT_ARM_PREEMPTMAP = 0x70000002U, // BPABI DLL dynamic linking pre-emption map
     SHT_ARM_ATTRIBUTES = 0x70000003U, //  Object file compatibility attributes
     SHT_ARM_DEBUGOVERLAY = 0x70000004U,
     SHT_ARM_OVERLAYSECTION = 0x70000005U,
     SHT_HEX_ORDERED = 0x70000000,     // Link editor is to sort the entries in this section based on their sizes
     SHT_X86_64_UNWIND = 0x70000001,   // Unwind information
     SHT_MIPS_REGINFO = 0x70000006,    // Register usage information
     SHT_MIPS_OPTIONS = 0x7000000d,    // General options
     SHT_MIPS_ABIFLAGS = 0x7000002a,   // Abiflags options
     SHT_HIPROC = 0x7fffffff,          // Highest processor arch-specific type.
     SHT_LOUSER = 0x80000000,          // Lowest type reserved for applications.
     SHT_HIUSER = 0xffffffff           // Highest type reserved for applications.
 };

 // Section flags.
 enum : unsigned {
     // Section data should be writable during execution.
     SHF_WRITE = 0x1,

     // Section occupies memory during program execution.
     SHF_ALLOC = 0x2,

     // Section contains executable machine instructions.
     SHF_EXECINSTR = 0x4,

     // The data in this section may be merged.
     SHF_MERGE = 0x10,

     // The data in this section is null-terminated strings.
     SHF_STRINGS = 0x20,

     // A field in this section holds a section header table index.
     SHF_INFO_LINK = 0x40U,

     // Adds special ordering requirements for link editors.
     SHF_LINK_ORDER = 0x80U,

     // This section requires special OS-specific processing to avoid incorrect
     // behavior.
     SHF_OS_NONCONFORMING = 0x100U,

     // This section is a member of a section group.
     SHF_GROUP = 0x200U,

     // This section holds Thread-Local Storage.
     SHF_TLS = 0x400U,

     // This section is excluded from the final executable or shared library.
     SHF_EXCLUDE = 0x80000000U,

     // Start of target-specific flags.

     /// XCORE_SHF_CP_SECTION - All sections with the "c" flag are grouped
     /// together by the linker to form the constant pool and the cp register is
     /// set to the start of the constant pool by the boot code.
     XCORE_SHF_CP_SECTION = 0x800U,

     /// XCORE_SHF_DP_SECTION - All sections with the "d" flag are grouped
     /// together by the linker to form the data section and the dp register is
     /// set to the start of the section by the boot code.
     XCORE_SHF_DP_SECTION = 0x1000U,

     SHF_MASKOS = 0x0ff00000,

     // Bits indicating processor-specific flags.
     SHF_MASKPROC = 0xf0000000,

     // If an object file section does not have this flag set, then it may not hold
     // more than 2GB and can be freely referred to in objects using smaller code
     // models. Otherwise, only objects using larger code models can refer to them.
     // For example, a medium code model object can refer to data in a section that
     // sets this flag besides being able to refer to data in a section that does
     // not set it; likewise, a small code model object can refer only to code in a
     // section that does not set this flag.
     SHF_X86_64_LARGE = 0x10000000,

     // All sections with the GPREL flag are grouped into a global data area
     // for faster accesses
     SHF_HEX_GPREL = 0x10000000,

     // Section contains text/data which may be replicated in other sections.
     // Linker must retain only one copy.
     SHF_MIPS_NODUPES = 0x01000000,

     // Linker must generate implicit hidden weak names.
     SHF_MIPS_NAMES = 0x02000000,

     // Section data local to process.
     SHF_MIPS_LOCAL = 0x04000000,

     // Do not strip this section.
     SHF_MIPS_NOSTRIP = 0x08000000,

     // Section must be part of global data area.
     SHF_MIPS_GPREL = 0x10000000,

     // This section should be merged.
     SHF_MIPS_MERGE = 0x20000000,

     // Address size to be inferred from section entry size.
     SHF_MIPS_ADDR = 0x40000000,

     // Section data is string data by default.
     SHF_MIPS_STRING = 0x80000000
 };

 // Section Group Flags
 enum : unsigned {
     GRP_COMDAT = 0x1,
     GRP_MASKOS = 0x0ff00000,
     GRP_MASKPROC = 0xf0000000
 };

// 符号信息
struct Elf32_Sym {
    Elf32_Word    st_name;  // 符号名字，包含了该符号名在字符串表中的下标
    Elf32_Addr    st_value; // 符号相对应的值，是一个绝对值，或地址等。不同的符号，含义不同
    Elf32_Word    st_size;  // 符号的大小
    unsigned char st_info;  // 符号的类型和绑定信息
    unsigned char st_other; // 目前为 0，保留
    Elf32_Half    st_shndx; // 符号所在段的下标
};

struct Elf64_Sym {
    Elf64_Word      st_name;  // Symbol name (index into string table)
    unsigned char   st_info;  // Symbol's type and binding attributes
    unsigned char   st_other; // Must be zero; reserved
    Elf64_Half      st_shndx; // Which section (header tbl index) it's defined in
    Elf64_Addr      st_value; // Value or address associated with the symbol
    Elf64_Xword     st_size;  // Size of the symbol
};

// Relocation entry, without explicit addend.
struct Elf32_Rel {
    Elf32_Addr r_offset; // Location (file byte offset, or program virtual addr)
    Elf32_Word r_info;   // Symbol table index and type of relocation to apply
};

// Relocation entry with explicit addend.
struct Elf32_Rela {
    Elf32_Addr  r_offset; // Location (file byte offset, or program virtual addr)
    Elf32_Word  r_info;   // Symbol table index and type of relocation to apply
    Elf32_Sword r_addend; // Compute value for relocatable field by adding this
};

// Relocation entry, without explicit addend.
struct Elf64_Rel {
    Elf64_Addr  r_offset; // Location (file byte offset, or program virtual addr).
    Elf64_Xword r_info;   // Symbol table index and type of relocation to apply.
};

// Relocation entry with explicit addend.
struct Elf64_Rela {
    Elf64_Addr   r_offset; // Location (file byte offset, or program virtual addr).
    Elf64_Xword  r_info;   // Symbol table index and type of relocation to apply.
    Elf64_Sxword r_addend; // Compute value for relocatable field by adding this.
};

#endif // !ELF_H
