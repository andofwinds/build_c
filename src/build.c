#include "inc/elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
	FILE* fp;
	long fsz;
	void* buf;
	bc_elf_header_t* hdr;
	bc_elf_system_t sys = {
		bc_elf_arch_x32,
		1,
		1,
		3
	};

	fp = fopen("test_jump", "rb");
	assert(fp != NULL);
	fseek(fp, 0, SEEK_END);
	fsz = ftell(fp) + 1;
	fseek(fp, 0, SEEK_SET);

	buf = malloc(fsz);
	fread(buf, fsz, 1, fp);

	hdr = buf;
	assert(bc_elf_check_hdr(hdr, &sys) == 0);

	printf("SECTIONS\n\n");

	int idx;

	void* shstrtab = bc_elf_secptr(hdr, hdr->shstridx);
	bc_elf_secent_t* symtab_ent = bc_elf_find_secname(hdr, ".symtab", &idx);
	void* symtab = bc_elf_secptr(hdr, idx);
	bc_elf_find_secname(hdr, ".strtab", &idx);
	void* strtab = bc_elf_secptr(hdr, idx);

	bc_elf_secent_t* sec;
	char* secname;

	printf("%2s\t%20s\t%5s\n", "ID", "NAME", "ADDRESS");
	for (int i = 0; i < hdr->shnum; i++) {
		sec = bc_elf_secent(hdr, i);
		secname = bc_elf_strname(shstrtab, sec->name);
		printf("%2d\t%20s\t0x%-5X\n", i, secname, sec->offset);

	}

	printf("---\n%d total.\n", hdr->shnum);
	printf("\nSYMBOLS .symtab\n\n");
	printf("%2s\t%40s\t%5s\n", "ID", "NAME", "ADDRESS");

	bc_elf_symtabent_t* stent;
	char* stentname;
	for (idx = 0; idx < symtab_ent->size/16; idx++) {
		stent = bc_elf_symtabent(hdr, symtab, idx);
		stentname = bc_elf_strname(strtab, stent->name);
		if (!stentname) break;
		printf("%2d\t%40s\t0x%-5X\n", idx, stentname, stent->addr);
	}
	printf("---\n%d total.\n", idx);

	bc_elf_secent_t* ssent = bc_elf_secent(hdr, stent->shndx);
	void (*startptr)(void) = (void*)((long)hdr + stent->addr);
	startptr();


	free(buf);
	return 0;
}
