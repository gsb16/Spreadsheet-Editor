
// GRR 20166812 -- Gabriel de Souza Barreto

#include <stdio.h>
#include <ncurses.h>
#include "func.h"
#include <string.h>
#include <stdlib.h>


//Escreve as coordenadas na matriz da planilha
void preenchemoldura(char ss[101][27][255])
{
	char indice;

	indice = 'A';

	for (int a = 1; a < 27; a++)
	{
		ss[0][a][0] = indice;
		indice++;
	}

	for (int b = 1; b < 101; b++)
	{
		ss[b][0][0] = b;
	}
}


//Lê um arquivo CSV
void leituracsv(char ss[101][27][255], FILE *f)
{
	char termo = fgetc(f);
	int i = 1;
	int j = 1;
	int k = 0;
	int aspas = 0;


	while (!feof(f))
	{
		switch (termo)
		{
			case '"':
				aspas ? (aspas = 0) : (aspas = 1);
				break;
			case ',':
				if (aspas)
					{
						if ((i < 101) && (j < 27) && (k < 255))
						{
							ss[i][j][k] = termo;
						}
					}
				else
				{
					j++;
					k = 0;
				}
				break;
			case '\n':
				i++;
				j = 1;
				k = 0;
				break;
			default:
				if ((i < 101) && (j < 27) && (k < 255))
						{
							ss[i][j][k] = termo;
						}
				k++;
				break;

		}
		termo = fgetc(f);
	}
}


//Mostra a moldura da planilha separadamente
void printmoldura(char ss[101][27][255])
{

		attron(COLOR_PAIR(2));

	for (int a = 1; a < 27; a++)
	{
		mvprintw(0, a*8, " %c      ", ss[0][a][0]);
	}

	for (int b = 1; b < 101; b++)
	{
		mvprintw(b, 0, "%8d", ss[b][0][0]);
	}

}


//Mostra o conteúdo da planilha
void printcsv(char ss[101][27][255], coordenadas seletor)
{
	int x,y;

	attron(COLOR_PAIR(1));

	getyx(stdscr, y, x);
	for (int i = 1; i < 101; ++i)
	{
		for (int j = 1; j < 27; ++j)
		{
			if (ss[i][j][0] == '=')
			{
				
				//Se é expressão, mostra o resultado na célula
				mvprintw(i, j*8, "%8.4lf", resultado(ss[i][j], ss));
			}
			else
			{
				for (int k = 0; k < 8; ++k)
				{
					mvprintw(i, j*8 + k, "%c", ss[i][j][k] != '\0'?ss[i][j][k]:' ');
				}
			}
		}
	}
	attron(COLOR_PAIR(2));
	for (int k = 0; k < 8; ++k)
	{
		mvprintw(seletor.y, seletor.x*8+k, "%c", ss[seletor.y][seletor.x][k]!='\0'?ss[seletor.y][seletor.x][k]:' ');
	}
}


//Faz as ações referentes a cada tecla apertada
int atualiza(int input, coordenadas* seletor, char ss[101][27][255], FILE **f)
{
	switch (input)
	{
		case KEY_UP:
		if (seletor->y != 1)
		{
			seletor->y--;
		}
		break;

		case KEY_DOWN:
		if (seletor->y != 100)
		{
			seletor->y++;
		}
		break;

		case KEY_LEFT:
		if (seletor->x != 1)
		{
			seletor->x--;
		}
		break;

		case KEY_RIGHT:
		if (seletor->x != 26)
		{
			seletor->x++;
		}
		break;

		//Caso ENTER
		case 10:
		edita(ss[seletor->y][seletor->x]);
		break;

		//Salvar = ctrlS
		case ('S' & 037):
		mvprintw(0, 0, "SALVO");
		salva(f, ss);
		break;

		//Fecha a planilha = ctrlQ
		case ('Q' & 037):
		if (f)
		{
			fclose(*f);
		}
		endwin();
		exit (0);
	}

	return 0;
}


//Calcula o resultado da expressão algébrica
double resultado(char* valor, char ss[101][27][255])
{
	char pri[255],seg[255];

	int t = 1;

	int r = 1;

	int i, j;

	double pnum, snum;

	char temp[4];

	char op;
	
	
	//Lê o primeiro valor da expressão
	while ((valor[t] != '*') && (valor[t] != '/') && (valor[t] != '+') && (valor[t] != '-'))
	{
		pri[t] = valor[t];
		t++;
	}
	
	//Lê qual é a operação a ser feita
	op = valor[t];
	t++;
	
	//Lê o segundo valor da expressão
	while (valor[t] != '\0')
	{
		seg[r] = valor[t];
		t++;
		r++;
	}
	
	
	//Checa se é uma letra - referência a outra célula
	if (((pri[1] > 64) && (pri[1] < 91)) || ((pri[1] > 96) && (pri[1] < 123)))
	{
		(pri[1] > 90)?(j = pri[1] - 96):(j = pri[1] - 64);
		for (int l = 0; l < 3; l++)
		{
			temp[l] = pri[l + 2];
		}
		sscanf(temp, "%d", &i);
		sscanf(ss[i][j], "%lf", &pnum);
	}
	else
	{
		sscanf(pri, "%lf", &pnum);
	}

	
	//Checa se é uma letra - referência a outra célula
	if (((seg[1] > 64) && (seg[1] < 91)) || ((seg[1] > 96) && (seg[1] < 123)))
	{
		(seg[1] > 90)?(j = seg[1] - 96):(j = seg[1] - 64);
		for (int l = 0; l < 3; l++)
		{
			temp[l] = seg[l + 2];
		}
		sscanf(temp, "%d", &i);
		sscanf(ss[i][j], "%lf", &snum);
	}
	else
	{
		sscanf(seg, "%lf", &snum);
	}

	switch (op)
	{
		case '*':
		return pnum*snum;
		break;

		case '+':
		return pnum+snum;
		break;

		case '-':
		return pnum-snum;
		break;

		case '/':
		return pnum/snum;
		break;

	}
	return 0;
}

void edita(char* valor)
{
	char key;
	attron(COLOR_PAIR(3));
	mvprintw(23, 0, "EDICAO: %s", valor);
	refresh();
	key = getch();
	int a = strlen(valor);

	//Só sai quando for Enter
	while(key != 10)
	{
		if((key == 263) || (key == 127) || (key == ('G' & 037)))
		{
			if(a)
			{
				a--;
			}
			valor[a] = ' ';
		}
		else
		{
			valor[a] = key;
			a++;
		}
		mvprintw(23, 0, "EDICAO: %s", valor);
		key = getch();
	}
	valor[a] = '\0';

}


void salva(FILE **f, char ss[101][27][255])
{
	if (!*f)
	{
		*f = fopen("novoarquivo", "w+");
	}

	rewind(*f);
	for (int i = 1; i < 101; i++)
	{
		for (int j = 1; j < 26; j++)
		{

			fprintf (*f, "\"%s\",", ss[i][j]);

		}
		fprintf (*f, "\"%s\"\n", ss[i][26]);
	}
}
