
// GRR 20166812 -- Gabriel de Souza Barreto

#ifndef __FUNC__
#define __FUNC__

#include <stdio.h>

typedef struct coordenadas
{
	int x;
	int y;
} coordenadas;

void preenchemoldura(char ss[101][27][255]);

void leituracsv(char ss[101][27][255], FILE *f);

void printmoldura(char ss[101][27][255]);

void printcsv(char ss[101][27][255], coordenadas seletor);

int atualiza(int input, coordenadas* seletor, char ss[101][27][255], FILE **f);

void edita(char* valor);

void salva(FILE **f, char ss[101][27][255]);

double resultado(char* valor, char ss[101][27][255]);

#endif
