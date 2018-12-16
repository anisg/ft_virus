#include <stdint.h>
#include <stdlib.h>
#include "ins.h"
#include "poly.h"
#include "ft_lib.h"

#define MAX_R 3

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

		if (r % MAX_R == 0)
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
		if (r % MAX_R == 2 && left >= 3)
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

int edit_ins_add(unsigned char *ins, uint64_t *val, char *sig, int *fixed, int *size)
{
	int prefixlen = 0;
	int instruction_size = sizeof(uint32_t);

	if (*ins == 0x48)
	{
		prefixlen++;
		instruction_size = sizeof(uint64_t);
		ins++;
	}

	if ((*ins == 0x81 && ((*(ins + 1)) & 0b11111000) == 0b11000000)
		|| (*ins == 0x81 && ((*(ins + 1)) & 0b11111000) == 0b11101000))
	{
		*sig = ((*(ins + 1)) & 0b00101000) == 0 ? 1 : -1;
		*fixed = 0;
		*val = *(uint32_t*)(ins + 2);
		*size = sizeof(uint32_t);
		return prefixlen + sizeof(uint32_t) + 2;
	}
	//if ((*ins == 0x83 && ((*(ins + 1)) & 0b11111000) == 0b11000000)
	//	|| (*ins == 0x83 && ((*(ins + 1)) & 0b11111000) == 0b11101000))
	//{
	//	*sig = ((*(ins + 1)) & 0b00101000) == 0 ? 1 : -1;
	//	*fixed = 0;
	//	*val = *(uint8_t*)(ins + 2);
	//	*size = sizeof(uint8_t);
	//	return prefixlen + sizeof(uint8_t) + 2;
	//}
	if (*ins == 0x05 || *ins == 0x2d)
	{
		*sig = (*ins == 0x05) ? 1 : -1;
		*fixed = 0;
		*val = *(uint32_t*)(ins + 1);
		*size = sizeof(uint32_t);
		return prefixlen + sizeof(uint32_t) + 1;
	}
	if (*ins >= 0xb8 && *ins < 0xc0)
	{
		*sig = 0;
		*fixed = 1;
		if (instruction_size == sizeof(uint64_t))
			*val = *(uint64_t*)(ins + 1);
		else
			*val = *(uint32_t*)(ins + 1);
		*size = instruction_size;
		return prefixlen + instruction_size + 1;
	}
	if (*ins == 0x8d && (*(ins + 1) & 0b11000111) == 0x05)
	{
		*sig = 0;
		*fixed = 1;
		*val = *(uint32_t*)(ins + 2);
		*size = sizeof(uint32_t);
		return prefixlen + sizeof(uint32_t) + 2;
	}
	return 0;
}

int edit_ins_set(unsigned char *ins, uint64_t val, uint8_t sig)
{
	int prefixlen = 0;
	int instruction_size = sizeof(uint32_t);

	if ((*ins & 0b1111000) == 0x48)
	{
		prefixlen++;
		instruction_size = sizeof(uint64_t);
		ins++;
	}
	else if ((*ins & 0b1110000) == 0b1000000)
	{
		prefixlen++;
		ins++;
	}

	if (*ins >= 0xb8 && *ins < 0xc0)
	{
		if (instruction_size == sizeof(uint64_t))
			*(uint64_t*)(ins + 1) = (uint64_t)val;
		else
			*(uint32_t*)(ins + 1) = (uint32_t)val;
		return prefixlen + instruction_size + 1;
	}
	if ((*ins == 0x81 && ((*(ins + 1)) & 0b11111000) == 0b11000000)
		|| (*ins == 0x81 && ((*(ins + 1)) & 0b11111000) == 0b11101000))
	{
		if (sig == -1)
			*(uint8_t*)(ins + 1) |= 0b00101000;
		else if (sig == 1)
			*(uint8_t*)(ins + 1) &= ~0b00111000;
		*(uint32_t*)(ins + 2) = (uint32_t)val;
		return prefixlen + sizeof(uint32_t) + 2;
	}
	if ((*ins == 0x83 && ((*(ins + 1)) & 0b11111000) == 0b11000000)
		|| (*ins == 0x83 && ((*(ins + 1)) & 0b11111000) == 0b11101000))
	{
		if (sig == -1)
			*(uint8_t*)(ins + 1) |= 0b00101000;
		else if (sig == 1)
			*(uint8_t*)(ins + 1) &= ~0b00111000;
		*(uint8_t*)(ins + 2) = (uint8_t)val;
		return prefixlen + sizeof(uint8_t) + 2;
	}

	if (*ins == 0x05 || *ins == 0x2d)
	{
		if (sig == -1)
			*ins = 0x2d;
		else if (sig == 1)
			*ins = 0x05;
		*(uint32_t*)(ins + 1) = (uint32_t)val;
		return prefixlen + sizeof(uint32_t) + 1;
	}
	if (*ins == 0x8d && (*(ins + 1) & 0b11000111) == 0x05)
	{
		*(uint32_t*)(ins + 2) = (uint32_t)val;
		return prefixlen + sizeof(uint32_t) + 2;
	}
	return 0;
}

