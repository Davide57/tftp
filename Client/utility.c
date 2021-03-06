#include "utility.h"



void sendBuffer(int sock, char* buffer, int position, struct sockaddr_in server_addr){
	uint16_t lengthNet = htons(position);
	uint16_t length = (position > sizeof(buffer))?position:sizeof(buffer);
	unsigned int addrlen = sizeof(server_addr);

	
	int nbytes = sendto(sock, &lengthNet, sizeof(uint16_t), 0, (struct sockaddr*)&server_addr, addrlen);
;
	if(nbytes < sizeof(uint16_t)){
		perror("\nERRORE: Lunghezza dei dati inviati invalida\n");
	}
	
	nbytes = sendto(sock, buffer, length, 0,(struct sockaddr*)&server_addr, sizeof(server_addr));

	if(nbytes < sizeof(buffer)){
		perror("\nERRORE: Invio dei dati non riuscito\n");
	}

}



struct result receiveBuffer(int sock){
	struct sockaddr_in client_addr;
	uint16_t length;
	char* buffer = NULL;
	unsigned int addrlen = sizeof(client_addr);
	struct result r;
	
	int nbytes = recvfrom(sock, &length, sizeof(uint16_t), 0, (struct sockaddr*)&client_addr, &addrlen);
	
	if(nbytes < sizeof(uint16_t)){
		perror("\nERRORE: Lunghezza dei dati ricevuti invalida\n");
	}
	nbytes = 0;
	length = ntohs(length);

	buffer = (char *)malloc(length);

	if(buffer == NULL){
		perror("\nERRORE! Allocazione non riuscita\n");
		r.buffer = NULL;
		r.length = 0;
		return r;
	}

	memset(buffer, 0, length);
	
	nbytes = recvfrom(sock, buffer, length, 0, (struct sockaddr*)&client_addr, &addrlen);

	buffer[length] = 0;
	r.length = length;
	r.buffer = buffer;
	r.client_addr = client_addr;
	return r;
}



