#include "lib/lib.h"
#include "lib/infect.c"

int usage(char *name){
	print("usage: ./");
	print(name);
	print("[--key YOUR_PRIVATE_KEY] binary\n\nnote: the binary must be in elf64\n");
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

int woody_woodpacker(char *bin_to_pack, char *k, int force){
	randomize(OLD_KEY);
	int i = 0;

	for (i = 0; i < 16 && k && k[i]; i++)
		KEY[i] = k[i];
	for (; i < 16; i++)
		KEY[i] = OLD_KEY[i];

	create_woody(bin_to_pack, virus_shellcode, virus_shellcode_len, force);
	return 0;
}

int main(int ac, char **av){
	if (ac < 2){
		return usage(av[0]);
	}
	if (ac == 3 && str_equal(av[1], "--force"))
		return woody_woodpacker(av[2], NULL, TRUE);
	if (ac == 4 && str_equal(av[1], "--key"))
		return woody_woodpacker(av[3], av[2], FALSE);
	else
		return woody_woodpacker(av[1], NULL, FALSE);
}
