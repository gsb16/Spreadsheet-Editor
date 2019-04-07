
// GRR 20166812 -- Gabriel de Souza Barreto

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include "func.h"

char ss[101][27][255];
coordenadas seletor;
int input;

int main( int argc, char *argv[] )
{
	FILE *f;

	//Abre o arquivo
	if (argc >1)
	{
		f = fopen(argv[1], "r+");
	}

	seletor.x = 1;
	seletor.y = 1;

	//Faz a moldura
	preenchemoldura(ss);

	//Lê o arquivo csv
	if (f)
	{
	leituracsv(ss, f);
	}

	//Inicializa o ncurses
	initscr();
    raw();
    noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	//Inicia cores
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_YELLOW);

	//Cria planilha

	while (1)
	{
		//Teste da matriz lida
		printcsv(ss, seletor);


		printmoldura(ss);

		refresh();
		input = getch();
		atualiza(input, &seletor, ss, &f);
		
	}

	if(f)
	{
		fclose(f);
	}
	endwin();

	return 0;
}
