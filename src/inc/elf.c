#include "elf.h"
#include "bctools.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t bc_elf_check_magic(bc_elf_header_t *hdr) {
	for (int i = 0; i < 4; i++) {
		if (hdr->ident.magic[i] == bc_elf_valid_magic[i]) continue;

		return i+1;
	}

	return 0;
}

uint8_t bc_elf_check_hdr(bc_elf_header_t *hdr, bc_elf_system_t *valid_sys) {
	if (hdr->ident.arch 	!= valid_sys->arch) 		return 1;
	if (hdr->ident.endian != valid_sys->endian) 	return 2;
	if (hdr->ident.elfver != valid_sys->elfver)		return 3;
	if (hdr->machine 			!= valid_sys->machine)	return 4;

	return 0;
}

bc_elf_secent_t* bc_elf_sectab(bc_elf_header_t *hdr) {
	return (bc_elf_secent_t*)((long)hdr + hdr->shoff);
}

bc_elf_secent_t* bc_elf_secent(bc_elf_header_t *hdr, int idx) {
	return &bc_elf_sectab(hdr)[idx];
}

char* bc_elf_strname(void* strtab, uint32_t nameoff) {
	return (char*)((long)strtab + nameoff);
}

void* bc_elf_secptr(bc_elf_header_t *hdr, int idx) {
	return (void*)((long)hdr + bc_elf_secent(hdr, idx)->offset);
}

bc_elf_secent_t* bc_elf_find_secname(bc_elf_header_t* hdr, char* secname, int* idx) {
	void* strtab = bc_elf_secptr(hdr, hdr->shstridx);

	bc_elf_secent_t* sec;
	for (int i = 0; i < hdr->shnum; i++) {
		sec = bc_elf_secent(hdr, i);
		if (strcmp(bc_elf_strname(strtab, sec->name), secname) == 0) {
			if (idx) *idx = i; 	// NULL pointer protection.

			return sec;
		}
	}
	return NULL;
}

bc_elf_symtabent_t* bc_elf_symtabent(bc_elf_header_t* hdr, void* tab, int idx) {
	return &((bc_elf_symtabent_t*)tab)[idx];
}
