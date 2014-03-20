#include "crypt.h"


/***** TRADUCTOR ****
IN : pointer string key, int key_iteration, int hkdf iteration, pointer string code, int space between 2 '-'
OUT : BOOL
DESC :
 code = hex(key_iteration) || hex(hkdf_iteration) || Base64-key-with-separa-tor
/*******************/
int traductor(char **key,  int k_i, int hkdf_i, char **code, int space){
	char *KI;
	char *HKDFI;
	char *body;
	char *tmp;
	int i,stop;
	
	KI = (char *)malloc(space*sizeof(char));
	HKDFI = (char *)malloc(space*sizeof(char));
	body = (char *)malloc(MAX*sizeof(char));
	tmp = (char *)malloc(MAX*sizeof(char));
	memset(KI,0,space);
	memset(HKDFI,0,space);
	memset(body,0,MAX);
	if(B64){
		//Code in base 64
		Base64Encode(*key,&body);
		memset(tmp,0,MAX);
	}else{
		//No encoding...could be changed to another encoding
		strcpy(body,*key);
	}
	/** Put Iteration in Hex first (must be ameliorated)**/
	sprintf(KI,"%x",k_i);
	/*** In UPPER case ***/
	stop = strlen(KI);
	for(i=0;i< stop;i++){
	  KI[i] = toupper(KI[i]);
	}
	/*** the same ...***/
	sprintf(HKDFI,"%x",hkdf_i);
	/*** In UPPER case ***/
	stop = strlen(HKDFI);
	for(i=0;i< stop;i++){
	  HKDFI[i] = toupper(HKDFI[i]);
	}
	/*** The size of the 2 hex key MUST be of the same size AND < space ***/
	 i = strlen(KI) + strlen(HKDFI);
	if ( i > space || strlen(KI) != strlen(HKDFI) ){
		printf("Cannot have a %d key for a %d space \n",i,space);
		return 0;
	}
	strcpy(*code,KI);
	strcat(*code,HKDFI);
	strcat(*code,"-");
	stop = strlen(body);
	for(i=0;i+space < stop;i=i+space){
		strncat(*code,body+i,space);
		strcat(*code,"-");
	}
	strncat(*code,body+i,space);
	return 1;
}

/***** INTERPRETOR ****
IN : pointer string key, pointer int key_iteration, pointer int hkdf_iteration, pointer string code
OUT : BOOL
DESC :
 code = hex(key_iteration) || hex(hkdf_iteration) || Base64-key-with-separa-tor
/*******************/
int interpretor(char **code, int *k_i, int *hkdf_i,  char **key, int space ){
	int idx, i, code_size, size;
	char *t_k, *l_key, *l_code;	
	idx=0;
	code_size = strlen(*code);
	l_key = malloc(sizeof(char)*code_size+1);
	l_code = malloc(sizeof(char)*code_size+1);
	
	memset(l_key,0,code_size+1);
	memset(l_code,0,code_size+1);
	strcpy(l_key,*code);
	/*** The iteration hex MUST be 2 blocs of the same size so : ***/
	while (l_key[idx] != '-'){ idx++;}
	if(idx >= code_size){
		printf("Your code is not correctly formated\n");
		return 2;
	}
	size = ceil((idx) / 2);
	t_k = malloc(sizeof(char)*(size + 2));
	memset(t_k,0,(size+2));

	/*** Get the first hex char of the key iteration **/
	strncpy(t_k,l_key,size);
	*k_i = (int)strtol(t_k,NULL,16);

	/*** Same for the hkdf iteration **/
	memset(t_k,0,(size+2));
	strncpy(t_k,l_key + size, size);
	*hkdf_i = (int)strtol(t_k,NULL,16);

	/**** Get the key***/
	idx = space + 1;
	strncpy(l_code,l_key + idx,space);
	idx = idx + space + 1;
	while (idx < code_size){
		strncat(l_code,l_key + idx,space);
		idx = idx + space + 1;
	}
	strcpy(*key,l_code);
	free(t_k);
	free(l_key);
	free(l_code);
return 0;
}
