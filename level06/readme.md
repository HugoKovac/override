C'est tous simple grace au reverse je print les value pour l'auth

```c
printf("%u != %u = %d\n", secret, v4, (int)(secret != v4));
printf("login: %s\n", login);
return secret != v4;//if secret not equal v4 return 1
```

```sh
***********************************
*               level06           *
***********************************
-> Enter Login: testtest
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 0000
0 != 6232054 = 1
login: testtest
```

C'est bon !

```sh
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: testtest
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6232054
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```