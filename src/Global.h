#ifndef _Globals
#define _Globals

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <fstream>

using namespace std;

typedef unsigned char byte;

/* --Definindo variáveis globais-- */
//arquivo do programa-fonte
extern FILE* src;
extern int linha;

//registro único de escopo global contendo campo para o número do token
extern string token;

//lexema
extern string lexema;

//endereço de inserção na tabela (somente para identificadores e palavras reservadas)
extern int posicao;

extern int memoria;
extern int memoriaTemp;
extern int rotulo;

//tipo (somente para o token constante)
enum TIPO{ TIPO_INTEIRO, TIPO_LOGICO, TIPO_BYTE, TIPO_STRING };
extern int tipo_constante;

enum CLASSE{ CLASSE_VAR, CLASSE_CONST };

#endif
