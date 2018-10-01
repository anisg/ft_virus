#include <elf.h>
#include <stddef.h>
#include "bool.h"
#include "lib.h"
#include "elf64.h"

uint64_t fail(char *s){
	//printf("ERROR:%s\n", s);
	print("fail: ");
	println(s);
	return 0;
}

static void _elf_shift_offset_ph(void *s, uint64_t n, uint64_t pos, uint64_t add){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;
	(void)n;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_offset > pos) {
			ph[i].p_offset += add;
			ph[i].p_vaddr += add;
			ph[i].p_paddr += add;
		}
}

static void _elf_shift_offset_sh(void *s, uint64_t n, uint64_t pos, uint64_t add){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;
	(void)n;
	for (int i = 0; i < h->e_shnum; i += 1)
		if (sh[i].sh_offset > pos){
			sh[i].sh_offset += add;
			sh[i].sh_addr += add;
		}
}

void elf_shift_offset(void *s, uint64_t n, uint64_t pos, uint64_t add){
	Elf64_Ehdr *h = s;
	(void)n;
	if (pos < h->e_shoff) h->e_shoff += add;
	if (pos < h->e_phoff) h->e_phoff += add;
	_elf_shift_offset_ph(s, n, pos, add);
	_elf_shift_offset_sh(s, n, pos, add);
}

uint64_t elf_off_text_section(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;

	(void)n;
	char *strs = s + (sh[h->e_shstrndx]).sh_offset;
	for (int i = 0; i < h->e_shnum; i += 1){
		if (str_equal(strs+sh[i].sh_name, ".text")){
			return sh[i].sh_offset;
		}
	}
	return fail("text section not found");
}

uint64_t elf_addr_text_section(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;

	(void)n;
	char *strs = s + (sh[h->e_shstrndx]).sh_offset;
	for (int i = 0; i < h->e_shnum; i += 1){
		if (str_equal(strs+sh[i].sh_name, ".text")){
			return sh[i].sh_addr;
		}
	}
	return fail("text section not found");
}
uint64_t elf_size_text_section(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;

	(void)n;
	//printf("IN2\n");
	char *strs = s + (sh[h->e_shstrndx]).sh_offset;
	for (int i = 0; i < h->e_shnum; i += 1){
		//printf("|%s|\n", strs+sh[i].sh_name);
		if (str_equal(strs+sh[i].sh_name, ".text")){
			return sh[i].sh_size;
		}
	}
	//printf("OUT2\n");
	return fail("text section not found");
}

uint64_t elf_offset_entry(char *s, uint64_t n){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = (void*)s + h->e_phoff;

	(void)n;
	uint64_t entry = h->e_entry;
	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				entry >= ph[i].p_vaddr && entry < ph[i].p_vaddr + ph[i].p_filesz){
			return ph[i].p_offset + (entry - ph[i].p_vaddr);
		}
	}
	return fail("entry is not in the file!");
}

void elf_update_flags_of_load_segments(char *s, uint64_t n){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = (void*)s + h->e_phoff;

	(void)n;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			ph[i].p_flags = PF_X | PF_W | PF_R;
}

int elf_last_load_segment(char *s, uint64_t n){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = (void*)s + h->e_phoff;

	(void)n;
	x = -1;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			x = i;
	if (x == -1) return fail("no load segment in the binary\n");
	return x;
}

int elf_first_load_segment(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;

	(void)n;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			return i;
	return fail("no load segment in the binary\n");
}

uint64_t elf_offset_to_addr(void *s, uint64_t n, uint64_t off){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;

	(void)n;
	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				off >= ph[i].p_offset && off <= ph[i].p_offset + ph[i].p_filesz){
			return ph[i].p_vaddr + (off - ph[i].p_offset);
		}
	}
	return fail("wasn\'t able to convert an offset to an memory address.");
}

uint64_t elf_addr_to_offset(void *s, uint64_t n, uint64_t addr){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;

	(void)n;
	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				addr >= ph[i].p_vaddr && addr <= ph[i].p_vaddr + ph[i].p_memsz){
			return ph[i].p_offset + (addr - ph[i].p_vaddr);
		}
	}
	return fail("wasn\'t able to convert an offset to an memory address.");
}
uint64_t elf_offset_after_last_load_segment(void *s, uint64_t n){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	(void)n;
	x = elf_last_load_segment(s, n);
	return ph[x].p_offset + ph[x].p_filesz;
}

