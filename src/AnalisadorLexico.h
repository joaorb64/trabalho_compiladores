#ifndef _AnalisadorLexico
#define _AnalisadorLexico

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include "Global.h"
#include "TabelaDeSimbolos.h"

using namespace std;

class AnalisadorLexico {
  public:
    AnalisadorLexico(string file, TabelaDeSimbolos* t);

    TabelaDeSimbolos* t;

    int LerToken();

    bool IsLetra(char c);
    bool IsDigito(char c);
    bool IsHexadecimal(char c);
    bool IsDelimitador(char c);

    bool IsConst(string lexema);
    bool IsID(string lexema);
};

#endif
