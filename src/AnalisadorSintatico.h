#ifndef _AnalisadorSintatico
#define _AnalisadorSintatico

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

#include "AnalisadorLexico.h"
#include "Global.h"
#include "TabelaDeSimbolos.h"

using namespace std;

class AnalisadorSintatico {
  public:
		AnalisadorSintatico(AnalisadorLexico *lex);

    bool fim;

    //arquivo de saida
    FILE* out;

    //Referencia a um AnalisadorLexico
    AnalisadorLexico *lex;

		//procedimento CasaToken [...]
		//receber como parâmetro o token esperado pela gramática
		//compará-lo com o token corrente
		bool CasaToken(string expectedToken);

		/*
		Implemente um procedimento para cada símbolo não-terminal da gramática.
		Em cada procedimento, a escolha da regra de produção a ser utilizada depende do token lido pelo analisador léxico.
		Cada token do lado direito da regra deverá ser verificado pelo procedimento CasaToken.
		Símbolos não-terminais do lado direito da regra correspondem a chamadas ao procedimento correspondente.
		*/
    bool S();
    bool Comm();

    bool Decl();
    bool Const();
    bool Att();
    bool Rep();
    bool T();
    bool Nulo();
    bool Read();
		bool Write();

		bool Exp(int *tipo);
		bool ExpA(int *tipo);
    bool ExpB(int *tipo);
    bool ExpC(int *tipo);
    bool ExpD(int *tipo);
};

#endif
