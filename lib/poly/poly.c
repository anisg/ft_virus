#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "ins.h"
#include "ft_lib.h"

void ploy_ins_add(char *data, size_t *i, char *ins, size_t ins_size)
{
	size_t j;
	for (j = 0; j < ins_size; j++)
		data[*i + j] = ins[j];
	*i += ins_size;
}

void ploy_ins_add_end(char *data, size_t *i, char *ins, size_t ins_size)
{
	*i -= ins_size;
	ploy_ins_add(data, i, ins, ins_size);
	*i -= ins_size;
}

int poly_generate(void **crypt, void **decrypt)
{
	const size_t data_size = 1024;
	//*crypt = mmap(NULL, data_size,
	//		PROT_READ | PROT_WRITE | PROT_EXEC,
	//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//*decrypt = mmap(NULL, data_size,
	//		PROT_READ | PROT_WRITE | PROT_EXEC,
	//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//if (*decrypt == MAP_FAILED || *crypt == MAP_FAILED)
	//{
	//	if (*crypt != MAP_FAILED)
	//		munmap(*crypt, data_size);
	//	if (*decrypt != MAP_FAILED)
	//		munmap(*decrypt, data_size);
	//	printf("[-] map error\n");
	//	return 1;
	//}

	size_t i = 0;
	size_t j = 0;
	//ploy_ins_add(*crypt, &i, INS_PUSH_RBP, sizeof(INS_PUSH_RBP) - 1);
	//ploy_ins_add(*crypt, &i, INS_MOV_RSP_RBP, sizeof(INS_MOV_RSP_RBP) - 1);
	ploy_ins_add(*crypt, &i, INS_MOV_RCX_RSI, sizeof(INS_MOV_RCX_RSI) - 1);
	ploy_ins_add(*crypt, &i, INS_K1_R8, sizeof(INS_K1_R8) - 1);
	ploy_ins_add(*crypt, &i, INS_K2_R9, sizeof(INS_K2_R9) - 1);

	//ploy_ins_add(*decrypt, &j, INS_PUSH_RBP, sizeof(INS_PUSH_RBP) - 1);
	//ploy_ins_add(*decrypt, &j, INS_MOV_RSP_RBP, sizeof(INS_MOV_RSP_RBP) - 1);
	ploy_ins_add(*decrypt, &j, INS_MOV_RCX_RSI, sizeof(INS_MOV_RCX_RSI) - 1);
	ploy_ins_add(*decrypt, &j, INS_K1_R8, sizeof(INS_K1_R8) - 1);
	ploy_ins_add(*decrypt, &j, INS_K2_R9, sizeof(INS_K2_R9) - 1);
	debug_ext("END", 0, "\n");

	#define NB_ELEM 14
	#define INS_MAX 120
	const size_t start_loop = i;
	const size_t start_loop2 = j;
	size_t end_loop2 = j + INS_MAX;
	j = end_loop2;
	//ploy_ins_add(crypt, &i, INS_INT3, sizeof(INS_INT3) - 1);
	int use_key0 = 0;
	int use_key1 = 0;
	while (1)
	{
		const size_t cryptrequest = start_loop + INS_MAX - i;
		const size_t decryptrequest = -start_loop2 + end_loop2;
		if (cryptrequest == 0 && decryptrequest == 0)
			break;
		if (cryptrequest == 0)
		{
			ploy_ins_add_end(*decrypt, &end_loop2, INS_NOP, sizeof(INS_NOP) - 1);
			continue;
		}
		if (decryptrequest == 0)
		{
			ploy_ins_add(*crypt, &i, INS_NOP, sizeof(INS_NOP) - 1);
			continue;
		}

		const size_t keep = (cryptrequest < decryptrequest) ? cryptrequest : decryptrequest;

		const int r = ft_rand();
		if (r % NB_ELEM == 0)
			ploy_ins_add(*crypt, &i, INS_NOP, sizeof(INS_NOP) - 1);
		else if (r % NB_ELEM == 1)
			ploy_ins_add_end(*decrypt, &end_loop2, INS_NOP, sizeof(INS_NOP) - 1);
		else if (r % NB_ELEM == 2 && keep > sizeof(INS_INCB_RDI) - 1)
		{
			ploy_ins_add(*crypt, &i, INS_INCB_RDI, sizeof(INS_INCB_RDI) - 1);
			ploy_ins_add_end(*decrypt, &end_loop2, INS_DECB_RDI, sizeof(INS_DECB_RDI) - 1);
		}
		else if (r % NB_ELEM == 3 && keep > sizeof(INS_DECB_RDI) - 1)
		{
			ploy_ins_add(*crypt, &i, INS_DECB_RDI, sizeof(INS_DECB_RDI) - 1);
			ploy_ins_add_end(*decrypt, &end_loop2, INS_INCB_RDI, sizeof(INS_INCB_RDI) - 1);
		}
		else if (r % NB_ELEM == 4 && keep > sizeof(INS_ROLB_1) - 1)
		{
			ploy_ins_add(*crypt, &i, INS_ROLB_1, sizeof(INS_ROLB_1) - 1);
			ploy_ins_add_end(*decrypt, &end_loop2, INS_RORB_1, sizeof(INS_RORB_1) - 1);
		}
		else if (r % NB_ELEM == 5 && keep > sizeof(INS_ROLB_1) - 1)
		{
			ploy_ins_add(*crypt, &i, INS_RORB_1, sizeof(INS_RORB_1) - 1);
			ploy_ins_add_end(*decrypt, &end_loop2, INS_ROLB_1, sizeof(INS_ROLB_1) - 1);
		}
		else if (r % NB_ELEM == 6 && keep > sizeof(INS_ROLB_I) - 1 + 1)
		{
			ploy_ins_add(*crypt, &i, INS_ROLB_I, sizeof(INS_ROLB_I) - 1);
			const char v = (char)ft_rand();
			((char*)*crypt)[i++] = v;
			((char*)*decrypt)[--end_loop2] = v;
			ploy_ins_add_end(*decrypt, &end_loop2, INS_RORB_I, sizeof(INS_RORB_I) - 1);
		}
		else if (r % NB_ELEM == 7 && keep > sizeof(INS_ROLB_I) - 1 + 1)
		{
			ploy_ins_add(*crypt, &i, INS_RORB_I, sizeof(INS_RORB_I) - 1);
			const char v = (char)ft_rand();
			((char*)*crypt)[i++] = v;
			((char*)*decrypt)[--end_loop2] = v;
			ploy_ins_add_end(*decrypt, &end_loop2, INS_ROLB_I, sizeof(INS_ROLB_I) - 1);
		}
		else if (r % NB_ELEM == 8 && keep > sizeof(INS_XORB_I) - 1 + 1)
		{
			ploy_ins_add(*crypt, &i, INS_XORB_I, sizeof(INS_XORB_I) - 1);
			const int v = ft_rand();
			((char*)(*crypt))[i++] = (char)v;
			((char*)(*decrypt))[--end_loop2] = (char)v;
			ploy_ins_add_end(*decrypt, &end_loop2, INS_XORB_I, sizeof(INS_XORB_I) - 1);
		}
		else if (r % NB_ELEM == 9 && keep > sizeof(INS_XORB_CL) - 1 + 1)
		{
			ploy_ins_add(*crypt, &i, INS_XORB_I, sizeof(INS_XORB_CL) - 1);
			ploy_ins_add_end(*decrypt, &end_loop2, INS_XORB_I, sizeof(INS_XORB_CL) - 1);
		}
		else if (r % NB_ELEM == 10 && keep > sizeof(INS_XORB_CH) - 1 + 1)
		{
			ploy_ins_add(*crypt, &i, INS_XORB_CH, sizeof(INS_XORB_CH) - 1);
			ploy_ins_add_end(*decrypt, &end_loop2, INS_XORB_CH, sizeof(INS_XORB_CH) - 1);
		}
		else if ((r % NB_ELEM == 11 || r % NB_ELEM == 12) && keep > sizeof(INS_XORB_R8) - 1 + sizeof(INS_ROR_R8) - 1)
		{
			if (use_key0 == 0)
			{
				use_key0 = 1;
				ploy_ins_add(*crypt, &i, INS_XORB_R8, sizeof(INS_XORB_R8) - 1);
				ploy_ins_add(*crypt, &i, INS_ROR_R8, sizeof(INS_ROR_R8) - 1);
				ploy_ins_add_end(*decrypt, &end_loop2, INS_ROR_R8, sizeof(INS_ROR_R8) - 1);
				ploy_ins_add_end(*decrypt, &end_loop2, INS_XORB_R8, sizeof(INS_XORB_R8) - 1);
			}
			else if (use_key1 == 0)
			{
				use_key1 = 1;
				ploy_ins_add(*crypt, &i, INS_XORB_R9, sizeof(INS_XORB_R9) - 1);
				ploy_ins_add(*crypt, &i, INS_ROR_R9, sizeof(INS_ROR_R9) - 1);
				ploy_ins_add_end(*decrypt, &end_loop2, INS_ROR_R9, sizeof(INS_ROR_R9) - 1);
				ploy_ins_add_end(*decrypt, &end_loop2, INS_XORB_R9, sizeof(INS_XORB_R9) - 1);
			}

		}
	}

	ploy_ins_add(*crypt, &i, INS_INC_RDI, sizeof(INS_INC_RDI) - 1);
	((char*)*crypt)[i++] = 0xe2;
	const size_t jmp_off = start_loop - i - 1;
	((char*)*crypt)[i++] = jmp_off;

	ploy_ins_add(*decrypt, &j, INS_INC_RDI, sizeof(INS_INC_RDI) - 1);
	((char*)*decrypt)[j++] = 0xe2;
	const size_t jmp_off2 = start_loop2 - j - 1;
	((char*)*decrypt)[j++] = jmp_off2;

	//ploy_ins_add(*crypt, &i, INS_LEAVEQ, sizeof(INS_LEAVEQ) - 1);
	ploy_ins_add(*crypt, &i, INS_RETQ, sizeof(INS_RETQ) - 1);
	//ploy_ins_add(*decrypt, &j, INS_LEAVEQ, sizeof(INS_LEAVEQ) - 1);
	ploy_ins_add(*decrypt, &j, INS_RETQ, sizeof(INS_RETQ) - 1);

	return 0;
}
