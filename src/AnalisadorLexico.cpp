#include "AnalisadorLexico.h"

AnalisadorLexico::AnalisadorLexico(string file, TabelaDeSimbolos* t){
  linha = 1;

  src = fopen (file.c_str(), "r");

  this->t = t;
}

//Checa se um caractere é uma letra
bool AnalisadorLexico::IsLetra(char c){
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return true;
	else return false;
}

//Checa se um caractere é um dígito
bool AnalisadorLexico::IsDigito(char c){
	if(c >= '0' && c<='9') return true;
	else return false;
}

//Checa se um caractere é hexadecimal
bool AnalisadorLexico::IsHexadecimal(char c){
	if((c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')) return true;
	else return false;
}

//Checa se um caractere é um delimitador
bool AnalisadorLexico::IsDelimitador(char c){
	if(c == ';' || c == ',' || c == '(' || c == ')' || c == ':' || c == '\'' || c == '=' ||
     c == '!' || c == '<' || c == '>' || c == '*' || c == '/' ||  c == '+' || c == '-' ) return true;
	else return false;
}

//Este procedimento deverá ler o programa-fonte e identificar o próximo token
int AnalisadorLexico::LerToken(){
	int ch, next;

	lexema = "";
  tipo_constante = -1;
  token = "";

	while( true ){
    ch = fgetc(src);
    if(ch == EOF) return -1;

    next = fgetc(src);

    if(next != EOF)
      fseek(src, -1, SEEK_CUR);

    if(ch == ' ' || ch == '\n' || ch == '\r'){
      if(ch == '\n') linha++;
      if(lexema != ""){
        break;
      }
      else{
        tipo_constante = -1;
        token = "";
      }
    }
    else if(IsDelimitador(ch)){
      lexema += ch;

      if((ch == '=' && next == '=') ||
         (ch == '>' && next == '=') ||
         (ch == '<' && next == '=') ||
         (ch == '!' && next == '=')){
        lexema += next;
        fseek(src, 1, SEEK_CUR);
        break;
      }

      //Comentário
      else if(ch == '/' && next == '*'){
        while(next != EOF){
          ch = fgetc(src);
          next = fgetc(src);
          fseek(src, -1, SEEK_CUR);

          if(ch == '\n') linha++;

          if(next == EOF){
            return -1;
          }

          if(ch == '*' && next == '/'){
            lexema = "";
            tipo_constante = -1;
            token = "";
            fseek(src, 2, SEEK_CUR);
            break;
          }
        }
      }

      //String
      else if(ch == '\''){
        lexema = "";
        while(next != EOF){
          ch = fgetc(src);
          next = fgetc(src);
          fseek(src, -1, SEEK_CUR);

          if(next == EOF){
            return -1;
          }

          if(ch == '\''){
            if(next != '\''){
              token = "const";
              tipo_constante = TIPO_STRING;
              return true;
            }
            else{
              lexema += ch;
              fseek(src, 1, SEEK_CUR);
            }
          }
          else{
            lexema += ch;
          }
        }

        break;
      }

      else{
        break;
      }
    }
    else if(IsDelimitador(next)){
      lexema += ch;
      break;
    }
    else{
      lexema += ch;

      if(!IsHexadecimal(ch) && !IsDigito(ch) && !IsLetra(ch) && ch != '.'){
        printf("%d:caractere invalido.\n", linha);
        return false;
      }
    }
	}

  //TODO: checar tamanho do lexema lido 255?

  if((token = t->FindSimbolo(lexema)) == "NULL"){
    if(IsID(lexema)){
      token = "id";
      return true;
    }
    else if(IsConst(lexema)){
      token = "const";
      return true;
    }
    else printf("%d:lexema nao identificado [%s].\n", linha, lexema.c_str());
    token = "";
  }
  else{
    return true;
  }

	return -1;
}

bool AnalisadorLexico::IsConst(string lexema){
  //0h(d U l)(d U l)	U	d*.d+	U	d+

  //Hex
  if(lexema.length() == 4){
    if(lexema[0] == '0' && lexema[1] == 'h' && IsHexadecimal(lexema[2]) && IsHexadecimal(lexema[3])){
      tipo_constante = TIPO_BYTE;
      return true;
    }
  }

  int estado = 0;

  for(int i = 0; i < lexema.length(); i++){
    switch (estado) {
      case 0:
        if(IsDigito(lexema[i])) estado = 1;
        else if(lexema[i] == '.') estado = 2;
        else return false;
      break;
      case 1:
        if(lexema[i] == '.') estado = 2;
        else if(!IsDigito(lexema[i])) return false;
      break;
      case 2:
        if(IsDigito(lexema[i])) estado = 3;
        else return false;
      break;
      case 3:
        if(!IsDigito(lexema[i])) return false;
      break;
    }
  }

  if(atoi(lexema.c_str()) >= 0 && atoi(lexema.c_str())<=255){
    tipo_constante = TIPO_BYTE;
  }
  else{
    tipo_constante = TIPO_INTEIRO;
  }

  if(estado == 3 || estado == 1) return true;
  else return false;
}

bool AnalisadorLexico::IsID(string lexema){
  //(l U _* (l U d) )(l U d U _)*

  int estado = 0;

  for(int i = 0; i < lexema.length(); i++){
    switch(estado){
      case 0:
        if(IsLetra(lexema[i])){
          estado = 3;
        }
        else if(lexema[i] == '_'){
          estado = 1;
        }
        else{
          return false;
        }
      break;

      // Leu um _, ler mais até ser diferente de _
      case 1:
        if(IsLetra(lexema[i]) || IsDigito(lexema[i])){
          estado = 3;
        }
      break;

      case 3:
        if(!IsLetra(lexema[i]) && !IsDigito(lexema[i]) && lexema[i] != '_'){
          return false;
        }
      break;
    }
  }

  if(estado == 3)
    return true;
  else
    return false;
}
