#ifndef _PACMAN_H_
#define _PACMAN_H_

#define CIMA 'w'
#define BAIXO 's'
#define ESQUERDA 'a'
#define DIREITA 'd'
#define BOMBA 'b'

#include "mapa.h"

int acabou();
void move(char direcao, POSICAO* hero);
int ehdirecao(char direcao);
void fantasmas();
int praondefantasmavai(int xatual, int yatual, int* xdestino, int* ydestino);
void explodepilula();
void explodepilula2(int x, int y, int somax, int somay, int qtd);
void updateheroi(char direcao[]);
void updateheroi2(char direcao[]);

#endif
