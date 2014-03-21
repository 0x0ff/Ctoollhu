#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define PRECISION 10
#ifndef MAX
	#define MAX 1024
#endif

/**** FERMAT Function *********
IN : A very long number N (mpz_t)
OUT : BOOL, 0 = False, 1 = True

DESC :
	while i < PRECISION :
		generate a random a
    if a**(N-1) != 1 mod(N), then return FALSE
  return TRUE
********************************/
int fermat(mpz_t n){
  int i;
  long seed;
  gmp_randstate_t init;
  mpz_t a;
  mpz_t e;
  mpz_t res;
  char *to_print;

  /*** Init var ****/
  to_print = malloc(MAX*sizeof(char));
  mpz_init(a);
  mpz_init(e);
  mpz_init(res);
  gmp_randinit_default(init);
  mpz_sub_ui(e, n, 1); // e =  n-1
  seed = (long)time(NULL);
  /** For accuracy of the supposition ***/
  for (i=0; i < PRECISION; i++){
    /*** gen rand ***/
    seed = seed++;
    gmp_randseed_ui(init,seed);
    mpz_urandomm(a,init,e);
    /** Test rand **/
    if(mpz_divisible_p(n,a)){
      free(to_print);
      mpz_clears(a,e,NULL);
      return 1;
    }
    /*** if a^(n-1) != 1[n] ==> composite ***/
    mpz_powm_sec(res, a, e, n);
    if (mpz_get_si(res) != 1){
    	mpz_clears(a,e,res,NULL);
	free(to_print);
      	return 1;
    } 
  }
/***/
  mpz_clears(a,e,res,NULL);
  free(to_print);
  return 0;
}

/********* GET PRIME Function ***
IN : Point string result, int number of bit of the prime
OUT : BOOL

DESC :
	while alea is not prime :
		generate random alea
		Test primality of alea with Fermat
*********************************/
int get_prime(char **r, int bit_size){
  /**init **/
  mpz_t alea;
  gmp_randstate_t init;	
  mpz_init(alea);
  gmp_randinit_default(init);
  int res = 1;
  long seed;
  /*** Find prime ***/
  seed = (long)time(NULL);
  while (res == 1){
    seed++;
    gmp_randseed_ui(init,seed);
    mpz_urandomb(alea,init,bit_size);
    if(mpz_divisible_ui_p(alea,2)){
      mpz_add_ui(alea,alea,1);
    }
    res = fermat(alea);
    if (res == 0 ){
	break;
    }
/***/
  }
  mpz_get_str(*r,10,alea);
  mpz_clear(alea);
  return 0;
}

/********* GET PRIME Function ***
IN : Point string result, mpz_t number n
OUT : BOOL

DESC :
	while alea is not prime :
		generate random alea from 0 to n
		Test primality of alea with Fermat
*********************************/
int get_rand_prime(char **r, char **n){
  /**init **/
  mpz_t alea;
  mpz_t limit;
  gmp_randstate_t init;	
  int res = 0;
  long seed;
	
  mpz_init(alea);
  mpz_init(limit);
  gmp_randinit_default(init);
  mpz_set_str(limit,*n,10);
  seed = (long)time(NULL);
  /*** Find prime ***/
  while (res == 0){
    seed++;
    gmp_randseed_ui(init,seed);
    mpz_urandomm(alea,init,limit);
    if(mpz_divisible_ui_p(alea,2)){
      mpz_add_ui(alea,alea,1);
    }
    res = fermat(alea);
    if (res > 0 ){
	break;
    }
/***/
  }
  mpz_get_str(*r,10,alea);
  mpz_clear(alea);
return 0;
}
