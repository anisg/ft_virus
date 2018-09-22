
/* 64-bit ELF base types. */
typedef unsigned long long int u64;
typedef long long int s64;
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef char s8;

typedef u64	Elf64_Addr;
typedef u16	Elf64_Half;
typedef s16	Elf64_SHalf;
typedef u64	Elf64_Off;
typedef s32	Elf64_Sword;
typedef u32	Elf64_Word;
typedef u64	Elf64_Xword;
typedef s64	Elf64_Sxword;

/* These constants are for the segment types stored in the image headers */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7               /* Thread local storage segment */
#define PT_LOOS    0x60000000      /* OS-specific */
#define PT_HIOS    0x6fffffff      /* OS-specific */
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff
#define PT_GNU_EH_FRAME

#define EI_NIDENT	16

typedef struct elf64_hdr {
  unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;		/* Entry point virtual address */
  Elf64_Off e_phoff;		/* Program header table file offset */
  Elf64_Off e_shoff;		/* Section header table file offset */
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;

/* These constants define the permissions on sections in the program
   header, p_flags. */
#define PF_R		0x4
#define PF_W		0x2
#define PF_X		0x1

typedef struct elf64_phdr {
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;		/* Segment file offset */
  Elf64_Addr p_vaddr;		/* Segment virtual address */
  Elf64_Addr p_paddr;		/* Segment physical address */
  Elf64_Xword p_filesz;		/* Segment size in file */
  Elf64_Xword p_memsz;		/* Segment size in memory */
  Elf64_Xword p_align;		/* Segment alignment, file & memory */
} Elf64_Phdr;

typedef struct elf64_shdr {
  Elf64_Word sh_name;		/* Section name, index in string tbl */
  Elf64_Word sh_type;		/* Type of section */
  Elf64_Xword sh_flags;		/* Miscellaneous section attributes */
  Elf64_Addr sh_addr;		/* Section virtual addr at execution */
  Elf64_Off sh_offset;		/* Section file offset */
  Elf64_Xword sh_size;		/* Size of section in bytes */
  Elf64_Word sh_link;		/* Index of another section */
  Elf64_Word sh_info;		/* Additional section information */
  Elf64_Xword sh_addralign;	/* Section alignment */
  Elf64_Xword sh_entsize;	/* Entry size if section holds table */
} Elf64_Shdr;


typedef u64 uint64_t;
typedef u32 uint32_t;

uint64_t fail(char *s){
	printf("ERROR:%s\n", s);
	return 0;
}

void _elf_shift_offset_ph(void *s, uint64_t n, uint64_t pos, uint64_t add){
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

void _elf_shift_offset_sh(void *s, uint64_t n, uint64_t pos, uint64_t add){
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

uint64_t elf_size_text_section(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;

	(void)n;
	char *strs = s + (sh[h->e_shstrndx]).sh_offset;
	for (int i = 0; i < h->e_shnum; i += 1){
		if (str_equal(strs+sh[i].sh_name, ".text")){
			return sh[i].sh_size;
		}
	}
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
	return TRUE;
}

int elf_check_valid(char *s, uint64_t n){
	Elf64_Ehdr *h;
	if (n < sizeof(*h))
		return fail("bad header");
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
