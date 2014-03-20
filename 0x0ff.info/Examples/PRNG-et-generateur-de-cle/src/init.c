#include "init.h"

int init(){
/**** DEF ***/
	mpz_t p; 	//prime 1
	mpz_t q;	//prime 2
	mpz_t x;	//alea
	mpz_t e;	//exponent
	mpz_t m;	//modulo
	mpz_t quadra;	// quadra = 3
	mpz_t mod;		//mod = 4
	mpz_t phi;		//phi = (p-1)(q-1)
	mpz_t limit;	//limit = 80e
	mpz_t gcd;		//gcd = gcd(e,phi)...must be 1
	mpz_t rbloc;	//number of bits of x = N*2/e
	char *xored;	//xored result value
	long seed;		//first time seed
	int nbit; 		//number of bits of x
	FILE *gen;		//gen.h file to write
	char gen_name[6]="gen.h";
  gmp_randstate_t init;
  int stop = 0, res = 0, i = 0, size = 0;
  char *to_print, *tmp, *buffer, *Kfinal;

  /*** Init var ***/
  to_print = (char *)malloc(MAX*sizeof(char));
  xored = (char *)malloc(MAX*sizeof(char));  
  tmp = (char *)malloc(MAX*sizeof(char));
  Kfinal = (char *)malloc(HASH_SIZE*sizeof(char));
  buffer = (char *)malloc(MAX*2*sizeof(char));
  
  mpz_init(p); //prime 1
  mpz_init(q); //prime 2
  mpz_init(e); //exponent
  mpz_init(x); //alea
  mpz_init(m); //Modulo = p*q
  mpz_init(phi); //Phi = (p-1)*(q-1)
  mpz_init(limit); //Limit = 80*e
  mpz_init(gcd);	//the GCD between e and phi
  mpz_init_set_si(rbloc,2); // rbloc = N*2/e
  mpz_init_set_si(quadra,3);
  mpz_init_set_si(mod,4);
  gmp_randinit_default(init);
  printf("Generate first prime P.\n");
  /*** Find prime p ***/
  while (stop == 0){
  	memset(to_print,0,MAX);
  	get_prime(&to_print,PRIME_SIZE);
  	mpz_set_str(p, to_print,10);
  	/*** p congrue to 3mod4 ***/
  	if (mpz_congruent_p(p, quadra, mod)) {
  		stop = 1;
  	}
  }
  printf("Prime p = %s\n",to_print);
  
  /*** Find prime q ***/
  printf("Generate second prime Q.\n");
  stop = 0;
  while (stop == 0){
  	memset(to_print,0,MAX);
  	get_prime(&to_print,PRIME_SIZE);
  	mpz_set_str(q, to_print,10);
  	/**** q congrue to 3mod4 and p!=q ***/
  	if ( (mpz_congruent_p(q, quadra, mod)) && (mpz_cmp(p,q) != 0) ) {
  		stop = 1;
  	}
  }
   printf("Prime q = %s\n",to_print);
   
  /*** Set Modulo m ***/
  mpz_mul(m,p,q);
  memset(to_print,0,MAX);
  mpz_get_str(to_print,10,m);
  printf("Modulo = %s\n",to_print);
  
  /*** Set Phi ***/
  mpz_sub_ui(e,p,1);
  mpz_sub_ui(x,q,1);
  mpz_mul(phi,e,x);
  
  /*** Set exponent ***/
  mpz_set_ui(e,2);
  mpz_mul_ui(limit,e,80);
  stop = 0;
  /*** e = rand(1,phi) ***/
  printf("Generate exponent.\n");
  while ( stop == 0 ) {
  	mpz_add_ui(e,e,1);
  	mpz_mul_ui(limit,e,80);
  	mpz_gcd(gcd,e,phi);
  	/***  80e < ALEA_SIZE && e < phi && gcd(e,phi) == 1 ***/
  	if (mpz_cmp_si(limit,ALEA_SIZE) < 0  && mpz_cmp(e,phi) < 0 && mpz_cmp_ui(gcd,1) == 0){
  		stop = 1;
  	}
  	if (mpz_cmp_si(limit,ALEA_SIZE) > 0){ printf("No exponent found ....\n"); return 2;}
  }
  memset(to_print,0,MAX);
  mpz_get_str(to_print,10,e);
  printf("Exponent = %s\n",to_print);
  
  /*** Set alea x ***/
  seed = (long)time(NULL);
  stop = 0;
  nbit = 0;
  /*** size in bit of X = N2/e ***/
  mpz_mul_ui(rbloc,rbloc,ALEA_SIZE);
  mpz_cdiv_q(rbloc,rbloc,e);
	nbit = mpz_get_ui(rbloc);
	
  while (stop == 0){
  	seed ++;
  	gmp_randseed_ui(init,seed);
  	mpz_urandomb(x,init,nbit);
  	if ( mpz_cmp_si(x,1) != 0 && mpz_cmp_si(x,0) != 0){ stop = 1;}
  }
  
  /**** Writing your obfuscated gen.h file ***/
	gen = fopen(gen_name,"w");
	/*** Write modulo M ***/
	memset(to_print,0,MAX);
	memset(tmp,0,MAX);
	strcpy(to_print,"#define M \"");
	/** xor **/
  st(m,&tmp);
  /** base64encode the xored value **/
  Base64Encode(tmp,&buffer);
  strcat(to_print,buffer);
  strncat(to_print,"\"\n",2);
  /** write in file gen.h **/
  fprintf(gen,to_print);
  memset(buffer,0,(2*MAX));
  
  /*** Write exponent E ***/
  memset(to_print,0,MAX);
  memset(tmp,0,MAX);
  strcpy(to_print,"#define E \"");
  /** xor **/
  st(e,&tmp);
  /** base64encode the xored value **/
  Base64Encode(tmp,&buffer);
  strcat(to_print,buffer);
  strncat(to_print,"\"\n",2);
  /** Write in file gen.h **/
  fprintf(gen,to_print);
  memset(buffer,0,(2*MAX));
  
  /*** Write alea X ***/
  memset(to_print,0,MAX);
  memset(tmp,0,MAX);
  strcpy(to_print,"#define X \"");
  /** xor **/
  st(x,&tmp);
  /** base64encode the xored value **/
  Base64Encode(tmp,&buffer);
  strcat(to_print,buffer);
  strncat(to_print,"\"\n",2);
  /** Write in file gen.h **/
  fprintf(gen,to_print);
  free(buffer);
  fclose(gen);

  /***** CLEANUP ***/
 mpz_clears(p,q,e,m,mod,x,limit,phi,quadra,gcd,NULL);
 free(tmp);
 free(to_print); 
  /****/
  return 0;
}


int main(){
	int res = 2;
	while (res !=0 ){res = init();}
return 0;
}
