#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "init.h"

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
	int k_iter;				//Key iteration
	int hkdf_iter;		//Derivation Key iteration
	int borne_start;	//create rand iteration between borne_start and borne_stop
	int borne_stop;
	char *alea;				//Alea from PRBG
	char *key_init;		//First key from PRBG
	char *key;					//Final key from Deruvation Function
	char *Ns;					//Serial number, the MAC adress here
	char *code;				//Final returned code
	int space = 6;		//Space between two '-'
	
	/**** If you want to enter the iteration by hand **
	if(argc < 3){printf("./%s <key iteration> <hkdf iteration>",argv[0]);return 1;}
	k_iter = argv[1];
	hkdf_iter = argv[2];
	/******** Otherwise it's a random ************************/
	srand(time(NULL));
	borne_start = 2560; //just to have 3 char with a least 1 alpha
	borne_stop = 4098;
	k_iter = rand()%(borne_stop - borne_start) + borne_start;
	hkdf_iter = rand()%(borne_stop - borne_start) + borne_start;
	/**************************************************/
	
	alea = (char *)malloc(sizeof(char)*ALEA_SIZE);
	key_init = (char *)malloc(sizeof(char)*HASH_SIZE);
	key = (char *)malloc(sizeof(char)*HASH_SIZE);
	Ns = (char *)malloc(sizeof(char)*MAX);
	code = (char *)malloc(sizeof(char)*MAX);
	/*** Gen Alea **/
	PRBG_RSA(&alea, k_iter);
	
	/*** Hash alea **/
	hash_function(alea,&key_init);

	/*** Get MAC adress for Ns ***/
	get_mac_adress(&Ns);
	
	/*** Key derivation **/
	HKDF(key_init, Ns, hkdf_iter, &key);

	/*** Traductor ***/
	traductor(&key, k_iter, hkdf_iter, &code, space);
	
	printf("The serial number is your MAC address...\nYour Serial Number is : %s\nYour Code is : %s\n",Ns,code);
	free(alea);
	free(key_init);
	free(key);
	free(Ns);
	free(code);
	return 0;
}
