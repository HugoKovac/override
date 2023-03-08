#include <stdbool.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include <string.h>

bool auth(char *login, int secret)
{
	int v4; // [esp+18h] [ebp-10h]
	int login_len; // [esp+1Ch] [ebp-Ch]

	login[strcspn(login, "\n")] = 0;
	login_len = strnlen(login, 32);
	if (login_len <= 5)
		return 1;
	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
	{
		puts("\x1B[32m.---------------------------.");
		puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
		puts("\x1B[32m'---------------------------'");
		return 1;
	}
	else
	{
		v4 = (login[3] ^ 0x1337) + 6221293;
		for (int i = 0; i < login_len; ++i)
		{
			if (login[i] <= 31)
				return 1;
			v4 += (v4 ^ (unsigned int)login[i]) % 0x539;
		}
		// printf("%u != %u = %d\n", secret, v4, (int)(secret != v4));
		// printf("login: %s\n", login);
		return secret != v4;//if secret not equal v4 return 1
	}
}
// 80485D0: using guessed type int __cdecl strnlen(_DWORD, _DWORD);

//----- (08048879) --------------------------------------------------------
int main(int argc, const char **argv, const char **envp)
{
	int secret;			 // [esp+2Ch] [ebp-24h] BYREF
	char login[28];		 // [esp+30h] [ebp-20h] BYREF

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(login, 32, stdin);
	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");
	__isoc99_scanf("%u", &secret);
	if (auth(login, secret))
		return 1;
	puts("Authenticated!");
	system("/bin/sh");
	return 0;
}