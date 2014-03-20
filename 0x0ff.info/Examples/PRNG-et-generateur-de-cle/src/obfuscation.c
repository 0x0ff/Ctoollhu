#include "init.h"

/******* GET CLEAR *****
IN : String pointer n, mpz_t pointer to out-number
OUT : void
DESC :
  foreach char of n :
    xor(n,key[i%size_of_key])
    i++
/************/
void g( char **n, char **b){
	/***define ***/
	int i;
	int sz;
	char *l_n;
	/*** init ***/
	sz = strlen(*n);
	l_n = malloc(sizeof(char)*(sz+1));
	memset(l_n,0,(sz+1));
	strncpy(l_n,*n,sz);
	
	/*** encipher ***/
	for (i=0;i<sz;i++){ x(l_n[i],i%s); }
	//l_n[sz+1]='\0';
	
	/*** update the decrypted given number **/
	strncpy(*b,l_n,sz);
	free(l_n);
}

/******* SET ENCIPHERED *****
IN : mpz_t n, String pointer to output b 
OUT : void
DESC :
  foreach char of n :
    xor(n,key[i%size_of_key])
    i++
/************/
int st( mpz_t n, char **b){
	/*** define ***/
	char *t;
	int sz;
	int i;
	/*** init ***/
	t = malloc(sizeof(char)*MAX);
	mpz_get_str(t,10,n);
	sz = strlen(t);
	/*** encipher ***/
	for (i=0;i<sz;i++){ x(t[i],i%s); }
	t[sz+1]='\0';
	/*** update enciphered string ***/
	strcpy(*b,t);
	free(t);
	return sz;
}
