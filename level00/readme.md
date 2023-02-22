## Analyse

In the disassembly code we can see that the program perform a scanf and compare this value to $0x149c

```sh
call   0x80483d0 <__isoc99_scanf@plt>
   0x080484e3 <+79>:    mov    0x1c(%esp),%eax
   0x080484e7 <+83>:    cmp    $0x149c,%eax
   0x080484ec <+88>:    jne    0x804850d <main+121>
```
> 0x149c = 5276

if the scanf value == 0x149c it runs `0x08048501 <+109>:   call   0x80483a0 <system@plt>`

## Exploit

```sh
level00@OverRide:~$ ./level00 5276
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
> Found it !