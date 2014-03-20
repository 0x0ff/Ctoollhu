#include "crypt.h"

/*********** HKDF Function *******************
IN : String Ks, String serial, int iteration, String pointer Kfinal
OUT : BOOL
DESC :
  while i < iteration 
     Ki = sha256( Ks ^ sha256(Ki-1 || NS || i))
**********************************************/
int HKDF(const char * Ks,const char *Ns, int p_i, char *Kfinal[HASH_SIZE]){
	int i,j;
	int Ksize;
	char *hash1;
	char *K;
	char *concat;
	char idx[2]="0";
	
	hash1 = malloc(HASH_SIZE*sizeof(char));
	K = malloc(HASH_SIZE*sizeof(char));
	concat = (char *)malloc(MAX*sizeof(char));
	memset(concat,0,MAX);
	memset(K,0,HASH_SIZE);
	memset(hash1,0,HASH_SIZE);
	Ksize = strlen(Ks);
	
	/***** First step : INITIALISATION *****/
	/** Concat = NS || i ***/
	strncpy(concat,Ns,strlen(Ns));
	strncat(concat,idx,1);

	/**you can change the hash function to be more flexible ***/
	/*** sha256( Ns || 0) ****/
	hash_function(concat,&hash1);
	
	/*** Ks ^ sha256(Ns || 0) **/
	for (i=0; i < Ksize; i++){
		K[i] = hash1[i]^Ks[i];
	}
	memset(hash1,0,HASH_SIZE);
	
	/*** sha256(Ks ^ sha256(Ns || 0)) *****/
	hash_function(K,&hash1);
	
	/******** Second step : DERIVATION ********/
	for(j=0;j < p_i; j++){
		memset(concat,0,MAX);
		/** concat = (Kn-1 || Ns || i) **/
		strncpy(concat,hash1,HASH_SIZE);
		strncat(concat,Ns,strlen(Ns));
		idx[0]=(char)i;
		strncat(concat,"1",1); ///TO DO
		
		/** sha256(Kn-1 || Ns || i) **/
		memset(hash1,0,HASH_SIZE);
		hash_function(concat,&hash1);
		
		/** Ks ^ sha256(Kn-1 || Ns || i) **/
		for (i=0; i < Ksize; i++){
			K[i] = hash1[i]^Ks[i];
		}
		memset(hash1,0,HASH_SIZE);
		hash_function(K,&hash1);
	}
	strncpy(*Kfinal,hash1,strlen(hash1));
	free(K);
	free(hash1);
	free(concat);
}



/********* BASE64ENCODE Function ***
IN : String m,  String pointer buffer
OUT : BOOL

DESC :Encode m in b64 to buffer	
*********************************/
int Base64Encode(const char *m, char **buffer) {
	BIO *bio, *b64;
	FILE *stream;
	int size;

	size = strlen(m);

	//*buffer = (char *)malloc(encodedSize+1);
	stream = fmemopen(*buffer, MAX,"w");
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(bio, m, size );
	BIO_flush(bio);
	BIO_free_all(bio);
	fclose(stream);
	return 0;
}
/********* BASE64DECODE Function ***
IN : String m, Double Size_of_buffer, String pointer buffer
OUT : BOOL

DESC :Decode m in b64 to buffer	
*********************************/
int Base64Decode(char *m, char **buffer) {
	BIO *bio, *b64;
	FILE *stream;
	int len;
	stream = fmemopen(m, strlen(m),"r");
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	len = BIO_read(bio, *buffer, strlen(m));
	(*buffer)[len]='\0';
	BIO_flush(bio);
	BIO_free_all(bio);
	fclose(stream);

	return 0;
}


/************ SHA256 Functions ***************
IN : String m, String pointer hash
OUT : Void
***********************************************/
void sha256(const char *m, char **hash){
	int size,i;
	unsigned char t_hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	size = strlen(m);
	SHA256_Update(&ctx, m, size);
	SHA256_Final(t_hash, &ctx);
	for (i=0;i< SHA256_DIGEST_LENGTH;i ++){
		sprintf(*hash + (i*2), "%02x", t_hash[i]);
	}
	
	strncat(*hash,"",1);
	//*hash[65]='\0';
}

/************ MD5 Functions ***************
IN : String m, String pointer hash
OUT : Void
***********************************************/
void md5(const char *m, char **hash){
	int size,i;
	unsigned char t_hash[MD5_DIGEST_LENGTH];
	MD5_CTX ctx;
	MD5_Init(&ctx);
	size = strlen(m);
	MD5_Update(&ctx, m, size);
	MD5_Final(t_hash, &ctx);
	for (i=0;i< MD5_DIGEST_LENGTH;i ++){
		sprintf(*hash + (i*2), "%02x", t_hash[i]);
	}
	
	strncat(*hash,"",1);
	//*hash[65]='\0';
}

/******* HASH FUNCTION *********
Choose hash function between MD5, SHA256 and other
IN : const string m, string pointer to hash
OUT : void
/******************************/
void hash_function(const char *m, char **p_hash){
	switch(HASH_TYPE){
		case 1 : md5(m,p_hash);break;
		default : sha256(m,p_hash);break;
	}

}




