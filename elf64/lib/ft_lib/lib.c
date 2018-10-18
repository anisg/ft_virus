#include <stddef.h>
#include "lib.h"

//asm(R"(
//	.globl call
//	.type call, @function
//	call:
//		.cfi_startproc
//		push %rbp
//		mov %rsp, %rbp
//		mov %rcx, %rax
//		mov %r8, %r10
//		mov %r9, %r8
//		mov 0x10(%rbp), %r9
//		syscall
//		leave
//		ret
//		.cfi_endproc
//)");

size_t __attribute__((section (".textearly"))) call(size_t p1, size_t p2, size_t p3, size_t p4, ...)
{
	asm(R"(
			mov %rcx, %rax
			mov %r8, %r10
			mov %r9, %r8
			mov 0x10(%rbp), %r9
			syscall
			leave
			ret
	)");
}

//==================== LOW FUNCTIONS =====================

void __attribute__((section (".textearly"))) *_malloc(size_t size, int flag)
{
	ssize_t *p = (void*)CALL(SYS_mmap, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
	if (SYS_HAVE_FAIL(p))
		return NULL;
	p[0] = size; //storing the size
	return p + sizeof(size_t);
}


void __attribute__((section (".textearly"))) *ft_malloc(size_t size)
{
	return _malloc(size, 34);
}

void *malloc_shared(size_t size)
{
	return _malloc(size, 33);
}

//==================== HIGHER FUNCTIONS =====================

size_t __attribute__((section (".textearly"))) slen(char const *s)
{
	size_t i = 0;
	while (s[i])
		i++;
	return i;
}

void print(const char *s)
{
	ft_write(1, s, slen(s));
}

void println(const char *s)
{
	print(s);
	print("\n");
}

int fget(const char *filename, char **ptr, size_t *l)
{
	int       fd;
	struct stat buf;

	if (ft_xstat(filename, &buf) < 0)
		return FALSE;
	if (!S_ISREG(buf.st_mode))
		return FALSE;
	if ((fd = ft_open(filename, 0, buf.st_mode)) < 0)
		return FALSE;
	(*l) = ft_lseek(fd, 0, 2);
	(*ptr) = (void*)CALL(SYS_mmap, NULL, *l, 3, 2, fd, 0);
	ft_close(fd);
	if (SYS_HAVE_FAIL(*ptr))
		return FALSE;
	return TRUE;
}

int ffree(char *ptr, size_t l)
{
	size_t i = (size_t)CALL2(SYS_munmap, ptr, l);
}

int fput(const char *filename, char *ptr, size_t l)
{
	int fd;
	mode_t m = 0755;	
	struct stat buf;
	if (!(ft_xstat(filename, &buf) < 0)){
		//file exist
		if (!S_ISREG(buf.st_mode)) return FALSE;
		m = buf.st_mode;
	}
	if ((fd = ft_open(filename, 65, m)) < 0)
		return FALSE;
	ft_write(fd, ptr, l);
	ft_close(fd);
	return TRUE;
}