#include <gmp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B64 1		//1 =  Use B64 encoding, 0 = No encoding
#define ALEA_SIZE 512 	//total size of the alea in bits
#define PRIME_SIZE 256	//the same for prime number
#define z "get_prime"		//key to xor (for obfuscation)
#define s 9							//size of key
#define x(a,i) a^=z[i]	//macro xor

#define MD5 //uncomment to use MD5
//#define SHA256  //uncomment to use SHA256

#ifndef MAX
	#define MAX 2048
#endif

#ifndef HASH_SIZE
	#ifdef MD5		
		#define HASH_SIZE 65
		#define HASH_TYPE 1	
	#else 
		#define HASH_SIZE 65	//number of char of the SHA256
		#define HASH_TYPE 2
	#endif
#endif