#define MAX_INS_EDIT 2 // DO NOT EDIT

size_t edit_ins(unsigned char *ins)
{
	debug_ext("IN\n");
	size_t i;
	size_t j;
	struct
	{
		uint64_t val;
		int fixed;
		void *edit_tab[MAX_INS_EDIT];
		int edit_cur;
		uint8_t max;
	}reg_stats = {0, 0, -1, {0}, 0};

	uint64_t val;
	char sig;
	int fixed;
	int size;

	int ret;
	int ret2;

	i = 0;

	debug_ext("P1\n");
	while (1)
	{
		if (reg_stats.edit_cur == MAX_INS_EDIT)
			break;
		else if ((ret = edit_ins_add(ins + i, &val, &sig, &fixed, &size)) > 0)
		{
			if (reg_stats.max == 0 || reg_stats.max > size)
				reg_stats.max = size;
			if (reg_stats.edit_cur == 0 && sig == -1)
				return 0;
			reg_stats.edit_tab[reg_stats.edit_cur++] = ins + i;
			if (fixed)
			{
				reg_stats.val = val;
				reg_stats.fixed = 1;
			}
			else if (sig == 1)
				reg_stats.val += val;
			else if (sig == -1)
				reg_stats.val -= val;
			debug_ext("find: ", *(ins + i), " ", *(ins + i + 1), "\n");
			debug_ext("      ", *(ins + i + 2), " ", *(ins + i + 3), "\n");
			debug_ext("      ", *(ins + i + 4), " ", *(ins + i + 5), "\n");
			debug_ext("      ", *(ins + i + 6), " ", *(ins + i + 7), "\n");
			i += ret;
		}
		else
		{
			debug_ext("bad: ", *(ins + i), " ", *(ins + i + 1), "\n");
			if (reg_stats.edit_cur >= 2)
				break;
			debug_ext("LEAVE\n");
			return 0;
		}
	}

	uint32_t rm = ft_rand();

	uint64_t v = 1 << ((uint64_t)reg_stats.max * 8);
	if (v != 0)
		rm %= v;

	uint32_t rm_add = ft_rand();
	//debug_ext("metha ", ins, " -> ", rm, "\n");
	debug_ext("P2\n");

	int64_t r;
	char r_sig = 1;
	if (reg_stats.val != 0 && (rm_add & 1) == 1)
		r = rm % reg_stats.val;
	else
	{
		while ((uint32_t)(reg_stats.val + rm) < (uint32_t)reg_stats.val)
			rm >>= 2;
		r = rm * -1;
		//r = -rm % reg_stats.val;
	}

	if (r < 0)
	{
		r = r * -1;
		r_sig = -1;
	}

	edit_ins_set(reg_stats.edit_tab[0], reg_stats.val - r_sig * r, 0);
	edit_ins_set(reg_stats.edit_tab[1], r, r_sig);

	debug_ext("OUT2\n");
	return 1;
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
