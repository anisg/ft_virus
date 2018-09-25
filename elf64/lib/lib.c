#include <stddef.h>
#include "lib.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

asm(R"(
	.globl call
	.type func, @function
	call:
		.cfi_startproc
		push %rbp
		mov %rsp, %rbp
		mov %rcx, %rax
		mov %r8, %r10
		mov %r9, %r8
		mov 0x10(%rbp), %r9
		syscall
		leave
		ret
		.cfi_endproc
)");

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

//==================== LOW FUNCTIONS =====================

void exit(int n){
	CALL1(EXIT, n);
	__builtin_unreachable();
}

void *malloc(size_t size){
	size_t *p = (void*)CALL(MMAP, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
	p[0] = size; //storing the size
	return p+sizeof(size_t);
}

void free(void *p){
	CALL2(MUNMAP, ((size_t)p)-sizeof(size_t), ((size_t*)(p-1))[0]);
}

ssize_t write(int fd, const void *s, size_t n){
	return CALL(WRITE, fd, (size_t)s, n);
}

ssize_t read(int fd, const void *s, size_t n){
	return CALL(WRITE, fd, (size_t)s, n);
}

int close(int fd){
	return CALL1(CLOSE, fd);
}

int open(const char *filename, int flag, int mode){  
	return CALL(OPEN, (size_t)filename, flag, mode);
}

off_t lseek(int fd, off_t offset, int whence){
	return CALL(LSEEK, fd, offset, whence);
}

//==================== HIGHER FUNCTIONS =====================

size_t slen(char *s){
	int i;for(i=0;s[i];i++);return i;
}

void print(char *s){
	write(1, s, slen(s));
}

void println(char *s){
	print(s);
	print("\n");
}

int fget(char *filename, char **ptr, size_t *l){
	int       fd;

	if ((fd = open(filename, 0, 0755)) < 0)
		return FALSE;
	//get 
	(*l) = lseek(fd, 0, 2);
	if (((*ptr) = (void*)CALL(MMAP, NULL, *l, 3, 2, fd, 0)) == (void*)-1)
		return FALSE;
	close(fd);
	return TRUE;
}

int fput(char *filename, char *ptr, size_t l){
	int fd;

	if ((fd = open(filename, 65, 0755)) < 0)
		return FALSE;
	write(fd, ptr, l);
	close(fd);
	return TRUE;
}

int     str_equal(char *s1, char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return FALSE;
	while (s1[i] && s2[i]
			&& (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]) == 0;
}

void  snbr(size_t nb, char *sn)
{
	int   i;
	int   j;
	char  tmp;

	i = 0;
	while (nb)
	{
		sn[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	sn[i] = 0;
	i -= 1;
	j = 0;
	while (j <= i)
	{
		tmp = sn[j];
		sn[j] = sn[i];
		sn[i] = tmp;
		j += 1;
		i -= 1;
	}
}

void printnb(size_t nb){
	char s[64];
	snbr(nb, s);
	print(s);
}
