#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <fstream>

#include "TabelaDeSimbolos.h"
#include "AnalisadorLexico.h"
#include "AnalisadorSintatico.h"
#include "Global.h"

using namespace std;

/* --Definindo variáveis globais-- */
//arquivo do programa-fonte
FILE* src;

int linha;

//registro único de escopo global contendo campo para o número do token
string token;
int numero_do_token;

//variável global para memória de dados
int memoria;

//lexema
string lexema;

//endereço de inserção na tabela (somente para identificadores e palavras reservadas)
int posicao;

//tipo (somente para o token constante)
int tipo_constante;

int main(int argc, char **argv){
	if (argc != 2) {
		std::cerr << "Uso: " << argv[0] << " arquivo-fonte" << std::endl;
		return 1;
	}

	TabelaDeSimbolos* t = new TabelaDeSimbolos();
	AnalisadorLexico* lex = new AnalisadorLexico(argv[1], t);
	AnalisadorSintatico* a = new AnalisadorSintatico(lex);

	a->S();

	return 0;
}
