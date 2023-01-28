#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "multiplayer.h"
#include "pacman.h"


int sock;

void closeclient() {
  closesocket(sock);
  WSACleanup();
}

void clientsend(char comando) {
  send(sock, &comando, sizeof comando, 0);
}

void initclient(char IP[]) {
  WSADATA wsa;

	if(WSAStartup(MAKEWORD(2, 0), &wsa)) {
    printf("WSAStartup deu erro.");
    exit(1);
  }

	sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(("127.0.0.1"));
	if(IP != NULL) serv_addr.sin_addr.s_addr = inet_addr((IP));
	
	int conn = connect(sock, (struct sockaddr *)&serv_addr, sizeof serv_addr);
	if(conn != 0) {
		printf("DEU ERRO AO TENTAR CONECTAR\n");
		exit(1);
	}
	puts("\033[0;34m\033[1m==========================\n     Client iniciado.\n==========================\033[0m\n");

	while(1) {
		char buffer[1024] = { 0 };

		int received = recv(sock, buffer, sizeof buffer, 0);
		// printf("buffer [%s]", buffer);

		if(received) {
			// printf("\033[0;31m\033[1m%s\033[0m\n", buffer);
			updateheroi(buffer);
		}
	}
}
