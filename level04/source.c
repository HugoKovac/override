#include <stdint.h>

int32_t main(int32_t argc, char **argv, char **envp)
{
	pid_t pid = fork();
	void var_a0;
	void *edi = &var_a0;

	for (int32_t ecx = 0x20; ecx != 0; ecx = (ecx - 1))
	{
		*edi = 0;
		edi = (edi + 4);
	}

	int32_t var_a4 = 0;
	if (pid == 0)
	{
		prctl(PR_SET_DUMPABLE, PR_DUMPABLE);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(&var_a0);
	}

	else
	{
		while (true)
		{
			wait(&var_a4);
			if (((var_a4 & 0x7f) != 0 && (((var_a4 & 0x7f) + 1) >> 1) <= 0))
			{
				if (ptrace(PTRACE_PEEKUSER, pid, 0x2c, 0) == 0xb)
				{
					puts("no exec() for you");
					kill(pid, 9);
					break;
				}
				continue;
			}
			puts("child is exiting...");
			break;
		}
	}
	return 0;
}
