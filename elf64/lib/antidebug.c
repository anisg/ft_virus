#include <sys/ptrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int catch = 42;

void handler(int signo)
{
	catch = 0;
}

int breakpoint()
{
	signal(SIGTRAP, handler);
	asm("int3");
	signal(SIGTRAP, SIG_DFL);
	return (catch);
}

int traceme()
{
	int ret = 0;
	int status;
	pid_t fatherpid = getpid();
	pid_t pid;

	if ((pid = fork()) == 0)
	{
		if (ptrace(PTRACE_ATTACH, fatherpid, 0, 0) != 0)
			ret = 1;

		int a = waitpid(fatherpid, &status, WUNTRACED);
		if (a != fatherpid)
			ret = 1;

		if (ptrace(PTRACE_ATTACH, fatherpid, 0, 0) != -1)
			ret = 1;

		exit(ret);
	}
	else if (pid > 0)
	{
		int a = waitpid(pid, &status, 0);
		if (status != 0 || a != pid)
			ret = 1;
		return (ret);
	}
	return (-1);
}

// https://0x00sec.org/t/linux-infecting-running-processes/1097
/*int hellodebug()
{
	pid_t p = getppid();
	kill(p, SIGTERM);
}*/

int checkdebug(void)
{
	if (traceme() || breakpoint())
	{
		//hellodebug();
		return (1);
	}
	return (0);
}
