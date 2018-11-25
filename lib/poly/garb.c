#include <stdint.h>
#include <stdlib.h>
#include "ins.h"
#include "poly.h"
#include "ft_lib.h"

#define MAX 3

void generate_garb(unsigned char *pos, size_t len)
{
	pos[0] = 0xeb;
	pos[1] = len - 2;
	debug_ext("jmp ", len - 2, " var ", pos[1], "\n");

	size_t i = 2;
	while (i < len)
	{
		debug_ext("val, ", i, "/", len, "\n");
		size_t left = len - i;
		unsigned r = ft_rand();

		if (r % MAX == 0)
		{
			if (left >= sizeof(INS_NOP9) - 1)
				ploy_ins_add(pos, &i, INS_NOP9, sizeof(INS_NOP9) - 1);
			else if (left >= sizeof(INS_NOP8) - 1)
				ploy_ins_add(pos, &i, INS_NOP8, sizeof(INS_NOP8) - 1);
			else if (left >= sizeof(INS_NOP7) - 1)
				ploy_ins_add(pos, &i, INS_NOP7, sizeof(INS_NOP7) - 1);
			else if (left >= sizeof(INS_NOP6) - 1)
				ploy_ins_add(pos, &i, INS_NOP6, sizeof(INS_NOP6) - 1);
			else if (left >= sizeof(INS_NOP5) - 1)
				ploy_ins_add(pos, &i, INS_NOP5, sizeof(INS_NOP5) - 1);
			else if (left >= sizeof(INS_NOP4) - 1)
				ploy_ins_add(pos, &i, INS_NOP4, sizeof(INS_NOP4) - 1);
			else if (left >= sizeof(INS_NOP3) - 1)
				ploy_ins_add(pos, &i, INS_NOP3, sizeof(INS_NOP3) - 1);
			else if (left >= sizeof(INS_NOP2) - 1)
				ploy_ins_add(pos, &i, INS_NOP2, sizeof(INS_NOP2) - 1);
			else if (left >= sizeof(INS_NOP) - 1)
				ploy_ins_add(pos, &i, INS_NOP, sizeof(INS_NOP) - 1);
		}
		if (r % MAX == 2 && left >= 3)
		{
			pos[i++] = 0xeb;
			pos[i++] = 1;
			pos[i++] = ft_rand();
		}
	}
	pos[0] = 0x90;
	pos[1] = 0x90;
}
