// A ELF 32 tools.

#ifndef BC_ELF32
#define BC_ELF32
#define bc_elf_valid_magic (uint8_t[4]){0x7F, 'E', 'L', 'F'}

#include <stdint.h>
#include <stdint.h>

// A ELF system.
typedef struct {
	uint8_t arch;
	uint8_t endian;
	uint8_t elfver;
	uint16_t machine;
} bc_elf_system_t;

enum bc_elf_type {
	bc_elf_type_none = 0,
	bc_elf_type_rel,
	bc_elf_type_exec,
};

enum bc_elf_arch {
	bc_elf_arch_x32 = 1,
	bc_elf_arch_x64 = 2,
};

// A ELF identifier.
typedef struct {
	uint8_t magic[4];
	uint8_t arch;
	uint8_t endian;
	uint8_t elfver;
	uint8_t abi;
	uint8_t abiver;
	uint8_t padding[7];
} bc_elf_hident_t;

// A main ELF header.
typedef struct {
	bc_elf_hident_t ident;
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint32_t entry;
	uint32_t phoff;
	uint32_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstridx;
} bc_elf_header_t;

// A Section Table entry.
typedef struct {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;
} bc_elf_secent_t;

// A Symbol Table entry.
typedef struct {
	uint32_t name;
	uint32_t addr;
	uint32_t size;
	uint8_t info;
	uint8_t other;
	uint16_t shndx;
} bc_elf_symtabent_t;

// Returns:
// 0 - magic is OK.
// n - byte n-1 is incorrect.
uint8_t bc_elf_check_magic(bc_elf_header_t* hdr);
// Retruns:
// 0 - Header is supported.
// 1 - Usupported arch.
// 2 - Usupported endian.
// 3 - Usupported ELF version.
// 4 - Usupported machine.
uint8_t bc_elf_check_hdr(bc_elf_header_t* hdr, bc_elf_system_t* valid_sys);
bc_elf_secent_t* bc_elf_sectab(bc_elf_header_t* hdr);
bc_elf_secent_t* bc_elf_secent(bc_elf_header_t* hdr, int idx);
char* bc_elf_strname(void* strtab, uint32_t nameoff);
void* bc_elf_secptr(bc_elf_header_t* hdr, int idx);
bc_elf_secent_t* bc_elf_find_secname(bc_elf_header_t* hdr, char* secname, int* idx);
bc_elf_symtabent_t* bc_elf_symtabent(bc_elf_header_t* hdr, void* tab, int idx);

#endif
