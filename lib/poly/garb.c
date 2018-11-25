#include <stdint.h>
#include <stdlib.h>
#include "ins.h"
#include "poly.h"
#include "ft_lib.h"

#define MAX 3

void generate_garb_end(void);
void generate_garb(unsigned char *pos, size_t len)
{
	int safe = 0;
	size_t i = 0;
	unsigned r = ft_rand();

	if (pos >= &generate_garb && pos < &generate_garb_end)
		safe = 1;

	if (safe == 1)
	{
		pos[i++] = 0xeb;
		pos[i++] = len - 2;
	}
	debug_ext("jmp ", len - 2, " var ", pos[1], "\n");

	while (i < len)
	{
		size_t left = len - i;
		r = ((uint64_t)r * 48271u) % 0x7fffffff + 1;

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
			r = ((uint64_t)r * 48271u) % 0x7fffffff;
			pos[i++] = r;
		}
	}

	if (safe == 1)
	{
		pos[0] = 0x90;
		pos[1] = 0x90;
	}
}

void ploy_ins_add(char *data, size_t *i, char *ins, size_t ins_size)
{
	size_t j;
	for (j = 0; j < ins_size; j++)
		data[*i + j] = ins[j];
	*i += ins_size;
}

void generate_garb_end(void)
{}