#include "crypt.h"

/*********** HKDF Function *******************
IN : String Ks, String serial, int iteration, String pointer Kfinal
OUT : BOOL
DESC :
  while i < iteration 
     Ki = sha256( Ks ^ sha256(Ki-1 || NS || i))
**********************************************/
int HKDF(const char * Ks,const char *Ns, int p_i, char *Kfinal[HASH_SIZE]){
	int i,j;
	int Ksize;
	char *hash1;
	char *K;
	char *concat;
	char idx[2]="0";
	
	hash1 = malloc(HASH_SIZE*sizeof(char));
	K = malloc(HASH_SIZE*sizeof(char));
	concat = (char *)malloc(MAX*sizeof(char));
	memset(concat,0,MAX);
	memset(K,0,HASH_SIZE);
	memset(hash1,0,HASH_SIZE);
	Ksize = strlen(Ks);
	
	/***** First step : INITIALISATION *****/
	/** Concat = NS || i ***/
	strncpy(concat,Ns,strlen(Ns));
	strncat(concat,idx,1);

	/**you can change the hash function to be more flexible ***/
	/*** sha256( Ns || 0) ****/
	hash_function(concat,&hash1);
	
	/*** Ks ^ sha256(Ns || 0) **/
	for (i=0; i < Ksize; i++){
		K[i] = hash1[i]^Ks[i];
	}
	memset(hash1,0,HASH_SIZE);
	
	/*** sha256(Ks ^ sha256(Ns || 0)) *****/
	hash_function(K,&hash1);
	
	/******** Second step : DERIVATION ********/
	for(j=0;j < p_i; j++){
		memset(concat,0,MAX);
		/** concat = (Kn-1 || Ns || i) **/
		strncpy(concat,hash1,HASH_SIZE);
		strncat(concat,Ns,strlen(Ns));
		idx[0]=(char)i;
		strncat(concat,"1",1); ///TO DO
		
		/** sha256(Kn-1 || Ns || i) **/
		memset(hash1,0,HASH_SIZE);
		hash_function(concat,&hash1);
		
		/** Ks ^ sha256(Kn-1 || Ns || i) **/
		for (i=0; i < Ksize; i++){
			K[i] = hash1[i]^Ks[i];
		}
		memset(hash1,0,HASH_SIZE);
		hash_function(K,&hash1);
	}
	strncpy(*Kfinal,hash1,strlen(hash1));
	free(K);
	free(hash1);
	free(concat);
}



/********* BASE64ENCODE Function ***
IN : String m,  String pointer buffer
OUT : BOOL

DESC :Encode m in b64 to buffer	
*********************************/
int Base64Encode(const char *m, char **buffer) {
	BIO *bio, *b64;
	FILE *stream;
	int size;

	size = strlen(m);

	//*buffer = (char *)malloc(encodedSize+1);
	stream = fmemopen(*buffer, MAX,"w");
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(bio, m, size );
	BIO_flush(bio);
	BIO_free_all(bio);
	fclose(stream);
	return 0;
}
/********* BASE64DECODE Function ***
IN : String m, Double Size_of_buffer, String pointer buffer
OUT : BOOL

DESC :Decode m in b64 to buffer	
*********************************/
int Base64Decode(char *m, char **buffer) {
	BIO *bio, *b64;
	FILE *stream;
	int len;
	stream = fmemopen(m, strlen(m),"r");
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	len = BIO_read(bio, *buffer, strlen(m));
	(*buffer)[len]='\0';
	BIO_flush(bio);
	BIO_free_all(bio);
	fclose(stream);

	return 0;
}


/************ SHA256 Functions ***************
IN : String m, String pointer hash
OUT : Void
***********************************************/
void sha256(const char *m, char **hash){
	int size,i;
	unsigned char t_hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	size = strlen(m);
	SHA256_Update(&ctx, m, size);
	SHA256_Final(t_hash, &ctx);
	for (i=0;i< SHA256_DIGEST_LENGTH;i ++){
		sprintf(*hash + (i*2), "%02x", t_hash[i]);
	}
	
	strncat(*hash,"",1);
	//*hash[65]='\0';
}

/************ MD5 Functions ***************
IN : String m, String pointer hash
OUT : Void
***********************************************/
void md5(const char *m, char **hash){
	int size,i;
	unsigned char t_hash[MD5_DIGEST_LENGTH];
	MD5_CTX ctx;
	MD5_Init(&ctx);
	size = strlen(m);
	MD5_Update(&ctx, m, size);
	MD5_Final(t_hash, &ctx);
	for (i=0;i< MD5_DIGEST_LENGTH;i ++){
		sprintf(*hash + (i*2), "%02x", t_hash[i]);
	}
	
	strncat(*hash,"",1);
	//*hash[65]='\0';
}

/******* HASH FUNCTION *********
Choose hash function between MD5, SHA256 and other
IN : const string m, string pointer to hash
OUT : void
/******************************/
void hash_function(const char *m, char **p_hash){
	switch(HASH_TYPE){
		case 1 : md5(m,p_hash);break;
		default : sha256(m,p_hash);break;
	}

}




