#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <math.h>
#include "init.h"

int HKDF(const char * Ks,const char *Ns, int p_i, char *Kfinal[HASH_SIZE]);
int Base64Encode(const char *m, char **buffer);
int Base64Decode(char *m, char **buffer);
void sha256(const char *m, char **hash);
void md5(const char *m, char **hash);
void hash_function(const char *m, char **hash);
