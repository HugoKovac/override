#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int i;
	size_t s;
	long l;
	char *buf;
	FILE *fp;

	// Initialize buffers with null bytes
	char password[12] = {0};
	char read_pass[6] = {0};
	char username[14] = {0};

	memset(username, 0, 12);

	memset(password, 0, 12);

	memset(read_pass, 0, 6);

	// Open password file
	fp = fopen("./.pass", "r");
	if (fp == NULL)
	{
		fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
		exit(1);
	}

	// Read password from file
	s = fread(read_pass, 1, 0x29, fp);
	if (s != 0x29)
	{
		fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
		exit(1);
	}

	// Remove newline character from password
	buf = strpbrk(read_pass, "\n");
	*buf = '\0';

	fclose(fp);

	// Prompt for username and password
	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/");

	printf("--[ Username: ");
	fgets(username, 100, stdin);
	buf = strpbrk(username, "\n");
	printf("username = %p | buf = %p\n", username, buf);
	if (buf)
		*buf = '\0';

	printf("--[ Password: ");
	fgets(password, 100, stdin);
	buf = strpbrk(password, "\n");
	if (buf)
		*buf = '\0';

	puts("*****************************************");

	printf("read_pass = %s | password = %s\n", read_pass, password);

	// Check if the password is correct
	i = strncmp(read_pass, password, 0x29);
	if (i == 0)
	{
		printf("Greetings, %s!\n", username);
		system("/bin/sh");
		return 0;
	}

	printf((char *)username);
	puts(" does not have access!");
	exit(1);
}
