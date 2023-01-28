#include <stdio.h>
#include <winsock2.h>

#include "multiplayer.h"
#include "pacman.h"


int client;

void closeserver() {
  closesocket(client);
  WSACleanup();
}

void serversend(char comando) {
    send(client, &comando, sizeof comando, 0);
}

void initserver() {
  WSADATA wsa;

  if(WSAStartup(MAKEWORD(2, 0), &wsa)) {
    printf("WSAStartup deu erro.");
    exit(1);
  }

  struct sockaddr_in caddr;
  struct sockaddr_in saddr = {
    .sin_family       = AF_INET,
    .sin_addr.s_addr  = INADDR_ANY,
    .sin_port         = htons(PORT)
  };

  int server = socket(AF_INET, SOCK_STREAM, 0);

  int sizec = sizeof caddr;

  bind(server, (struct sockaddr *)&saddr, sizeof saddr);
  listen(server, 3);
  puts("\033[0;36m\033[1m==========================\n    Servidor iniciado.\n==========================\033[0m\n");

  client = accept(server, (struct sockaddr *)&caddr, &sizec);
  while(1) {
    char buffer[1024] = { 0 };

    int received= recv(client, buffer, sizeof buffer, 0);
    // printf("buffer [%s]", buffer);

    if(received) {
      // printf("\033[0;31m\033[1m%s\033[0m\n", buffer);
      updateheroi2(buffer);
    }

  }
}
