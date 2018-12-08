#include <stdint.h>
#include <stdlib.h>
#include "ins.h"
#include "poly.h"
#include "ft_lib.h"

#define MAX 3

size_t edit_ins(unsigned char *ins, size_t len, size_t real);
void generate_garb_end(void);
void generate_garb(unsigned char *pos, size_t len, unsigned char *prev)
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
	/*else
	{
		if (prev != NULL && pos - prev < 25)
		{
			i += edit_ins(prev, len - i, pos - prev);
			//pos[i++] = 0xcc;
		}
	}*/
	//debug_ext("jmp ", len - 2, " var ", pos[1], "\n");

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
			size_t to_jump = left - 2;
			if (to_jump > 100)
				to_jump = 100;
			pos[i++] = 0xeb;
			pos[i++] = to_jump;
			for (;to_jump > 0;to_jump--)
			{
				r = ((uint64_t)r * 48271u) % 0x7fffffff;
				pos[i++] = r;
			}
		}
	}

	if (safe == 1)
	{
		pos[0] = 0x90;
		pos[1] = 0x90;
	}
}

int edit_ins_add(unsigned char *ins, int *register_nb, uint32_t *val, char *sig, int *fixed)
{
	if (*ins >= 0xb8 && *ins < 0xc0)
	{
		*sig = 0;
		*fixed = 1;
		*val = *(uint32_t*)(ins + 1);
		*register_nb = *ins - 0xb8;
		return 5;
	}
	if ((*ins == 0x81 && ((*(ins + 1)) & 0b11111000) == 0b11000000)
		|| (*ins == 0x81 && ((*(ins + 1)) & 0b11111000) == 0b11101000))
	{
		*sig = ((*(ins + 1)) & 0b00101000) == 0 ? 1 : -1;
		*fixed = 0;
		*register_nb = (*(ins + 1)) & 0b111;
		*val = *(uint32_t*)(ins + 2);
		return 6;
	}
	return 0;
}

int edit_ins_set(unsigned char *ins, int register_nb, uint32_t val, char sig, int fixed, int len)
{
	if (fixed == 1 && len >= 5)
	{
		*ins = 0xb8 + register_nb;
		*(uint32_t*)(ins + 1) = (uint32_t)val;
		return 5;
	}
	else if (fixed == 0 && len >= 6)
	{
		*ins = 0x81;
		*(ins + 1) = register_nb | 0b11000000 | ((sig == -1) ? 0b00101000 : 0);
		*(uint32_t*)(ins + 2) = (uint32_t)val;
		return 6;
	}
	return 0;
}

#define MAX_REG_NB 8

size_t edit_ins(unsigned char *ins, size_t len, size_t real)
{
	debug_ext("IN\n");
	size_t i;
	size_t j;
	struct
	{
		uint32_t val;
		int fixed;
	}reg_stats[MAX_REG_NB] = {{0, 0}};

	int register_nb;
	uint32_t val;
	char sig;
	int fixed;

	int ret;
	int ret2;

	i = 0;

	debug_ext("P1\n");
	while (1)
	{
		if ((ret = edit_ins_add(ins + i, &register_nb, &val, &sig, &fixed)) > 0
				&& register_nb < MAX_REG_NB
				&& (reg_stats[register_nb].fixed == 1 || fixed == 1))
		{
			i += ret;
			if (fixed)
				reg_stats[register_nb].val = val;
			else if (sig == 1)
				reg_stats[register_nb].val += val;
			else if (sig == -1)
				reg_stats[register_nb].val -= val;
			reg_stats[register_nb].fixed = 1;
		}
		else
		{
			if (i >= real && real != 0)
				break;
			debug_ext("LEAVE\n");
			return 0;
		}
	}

	uint32_t rm = ft_rand();
	uint32_t rm_add = ft_rand();
	//debug_ext("metha ", ins, " -> ", rm, "\n");
	debug_ext("P2\n");

	i = 0;
	j = 0;
	while (1)
	{
		if (i >= real)
			break ;
		else if ((ret = edit_ins_add(ins + i, &register_nb, &val, &sig, &fixed)) > 0
				&& register_nb < MAX_REG_NB
				&& (/*reg_stats[register_nb].fixed == 1 || */fixed == 1))
		{
			int64_t r;
			char r_sig = 1;
			if (reg_stats[register_nb].val != 0 && (rm_add & 1) == 1)
				r = rm % reg_stats[register_nb].val;
			else
			{
				while ((uint32_t)(reg_stats[register_nb].val + rm) < (uint32_t)reg_stats[register_nb].val)
					rm >>= 2;
				r = -rm;
			}

			if (r < 0)
			{
				r = r * -1;
				r_sig = -1;
			}

			if ((ret2 = edit_ins_set(ins + real + j, register_nb, r, r_sig, 0, len - j)) > 0)
			{
				j += ret2;
				edit_ins_set(ins + i, register_nb, reg_stats[register_nb].val - r_sig * r, 0, 1, ret);
			}
			else
				edit_ins_set(ins + i, register_nb, reg_stats[register_nb].val, 0, 1, ret);
			i += ret;
		}
		else
		{
			debug_ext("OUT1\n");
			return j;
		}
	}

	debug_ext("OUT2\n");
	return j;
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
