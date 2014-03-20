#include <math.h>
#include "init.h"
#include "../gen.h"

/****** PRNG ****
IN : int iteration, int size (number of bits of the alea), string pointer alea
OUT : BOOL
DESC :
	global X, E, M
	x = M
	e = E
	m = M
	While i < iteration :
	 x = x**e mod m
/*****************/
int PRBG_RSA(char **alea, int iteration){
 mpz_t x; //alea X
 mpz_t e; // exponent
 mpz_t m; // modulo
 mpz_t kbloc; // kbloc = N(1 - 2/e)
 int i, size;
 int k; //number of bit of one bloc = N(1 -2/e)
 int r; //number of bit of Xi 
 char *t;
 char *t2;
 char *alea_res; //alea resultant
 
 t = (char *)malloc(sizeof(char)*MAX);
 t2 = (char *)malloc(sizeof(char)*MAX);

 mpz_init(x);
 mpz_init(e);
 mpz_init(m);
 mpz_init_set_ui(kbloc,2);
 
 memset(t,0,MAX);
 memset(t2,0,MAX);
 
 /** Get x, e and m from gen.h ***/
	/** get X **/
 Base64Decode(X,&t);
 g(&t,&t2);
 mpz_set_str(x,t2,10);
 memset(t,0,MAX);
 memset(t2,0,MAX);
 /** Get E **/
 Base64Decode(E,&t);
 g(&t,&t2);
 mpz_set_str(e,t2,10);
 memset(t,0,MAX);
 memset(t2,0,MAX);
 /** Get M **/
 Base64Decode(M,&t);
 g(&t,&t2);
 mpz_set_str(m,t2,10);
 memset(t,0,MAX);
 memset(t2,0,MAX);
 
 /*** Size of bloc = N*2/e **/
 mpz_mul_ui(kbloc,kbloc,ALEA_SIZE);
 mpz_cdiv_q(kbloc,kbloc,e);
 k = mpz_get_ui(kbloc);

 /*** Allow memory for the alea ***/
 alea_res = (char *)malloc(sizeof(char)*iteration*(k+1));
 memset(alea_res,0,((k+1)*ALEA_SIZE));
 
 /*** First bloc of alea ***/
 mpz_powm_sec(x,x,e,m);
 /** concatenate the result to the existent alea **/
 memset(t,0,MAX);
 mpz_get_str(t,2,x);
 r = strlen(t) - k;
 strcpy(alea_res,t+r);

 /*** Generate i bloc of alea. Alea MUST be at least of the size of the ALEA_SIZE**/
 if((k*iteration) < ALEA_SIZE) {
 	iteration = floor(ALEA_SIZE / k);
 } 
 /** Loop ***/
 for(i=0; i < iteration ;i++){
 	mpz_powm_sec(x,x,e,m);
 	/** concatenate the result to the existent alea **/
 	memset(t,0,MAX);
 	mpz_get_str(t,2,x);
 	r = strlen(t) - k;
 	strncat(alea_res,t+r,k);
 	/***/
 }
 /*** Take the last ALEA_SIZE bits OR TAKE a HASH FUNCTION ***/
 r = strlen(alea_res);
 if(r > ALEA_SIZE){
 	r = r - ALEA_SIZE;
 	strncpy(*alea, alea_res+r , ALEA_SIZE );
 }
 else{
 	strcpy(*alea, alea_res); 
 }
 free(t);
 free(t2);
 free(alea_res);
 return 0;
}


