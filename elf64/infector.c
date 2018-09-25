#include "lib/lib.c"
#include "lib/infect.c"

int usage(char *name){
	print("usage: ./");
	print(name);
	print(" binary\n\nnote: the binary must be in elf64\n");
	return -1;
}


void randomize(char *k){
        int             fd;

        if ((fd = open("/dev/urandom", 0, 0)) == -1)
                return FALSE;
        read(fd, (char *)k, sizeof(*k)*16);
}

extern char KEY[16]; //defined in infect.c

int woody_woodpacker(char *bin_to_pack){
	char *template;
	size_t n;
	fget("bin/virus.template", &template, &n);
	randomize(KEY);
	/*print("KEY IS: ");
        printnb(((uint32_t*)KEY)[0]);print(" ");
        printnb(((uint32_t*)KEY)[1]);print(" ");
        printnb(((uint32_t*)KEY)[2]);print(" ");
        printnb(((uint32_t*)KEY)[3]);print(" ");
        println("");*/
	create_woody(bin_to_pack, template, n);
	return 0;
}

int main(int ac, char **av){
	if (ac < 2){
		return usage(av[0]);
	}
	return woody_woodpacker(av[1]);
}
