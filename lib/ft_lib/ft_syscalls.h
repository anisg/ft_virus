#include "ft_lib.h"

static inline __start void ft_exit(int n)
{
	CALL1(SYS_exit, n);
	__builtin_unreachable();
}

static inline int ft_execve(const char *fichier, char *const argv[], char *const envp[])
{
	return CALL(SYS_execve, fichier, argv, envp);
}

static inline pid_t __start ft_fork(void)
{
	return CALL0(SYS_fork);
}

static inline __start void ft_free(void *p)
{
	size_t *tab = p;
	CALL2(SYS_munmap, &tab[-1], tab[-1]);
}

static inline __start ssize_t ft_write(int fd, const void *s, size_t n)
{
	return CALL(SYS_write, fd, (size_t)s, n);
}

static inline __start ssize_t ft_read(int fd, void *s, size_t n)
{
	return CALL(SYS_read, fd, (size_t)s, n);
}

static inline __start int ft_close(int fd)
{
	return CALL1(SYS_close, fd);
}

static inline __start int ft_open(const char *filename, int flag, int mode)
{
	return CALL(SYS_open, (size_t)filename, flag, mode);
}

static inline off_t ft_lseek(int fd, off_t offset, int whence)
{
	return CALL(SYS_lseek, fd, offset, whence);
}

static inline __start long ft_ptrace(long request, pid_t pid, void* addr, void *data){
	return CALL(SYS_ptrace, request, pid, addr, data);
}

static inline int ft_xstat(const char *filename, struct stat *buf){
	return CALL2(SYS_stat, filename, buf);
}

static inline __start pid_t ft_waitpid(pid_t pid, int *stat_loc, int option){
	return CALL(SYS_wait4, pid, stat_loc, option, NULL);
}

static inline pid_t ft_wait(int *stat_loc){
	return ft_waitpid(-1, stat_loc, 0);
}

static inline __start int getdents(int fd, char *buff, size_t buff_size)
{
	return CALL(SYS_getdents, fd, buff, buff_size);
}

static inline pid_t __start ft_getpid(void)
{
	return CALL0(SYS_getpid);
}

static inline ssize_t ft_pipe(int pipefd[2], int flags)
{
	return CALL2(SYS_pipe, pipefd, flags);
}

static inline ssize_t ft_dup2(int oldfd, int newfd)
{
	return CALL2(SYS_dup2, oldfd, newfd);
}
