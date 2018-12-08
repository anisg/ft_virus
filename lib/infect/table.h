#ifndef table
# define table

typedef struct s_garbage {
	unsigned int off;
	unsigned int len;
} /*__attribute__((packed))*/ Garbage;

extern Garbage garbage_table[];
extern unsigned int garbage_table_len;

typedef struct s_modified {
	unsigned int off;
	unsigned int len;
	unsigned int num;
} /*__attribute__((packed))*/ Modif;

extern Modif modified_table[];
extern unsigned int modified_table_len;


//extern unsigned int virus_shellcode_len;

#endif
