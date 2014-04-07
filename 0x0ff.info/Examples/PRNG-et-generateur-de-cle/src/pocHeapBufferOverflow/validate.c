#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "../init.h"

int get_mac_adress(char **return_mac){
    int fd;
    struct ifreq ifr;
    char *iface = "eth0";
    unsigned char *mac;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

    ioctl(fd, SIOCGIFHWADDR, &ifr);

    close(fd);

    mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;

    //display mac address
    sprintf(*return_mac,"%.2x%.2x%.2x%.2x%.2x%.2x\0" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return 0;
}

int main(int argc, char *argv[]){
	int k_iter;			//Key iteration
	int hkdf_iter;		//Derivation Key iteration
	int borne_start;	//create rand iteration between borne_start and borne_stop
	int borne_stop;
	char *to_valid;		//Alea from PRBG
	char *key_init;		//First key from PRBG
	char *key;			//Final key from Deruvation Function
	char *Ns;			//Serial number, the MAC adress here
	char *code;			//Final returned code
	int space = 6;		//Space between two '-'
	int i,j;
		char *userID;
	/**** Enter the entire code **/
	if(argc < 2){printf("./%s <your code>",argv[0]);return 1;}


	/**************************************************/

	to_valid = (char *)malloc(sizeof(char)*MAX);
	key_init = (char *)malloc(sizeof(char)*HASH_SIZE);
	key = (char *)malloc(sizeof(char)*HASH_SIZE);
	Ns = (char *)malloc(sizeof(char)*MAX);
	code = (char *)malloc(sizeof(char)*MAX);
	userID = (char *)malloc(sizeof(char)*HASH_SIZE);

	memset(to_valid,0,MAX);
	memset(code,1,MAX);
	memset(key,0,HASH_SIZE);
	memset(key_init,0,HASH_SIZE);
	memset(Ns,0,MAX);
	memset(userID,0,HASH_SIZE);
	/** Get the iteration variable and the code **/
	
	/** Heap-Based Overflow Part : Display the Heap **/
	strcpy(userID,"guest");
	printf("Heap avant Overflow :");
	for(j=0;j<23;j++){
		printf(" %d", code[(2045+j)*sizeof(char)]);
	}
	printf("\n");
	strcpy(code,argv[1]);
	printf("Heap après Overflow :");
	for(j=0;j<23;j++){
		printf(" %d", code[(2045+j)*sizeof(char)]);
	}
	printf("\n");
	
	i = interpretor(&code,&k_iter,&hkdf_iter,&to_valid,space);
	if ( i > 0 ){ return 2;}

	if(B64){
		/*** B64 Decoding ***/
		memset(code,0,MAX);
		Base64Decode(to_valid,&code);
	}
	else{
		/*** No encoding...another encoding could be used ***/
		strcpy(code,to_valid);
	}
	/*** Gen Alea **/
	memset(to_valid,0,MAX);
	PRBG_RSA(&to_valid, k_iter);

	/*** Hash alea **/
	hash_function(to_valid,&key_init);

	/*** Get MAC adress for Ns ***/
	get_mac_adress(&Ns);

	/*** Key derivation **/
	HKDF(key_init, Ns, hkdf_iter, &key);
	printf("Key : %s\nCode: %s\n",key,code);
	
	/*** Check ***/
	if(strcmp(key,code)==0){
		printf("Your code is OK !!\\o/\n");
				sha256(code,&userID);
	}else{
		printf("Bad code...\n");
	}
	
	/** Heap-Based Overflow Part : Guest (Demo) or Registered User (Full Version) **/
	if(strcmp(userID,"guest")==0){
		printf("Welcome guest...\n");
		/** There, the demo version **/
	}
	else {
		printf("Welcome registered user : %s !\n",userID);
		/** There, the full version **/
	}
	
	free(to_valid);
	free(key_init);
	free(key);
	free(Ns);
	free(code);
	return 0;
}
