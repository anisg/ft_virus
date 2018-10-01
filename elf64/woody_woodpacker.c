#include "lib/lib.h"
#include "virus_shellcode.h"
#include "lib/infect.h"
#include "lib/elf64.h"

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

int woody_woodpacker(char *bin_to_pack){
	size_t text_pos = elf_offset_entry(virus_shellcode, virus_shellcode_len);
	randomize(KEY);
	infect(bin_to_pack, "woody", virus_shellcode + text_pos, virus_shellcode_len - text_pos);
	return 0;
}

int main(int ac, char **av){
	if (ac < 2){
		return usage(av[0]);
	}
	return woody_woodpacker(av[1]);
}
