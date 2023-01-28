#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "mapa.h"
#include "pacman.h"
#include "ui.h"

#include "multiplayer.h"

MAPA m;
POSICAO heroi;
POSICAO heroi2;

struct multiplayerinfo {
    int isonline;
    int isserver;
} multiplayer = {0, 0};

int tempilula = 0;

void* startserver() {
    multiplayer.isserver = 1;
    initserver();
}
void* startclient(char IP[]) {
    initclient(IP);
}

void updateheroi(char direcao[]) {
    move(direcao[0], &heroi);
}
void updateheroi2(char direcao[]) {
    move(direcao[0], &heroi2);
}

int main(int argc, char const* argv[]) {

    //// Start multiplayer ////
    if(argv[1] != '\0') {
        multiplayer.isonline = 1;
        pthread_t T_SERVER, T_CLIENT;
        if(!strcmp(argv[1], "server") || !strcmp(argv[1], "s"))
            pthread_create(&T_SERVER, NULL, startserver, NULL);
        if(!strcmp(argv[1], "client") || !strcmp(argv[1], "c"))
            pthread_create(&T_CLIENT, NULL, startclient, argv[2]);
    }
    //////////////////////////
    lemapa(&m, multiplayer.isonline);
    encontramapa(&m, &heroi, HEROI);
    encontramapa(&m, &heroi2, HEROI2);

Sleep(200);
    do{
        // system("cls");
        printf("Tem pilula %s\n", (tempilula ? "SIM" : "NAO"));
        printf("\033[0;34m\033[1m");
        imprimemapa(&m);
        printf("\033[0m");

        char comando;
        scanf(" %c", &comando);

        if(multiplayer.isonline) {
            if(multiplayer.isserver) {
                serversend(comando);
            }else {
                clientsend(comando);
            }
        }

        if(multiplayer.isserver || !multiplayer.isonline) {
            move(comando, &heroi);
        } else
            move(comando, &heroi2);
        fantasmas();

        POSICAO hero = (multiplayer.isserver || !multiplayer.isonline) ? heroi : heroi2;
        if(comando == BOMBA) explodepilula(hero.x, hero.y, 3);

    }while(!acabou());

    liberamapa(&m);

    return 0;
}

void explodepilula() {
    if(!tempilula) return;
    POSICAO hero = (multiplayer.isserver || !multiplayer.isonline) ? heroi : heroi2;

    explodepilula2(hero.x, hero.y, 0, 1, 3);
    explodepilula2(hero.x, hero.y, 0, -1, 3);
    explodepilula2(hero.x, hero.y, -1, 0, 3);
    explodepilula2(hero.x, hero.y, 1, 0, 3);

    tempilula = 0;
}

void explodepilula2(int x, int y, int somax, int somay, int qtd) {
    if(qtd == 0) return;

    int novox = x + somax;
    int novoy = y + somay;

    if(!ehvalida(&m, novox, novoy)) return;
    if(ehparede(&m, novox, novoy)) return;

    m.matriz[novox][novoy] = VAZIO;
    explodepilula2(novox, novoy, somax, somay, --qtd);
}

int praondefantasmavai(int xatual, int yatual, int* xdestino, int* ydestino) {
    int opcoes[4][2] = {
        { xatual, yatual +1 },
        { xatual +1, yatual },
        { xatual, yatual -1 },
        { xatual -1, yatual }
    };

    srand(time(0));
    for(int i = 0; i < 10; i++) {
        int posicao = rand() % 4;

        if(podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])) {
            *xdestino = opcoes[posicao][0];
            *ydestino = opcoes[posicao][1];

            return 1;
        }
    }

    return 0;
}

void fantasmas() {
    MAPA copia;

    copiamapa(&copia, &m);

    for(int i = 0; i < m.linhas; i++) {
        for(int j = 0; j < m.colunas; j++) {
            if(copia.matriz[i][j] == FANTASMA) {

                int xdestino;
                int ydestino;

                int encontrou = praondefantasmavai(i, j, &xdestino, &ydestino);

                if(encontrou)
                    andanomapa(&m, i, j, xdestino, ydestino);
            }
        }
    }

    liberamapa(&copia);
}

int ehdirecao(char direcao) {
    return (direcao == 'a' || direcao == 'w' || direcao == 's' || direcao == 'd');
}

void move(char direcao, POSICAO* hero) {
    if(!ehdirecao(direcao)) return;

    int proximox = hero->x;
    int proximoy = hero->y;

    switch(direcao) {
        case ESQUERDA:
            proximoy--;
            break;
        case CIMA:
            proximox--;
            break;
        case BAIXO:
            proximox++;
            break;
        case DIREITA:
            proximoy++;
            break;
    }

    if(!podeandar(&m, HEROI, proximox, proximoy) || ehpersonagem(&m, HEROI2, proximox, proximoy))
        return;

    if(ehpersonagem(&m, PILULA, proximox, proximoy)) {
        tempilula = 1;
    }

    andanomapa(&m, hero->x, hero->y, proximox, proximoy);
    hero->x = proximox;
    hero->y = proximoy;
}

int acabou() {
    POSICAO pos;
    POSICAO pos2;
    int heroinomapa = encontramapa(&m, &pos, HEROI);
    int heroi2nomapa = encontramapa(&m, &pos2, HEROI2);
    return !heroinomapa && !heroi2nomapa;
}
