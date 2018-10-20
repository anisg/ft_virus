#include <sys/ptrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "ft_lib.h"

extern unsigned char   key[16];

int __attribute__((section (".textearly"))) traceme()
{
	int ret = 0;
	int status;
	pid_t fatherpid = ft_getpid();
	pid_t pid;

	if ((pid = ft_fork()) == 0)
	{
		if (ft_ptrace(PTRACE_ATTACH, fatherpid, 0, 0) != 0)
			ret = 1;

		int a = ft_waitpid(fatherpid, &status, WUNTRACED);
		if (a != fatherpid)
			ret = 1;

		if (ft_ptrace(PTRACE_ATTACH, fatherpid, 0, 0) != -1)
			ret = 1;

		ft_exit(ret);
	}
	else if (pid > 0)
	{
		int a = ft_waitpid(pid, &status, 0);
		if (status != 0 || a != pid)
			ret = 1;
		return (ret);
	}
	return (-1);
}

void __attribute__((section (".textearly"))) handler(int signo)
{
	(void)signo;
	//key[0] ^= 0b01110010 << traceme();
}

int __attribute__((section (".textearly"))) breakpoint()
{
	ft_signal(SIGTRAP, handler);
	//asm("int3");
	ft_signal(SIGTRAP, SIG_DFL);
	return (0);
}

// https://0x00sec.org/t/linux-infecting-running-processes/1097
/*int hellodebug()
{
	pid_t p = getppid();
	kill(p, SIGTERM);
}*/

bool __attribute__((section (".textearly"))) checkdebug(void)
{
	if (traceme() || breakpoint())
	{
		//hellodebug();
		return (1);
	}
	return (0);
}
