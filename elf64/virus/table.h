#ifndef table
# define table

typedef struct s_garbage {
	unsigned int off;
	unsigned int len;
} Garbage;

extern Garbage garbage_table[];
extern unsigned int garbage_table_len;

//extern unsigned int virus_shellcode_len;

#endif
