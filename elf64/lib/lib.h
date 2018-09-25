#ifndef LIB_H
# define LIB_H

#include <sys/types.h>

size_t call(size_t p1, size_t p2, size_t p3, size_t sys_num, ...);
// rax      rdi        rsi        rdx        rcx         r8         r9        16(%rbp)
// rax      rdi        rsi        rdx        rax         r10        r8        r9

#define CALL(X,a1,a2,a3,...)		call((size_t)(a1), (size_t)(a2), (size_t)(a3), X, ##__VA_ARGS__)
#define CALL0(X)			CALL(X, 0, 0, 0)
#define CALL1(X,a1)			CALL(X, a1, 0, 0)
#define CALL2(X,a1,a2)			CALL(X, a1, a2, 0)

enum syscall{
	READ = 0, WRITE = 1, OPEN = 2, CLOSE = 3, LSEEK = 8,
	MMAP = 9, MUNMAP = 11,
	EXIT = 60, EXECVE = 59, FORK = 57};

void exit(int status);
void *malloc(size_t size);
void free(void *p);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);
int open(const char *pathname, int flags, int mode);
off_t lseek(int fd, off_t offset, int whence);

#endif
