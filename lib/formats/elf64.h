#ifndef ELF64_H
# define ELF64_H

#include <stdint.h>
#include <stddef.h>
#include "ft_lib.h"
#include <elf.h>

uint64_t fail(char *s);

void elf_shift_offset(void *s, uint64_t n, uint64_t pos, uint64_t add);

uint64_t elf_off_text_section(void *s, uint64_t n);

uint64_t elf_addr_text_section(void *s, uint64_t n);
uint64_t elf_size_text_section(void *s, uint64_t n);
uint64_t elf_offset_entry(char *s, uint64_t n);
void elf_update_flags_of_load_segments(char *s, uint64_t n);
int elf_last_load_segment(char *s, uint64_t n);
int elf_first_load_segment(void *s, uint64_t n);

uint64_t elf_offset_to_addr(void *s, uint64_t n, uint64_t off);

uint64_t elf_addr_to_offset(void *s, uint64_t n, uint64_t addr);
uint64_t elf_offset_after_last_load_segment(void *s, uint64_t n);
void elf_change_size_last_load_segment(void *s, uint64_t n, int add);
int is_elf(char *s, uint64_t n);

int elf_set_off_entry(char *s, uint64_t n, uint64_t off_entry);

int elf_check_valid(char *s, uint64_t n);
int elf_off_symbol(char *s, uint64_t n, char *name, int64_t *x);
int elf_get_tabs(char *s, uint64_t n, Elf64_Shdr **symtab, Elf64_Shdr **strtab);

#endif
