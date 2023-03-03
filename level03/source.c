#include <openssl/ossl_typ.h>
#include <stddef.h>
#include <stdbool.h>
#include <defs.h>


/*
EVP_PKEY_CTX is a type from openssl/ossl_typ.h

struct evp_pkey_ctx_st {
    // Method associated with this operation 
    const EVP_PKEY_METHOD *pmeth;
    // Engine that implements this method or NULL if builtin 
    ENGINE *engine;
    // Key: may be NULL 
    EVP_PKEY *pkey;
    // Peer key for key agreement, may be NULL 
    EVP_PKEY *peerkey;
    // Actual operation 
    int operation;
    // Algorithm specific data 
    void *data;
    // Application specific data 
    void *app_data;
    // Keygen callback 
    EVP_PKEY_gen_cb *pkey_gencb;
    // implementation specific keygen data 
    int *keygen_info;
    int keygen_info_count;
} EVP_PKEY_CTX  ;
*/

int __cdecl decrypt(char a1)
{
  unsigned int v3; // [esp+24h] [ebp-24h]
  char v4[29]; // [esp+2Bh] [ebp-1Dh] BYREF

  *(_DWORD *)&v4[17] = __readgsdword(0x14u);
  strcpy(v4, "Q}|u`sfg~sf{}|a3");
  v3 = strlen(v4);
  for (int i = 0; i < v3; ++i )
    v4[i] ^= a1;
  if ( !strcmp(v4, "Congratulations!") )
    return system("/bin/sh");
  else
    return puts("\nInvalid Password");
}

int __cdecl test(int a1, int a2)
{
  int result; // eax
  char v3; // al

  switch ( a2 - a1 )
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      result = decrypt(a2 - a1);
      break;
    default:
      v3 = rand();
      result = decrypt(v3);
      break;
  }
  return result;
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned int v3; // eax
  int savedregs; // [esp+20h] [ebp+0h] BYREF

  v3 = time(0);
  srand(v3);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  __isoc99_scanf("%d", &savedregs);
  test(savedregs, 322424845);
  return 0;
}

