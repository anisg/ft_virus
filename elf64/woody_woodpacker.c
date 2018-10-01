#include "lib/lib.h"
#include "lib/infect.h"

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

extern char KEY[16]; //defined in infect.c

extern unsigned char virus_shellcode[];
extern unsigned int virus_shellcode_len;

int woody_woodpacker(char *bin_to_pack){
	randomize(KEY);
	int i = 0;
	char *k = NULL;

	create_woody(bin_to_pack, virus_shellcode, virus_shellcode_len);
	return 0;
}

int main(int ac, char **av){
	if (ac < 2){
		return usage(av[0]);
	}
	return woody_woodpacker(av[1]);
}
