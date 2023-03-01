We can see on the main function `__regparam3` it's a convention :

```
regparm (number)
On x86-32 targets, the regparm attribute causes the compiler to pass arguments number one to number if they are of integral type in registers EAX, EDX, and ECX instead of on the stack. Functions that take a variable number of arguments continue to be passed all of their arguments on the stack.

Beware that on some ELF systems this attribute is unsuitable for global functions in shared libraries with lazy binding (which is the default). Lazy binding sends the first call via resolving code in the loader, which might assume EAX, EDX and ECX can be clobbered, as per the standard calling conventions. Solaris 8 is affected by this. Systems with the GNU C Library version 2.1 or higher and FreeBSD are believed to be safe since the loaders there save EAX, EDX and ECX. (Lazy binding can be disabled with the linker or the loader if desired, to avoid the problem.)
```
> So the params will be pass in EAX, EDX and ECX and not on the stack

If we look at the source code we see that le password read and pass to test

if the password - 0x1337d00d == 21 (0x15)

we enter un a special condition that decrypt the Q}|u`sfg~sf{}|a3 by doing str[i] ^= 0x15 so strcmp(v4, "Congratulations!") == 0 and we execute the shell

The value to substract is 0x1337d00d (322424845)  322424845 - 322424824 = 21 (0x15)

```s
0x08048759 <+18>:    mov    DWORD PTR [ebp-0xc],eax
0x0804875c <+21>:    cmp    DWORD PTR [ebp-0xc],0x15
```

So on this line it should match

```sh
(gdb) x $ebp-0xc
0xffffd6cc:     0x00000015
```

it enter in the function with the good value but don't work so I did a script to test values around :
```py
for i in range(322424814, 322425824):
    p = subprocess.Popen(["echo", "-ne", str(i)], stdout=subprocess.PIPE)
    result = subprocess.check_output(["/home/users/level03/level03"], stdin=p.stdout)
    p.wait()
    print(i)
    if not result.split('\n')[4] == 'Invalid Password':
        print(i, result)
```
My script crash at the index 322424827 there is no `result.split('\n')[4]`

We test it :

```sh
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