void elf_change_size_last_load_segment(void *s, uint64_t n, int add){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	(void)n;
	x = elf_last_load_segment(s, n);
	ph[x].p_filesz += add;
	ph[x].p_memsz += add;
}

int is_elf(char *s, uint64_t n){
	(void)n;
	return (*((uint32_t*)s) == 0x464c457f) && (s[4] == 2);
}

int elf_set_off_entry(char *s, uint64_t n, uint64_t off_entry){
	Elf64_Ehdr *h = (void*)s;

	h->e_entry = elf_offset_to_addr(s,n,off_entry);
	//printf("addr:%zu\n", h->e_entry);
	return TRUE;
}

int elf_check_valid(char *s, uint64_t n){
	Elf64_Ehdr *h;
	if (n < sizeof(*h))
		return fail("bad header");
	if (!is_elf(s,n))
		return fail("bad magic");
	h = (void*)s;
	if (h->e_shoff >= n)
		return fail("bad header.sh_offset");
	if (h->e_phoff >= n)
		return fail("bad header.ph_offset");

	//sh

	Elf64_Shdr *sh;
	if (h->e_shoff + sizeof(*sh) >= n)
		return fail("bad section header (1)");
	if (h->e_shoff + sizeof(*sh) * h->e_shstrndx >= n)
		return fail("bad strings section header");

	sh = (void*)s + h->e_shoff;
	uint64_t sn = 0;
	char *strs = NULL;
	//check section header
	for (int i = 0; i < h->e_shnum; i += 1){
		if (h->e_shoff + sizeof(*sh) * i >= n || h->e_shoff + sizeof(*sh) * i + sizeof(*sh) > n)
			return fail("bad section header (2)");

		if (i == h->e_shstrndx){
			if (sh[i].sh_offset >= n || sh[i].sh_offset + sh[i].sh_size > n)
				return fail("bad strings section (1)");
			strs = (void*)s + sh[i].sh_offset;
			sn = sh[i].sh_size;
		}
	}
	for (int i = 0; i < h->e_shnum; i += 1){
		if (i != h->e_shstrndx && strs != NULL && sh[i].sh_name + 5 <= sn && str_equal(strs+sh[i].sh_name, ".text")) {
			if (sh[i].sh_offset >= n || sh[i].sh_offset + sh[i].sh_size > n)
				return fail("bad .text section (1)");
		}
	}

	//ph
	Elf64_Phdr *ph;
	if (h->e_phoff + sizeof(*ph) >= n)
		return fail("bad program header (1)");

	ph = (void*)s + h->e_phoff;
	for (int i = 0; i < h->e_phnum; i += 1){
		if (h->e_phoff + sizeof(*ph) * i >= n || h->e_phoff + sizeof(*ph) * i + sizeof(*ph) > n)
			return fail("bad program header (2)");

		if (ph[i].p_offset >= n)
			return fail("bad segment.offset (1)");
		if (ph[i].p_offset + ph[i].p_filesz > n)
			return fail("bad segment.offset (2)");
	}
	return TRUE;
}

int elf_off_symbol(char *s, uint64_t n, char *name, long long *x){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Shdr *sh = (void*)(s + h->e_shoff);
	
	(void)n;
	Elf64_Shdr *symtab = NULL;
	Elf64_Shdr *strtab = NULL;
	char *strs = s + (sh[h->e_shstrndx]).sh_offset;
	for (int i = 0; i < h->e_shnum; i += 1){
		if (str_equal(strs+sh[i].sh_name, ".symtab"))
			symtab = sh + i;
		if (str_equal(strs+sh[i].sh_name, ".strtab"))
			strtab = sh + i;
	}

	if (!strtab || !symtab || (size_t)strtab >= ((size_t)s) + n || (size_t)symtab >= ((size_t)s) + n) return fail("off symbol error");

	Elf64_Sym *sym = s + symtab->sh_offset;
	strs = s + strtab->sh_offset;

	for (size_t i = 0; i < (symtab->sh_size / sizeof(Elf64_Sym *)); i++) {
		//println(strs + sym[i].st_name);
		if (str_equal(name, strs + sym[i].st_name)){
			*x = sym[i].st_value;
			return TRUE;
		}
	}
	return FALSE;
}
