#include "TabelaDeSimbolos.h"

TabelaDeSimbolos::TabelaDeSimbolos(){
	numSimbolos = 0;

	AddSimbolo("const", "decl_const");
	AddSimbolo("integer", "decl");
	AddSimbolo("byte", "decl");
	AddSimbolo("string", "decl");
	AddSimbolo("boolean", "decl");
	AddSimbolo("while", "while");
	AddSimbolo("if", "if");
	AddSimbolo("else", "else");
	AddSimbolo("and", "and");
	AddSimbolo("or", "or");
	AddSimbolo("not", "not");
	AddSimbolo("=", "=");
	AddSimbolo("==", "comp");
	AddSimbolo("(", "(");
	AddSimbolo(")", ")");
	AddSimbolo("<", "comp");
	AddSimbolo(">", "comp");
	AddSimbolo("!=", "comp");
	AddSimbolo(">=", "comp");
	AddSimbolo("<=", "comp");
	AddSimbolo(",", ",");
	AddSimbolo("+", "+");
	AddSimbolo("-", "-");
	AddSimbolo("*", "*");
	AddSimbolo("/", "/");
	AddSimbolo(";", ";");
	AddSimbolo("begin", "begin");
	AddSimbolo("end", "end");
	AddSimbolo("then", "then");
	AddSimbolo("main", "main");
	AddSimbolo("readln", "readln");
	AddSimbolo("write", "write");
	AddSimbolo("writeln", "writeln");
	AddSimbolo("true", "const");
	AddSimbolo("false", "const");
}

string TabelaDeSimbolos::FindSimbolo(std::string lex){
	std::map<string, Simbolo>::iterator it = simbolos.find(lex);
	string ret = "NULL";

	if (it != simbolos.end()){
		ret = it->second.token;
	}

	return ret;
}

int TabelaDeSimbolos::AddSimbolo(string lex, string tok){
	AddSimbolo(lex, tok, -1, -1);
}

int TabelaDeSimbolos::AddSimbolo(string lex, string tok, int classe, int tipo){
	if(FindSimbolo(lex) == "NULL"){
		Simbolo s;
		s.token = tok;
		s.classe = classe;
		s.tipo = tipo;

		simbolos.insert ( std::pair<string, Simbolo>(lex, s) );

		#ifdef DEBUG
			printf("(Simbolo adicionado: %d - %s - %s - %d)\n", numSimbolos, lex.c_str(), tok.c_str(), tipo);
		#endif

		numSimbolos++;
	}
}

Simbolo* TabelaDeSimbolos::GetSimbolo(std::string lex){
	std::map<string, Simbolo>::iterator it = simbolos.find(lex);

	Simbolo* ret = NULL;

	if (it != simbolos.end()){
		ret = &it->second;
	}

	return ret;
}
