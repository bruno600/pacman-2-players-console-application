#ifndef _MULTIPLAYER_
#define _MULTIPLAYER_

#define PORT 8080

void initserver();
void initclient(char IP[]);

void serversend(char comando);
void clientsend(char comando);

void closeserver();
void closeclient();

#endif
