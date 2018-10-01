#ifndef LIB_H
# define LIB_H

#include <sys/types.h>
#include "bool.h"

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
	EXIT = 60, EXECVE = 59, FORK = 57, PTRACE = 101, GETPID = 39, WAIT = 61, GETDENTS = 78};

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

void exit(int status);
void *malloc(size_t size);
int execve(const char *fichier, char *const argv[], char *const envp[]);
pid_t fork(void);
void free(void *p);
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
int close(int fd);
int open(const char *pathname, int flags, int mode);
off_t lseek(int fd, off_t offset, int whence);

size_t slen(const char *s);
void snbr(size_t nbr, char *sn);

void print(const char *s);
void println(const char *s);
void printnb(size_t nb);

int     sncmp(const char *s1, const char *s2, size_t n);
int fget(const char *filename, char **ptr, size_t *l);
int fput(const char *filename, char *ptr, size_t l);
int str_equal(const char *s1, const char *s2);

int is_debbuger_on();
char *getenv(char *k);
#include <dirent.h>

struct linux_dirent {
    long           d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[];
};
int getdir(char *dirname, char **p, size_t *size);

#endif
