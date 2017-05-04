#ifndef _TabelaDeSimbolos
#define _TabelaDeSimbolos

#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "Global.h"

using namespace std;

struct Simbolo {
	string token;
	int classe;
	int tipo;
};

class TabelaDeSimbolos {
private:
	//(arranjo de caracteres) lexema da palavra ou identificador, token
	std::map<string, Simbolo> simbolos;

	int numSimbolos;

public:
	TabelaDeSimbolos();

	string FindSimbolo(std::string lex); //Uma função que pesquisa a tabela em busca de um lexema e retorna o endereço do registro correspondente, caso exista, ou NULL se este não estiver na tabela
	int AddSimbolo(std::string lex, string tok); //Uma função que insere dinamicamente um registro na tabela, com o token e seu lexema, retornando o endereço de inserção
	int AddSimbolo(std::string lex, string tok, int classe, int tipo);
	Simbolo* GetSimbolo(std::string lex);
};

//ID: {l, d, _} [(l U _)(l U d U _)*] (max 255 chars)

#endif
