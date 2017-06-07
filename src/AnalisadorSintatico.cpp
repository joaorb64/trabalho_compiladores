#include "AnalisadorSintatico.h"

AnalisadorSintatico::AnalisadorSintatico(AnalisadorLexico *lex){
	this->lex = lex;
	fim = false;
	lex->LerToken();
	out = fopen ("out.asm", "w");
}

//retorna o endereço de um novo temporário
int AnalisadorSintatico::NovoTemp(int tamanho){
	int resposta = memoriaTemp;
	memoriaTemp += tamanho;
	return resposta;
}

//retorna o endereço de um novo rótulo
int AnalisadorSintatico::NovoRot(){
	int resposta = rotulo;
	rotulo += 1;
	return resposta;
}

//procedimento CasaToken [...]
//receber como parâmetro o token esperado pela gramática
//compará-lo com o token corrente
bool AnalisadorSintatico::CasaToken(string expectedToken){
	//Se estes forem iguais
	if(token.compare(expectedToken) == 0){
		#ifdef DEBUG
			static int ident = 0;
			static bool newline = true;

			if(token == "begin" || token == "main"){ident += 1;}
			if(token == "end"){ident -= 1;}

			if(newline){
				for(int i=0; i<ident; i++) printf("\t");
			}
			printf("[%s (%s)]", lexema.c_str(), token.c_str());

			if(token == ";" || token == "begin" || token == "main" || token == "end"){
				printf("\n");
				newline = true;
			}
			else{printf(" "); newline = false;}
		#endif

		//o próximo token do programa será lido (chamada ao analisador léxico)
		int res = lex->LerToken();
		if(res == 0) fim=true;
	}
	//caso contrário
	//deverá ser emitida uma mensagem com o erro encontrado e a linha do programa onde este ocorreu, encerrando o processo de compilação.
	else{
		if(!fim){
			if(token == ""){
				printf("%d:fim de arquivo nao esperado.\n", linha);
				exit(0);
			}
			else{
				printf("%d:Token não esperado [%s].\n", linha, token.c_str());
				exit(0);
			}
		}
		fim = true;
		return false;
	}
}

// S →  { Decl | Const | Nulo } 'main' {Comm} 'end'
bool AnalisadorSintatico::S(){
	fprintf(out, "dseg SEGMENT PUBLIC; inicio seg. dados;\n	byte 4000h DUP(?); temporarios\n");
	fprintf(out, "dseg ENDS;\n");
	memoria = 16384;
	memoriaTemp = 0;
	rotulo = 0;

	fprintf(out, "cseg SEGMENT PUBLIC; inicio do seg. código\n");
	fprintf(out, "ASSUME CS:cseg, DS:dseg\n");
	fprintf(out, "_strt: ; inicio do programa\n");
	fprintf(out, "mov ax, dseg\n");
	fprintf(out, "mov ds, ax\n");

	while(token == "decl" || token == "decl_const" || token == ";"){
		if(token == "decl"){
			Decl();
		}
		else if(token == "decl_const"){
			Const();
		}
		else{
			Nulo();
		}
	}

	CasaToken("main");
	while(token == "id" || token == "while" || token == "if" || token == "readln" || token == "write" || token == "writeln" || token == ";"){
		Comm();
	}
	CasaToken("end");

	if(token != ""){
		printf("%d:Token não esperado [%s].\n", linha, token.c_str());
	}

	fprintf(out, "mov ah, 4Ch; termina o programa\n");
	fprintf(out, "int 21h\n");
	fprintf(out, "cseg ENDS; fim seg. código\n");
	fprintf(out, "END _strt\n");
}

// Comm → ( Att | Rep | T | Read | Write | Nulo )
bool AnalisadorSintatico::Comm(){
	if(token == "id"){
		Att();
	}
	else if(token == "while"){
		Rep();
	}
	else if(token == "if"){
		T();
	}
	else if(token == "readln"){
		Read();
	}
	else if(token == "write" || token == "writeln"){
		Write();
	}
	else{
		Nulo();
	}
}

// Decl → tipo id [= Exp] {, id [= Exp]};
bool AnalisadorSintatico::Decl(){
	//TODO: checagem de tipos!!

	string tipo = lexema;
	string lexe;

	CasaToken("decl");

	lexe = lexema;

	fprintf(out, "dseg SEGMENT PUBLIC\n");

	//Checar se já foi declarado
	if(token == "id"){
		std::stringstream ss;
		ss << std::uppercase << std::hex << memoria;
		std::locale loc;

		if(lex->t->FindSimbolo(lexema) == "NULL"){
			if(tipo == "string"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_STRING);
				fprintf(out, "	byte 100h DUP(?); string [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else if(tipo == "integer"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_INTEIRO);
				fprintf(out, "	sword ?; inteiro [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else if(tipo == "boolean"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_LOGICO);
				fprintf(out, "	byte ?; boolean [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else if(tipo == "byte"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_BYTE);
				fprintf(out, "	byte ?; byte [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
		}
		else{
			printf("%d:identificador ja declarado [%s].\n", linha, lexema.c_str());
			exit(0);
		}
	}

	fprintf(out, "dseg ENDS;\n");

	CasaToken("id");

	if(token == "="){
		CasaToken("=");
		int *tipo = new int(-1);
		int *end = new int(-1);
		bool converter = false;
		Exp(tipo, end);

		if(*tipo != lex->t->GetSimbolo(lexe)->tipo){
			if(*tipo == TIPO_BYTE && lex->t->GetSimbolo(lexe)->tipo == TIPO_INTEIRO){
				converter = true;
			}
			else{
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
		}

		std::stringstream ssA, ssB;
		ssA << std::uppercase << std::hex << lex->t->GetSimbolo(lexe)->endereco;
		ssB << std::uppercase << std::hex << *end;

		string regB;

		if(*tipo == TIPO_INTEIRO){
			regB = "BX";
		}
		else{
			regB = "BL";

			if(converter){
				fprintf(out, "	mov BH, 0\n");
			}
		}

		fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssB.str().c_str());

		if(converter){
			regB = "BX";
		}

		fprintf(out, "	mov DS:[%sh], %s\n", ssA.str().c_str(), regB.c_str());
	}

	while (token == ",") {
		CasaToken(",");

		fprintf(out, "dseg SEGMENT PUBLIC\n");

		//Checar se já foi declarado
		if(token == "id"){
			std::stringstream ss;
			ss << std::uppercase << std::hex << memoria;
			std::locale loc;

			if(tipo == "string"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_STRING);
				fprintf(out, "	byte 100h DUP(?); string [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else if(tipo == "integer"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_INTEIRO);
				fprintf(out, "	sword ?; inteiro [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else if(tipo == "boolean"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_LOGICO);
				fprintf(out, "	byte ?; boolean [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else if(tipo == "byte"){
				lex->t->AddSimbolo(lexema, "id", CLASSE_VAR, TIPO_BYTE);
				fprintf(out, "	byte ?; byte [%s] em %sh\n", lexema.c_str(), ss.str().c_str());
			}
			else{
				printf("%d:identificador ja declarado [%s].\n", linha, lexema.c_str());
				exit(0);
			}
		}

		CasaToken("id");

		fprintf(out, "dseg ENDS;\n");

		if(token == "="){
			CasaToken("=");
			int *tipo = new int(-1);
			int *end = new int(-1);
			bool converter = false;
			Exp(tipo, end);

			if(*tipo != lex->t->GetSimbolo(lexe)->tipo){
				if(*tipo == TIPO_BYTE && lex->t->GetSimbolo(lexe)->tipo == TIPO_INTEIRO){
					converter = true;
				}
				else{
					printf("%d:tipos incompatíveis.\n", linha);
					exit(0);
				}
			}

			std::stringstream ssA, ssB;
			ssA << std::uppercase << std::hex << lex->t->GetSimbolo(lexe)->endereco;
			ssB << std::uppercase << std::hex << *end;

			string regB;

			if(*tipo == TIPO_INTEIRO){
				regB = "BX";
			}
			else{
				regB = "BL";

				if(converter){
					fprintf(out, "	mov BH, 0\n");
				}
			}

			fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssB.str().c_str());

			if(converter){
				regB = "BX";
			}

			fprintf(out, "	mov DS:[%sh], %s\n", ssA.str().c_str(), regB.c_str());
		}
	}

	CasaToken(";");
}

// Const → 'const' id = Exp;
bool AnalisadorSintatico::Const(){
	CasaToken("decl_const");

	string tok = token;
	string lexe = lexema;
	string valor = "";

	CasaToken("id");
	CasaToken("=");
	int *tipo = new int(-1);
	int *end = new int(-1);
	valor = lexema;
	Exp(tipo, end);
	CasaToken(";");

	//Checar se já foi declarado
	if(tok == "id"){
		if(lex->t->FindSimbolo(lexe) == "NULL"){
			fprintf(out, "dseg SEGMENT PUBLIC;\n");

			lex->t->AddSimbolo(lexe, "id", CLASSE_CONST, *tipo);

			std::stringstream ssA, ssB;
			ssA << std::uppercase << std::hex << lex->t->GetSimbolo(lexe)->endereco;
			ssB << std::uppercase << std::hex << *end;

			if (*tipo == TIPO_INTEIRO) {
				fprintf(out, "	sword ?; inteiro [%s] em %sh\n", lexe.c_str(), ssA.str().c_str());
				fprintf(out, "dseg ENDS;\n");
				fprintf(out, "	mov BX, DS:[%sh]\n", ssB.str().c_str());
				fprintf(out, "	mov DS:[%sh], BX\n", ssA.str().c_str());
			} else if (*tipo == TIPO_BYTE) {
				fprintf(out, "	byte ?; byte [%s] em %sh\n", lexe.c_str(), ssA.str().c_str());
				fprintf(out, "dseg ENDS;\n");
				fprintf(out, "	mov BX, DS:[%sh]\n", ssB.str().c_str());
				fprintf(out, "	mov DS:[%sh], BX\n", ssA.str().c_str());
			} else if (*tipo == TIPO_LOGICO) {
				fprintf(out, "	byte ?; byte [%s] em %sh\n", lexe.c_str(), ssA.str().c_str());
				fprintf(out, "dseg ENDS;\n");
				fprintf(out, "	mov BL, DS:[%sh]\n", ssB.str().c_str());
				fprintf(out, "	mov DS:[%sh], BL\n", ssA.str().c_str());
			} else if (*tipo == TIPO_STRING) {
				fprintf(out, "	byte 100h DUP(?); string [%s] em %sh\n", lexe.c_str(), ssA.str().c_str());
				fprintf(out, "dseg ENDS;\n");

				int rotInicio = NovoRot();
				int rotFim = NovoRot();

				fprintf(out, ";[While - Copia de string]\n");
				fprintf(out, "	mov BX, %sh; guarda posicao inicial do string\n", ssA.str().c_str());
				fprintf(out, "	mov CX, %sh; guarda posicao do temporario\n", ssB.str().c_str());
				fprintf(out, "R%d:\n", rotInicio);
				fprintf(out, "	mov DL, DS:[CX]; move temporario\n");
				fprintf(out, "	cmp DL, 36; compara fim do string\n");
				fprintf(out, "	je R%d\n", rotFim);

				fprintf(out, "	mov AX, DS:[CX]; move do temporario\n");
				fprintf(out, "	mov DS:[BX], AX; move para string\n");
				fprintf(out, "	add CX, 1; aumenta ponteiro do temporario\n");
				fprintf(out, "	add BX, 1; aumenta ponteiro do string\n");

				fprintf(out, "jmp %d\n", rotInicio);
				fprintf(out, "R%d:\n", rotFim);
				fprintf(out, ";[While fim - Copia de string]\n");
			}
		}
		else{
			printf("%d:identificador ja declarado [%s].\n", linha, lexe.c_str());
			exit(0);
		}
	}
}

// Att → id = Exp;
bool AnalisadorSintatico::Att(){
	fprintf(out, ";[Att]\n");

	if(lex->t->FindSimbolo(lexema) == "NULL"){
		printf("%d:identificador nao declarado [%s].\n", linha, lexema.c_str());
		exit(0);
	}
	else if(lex->t->GetSimbolo(lexema)->classe == CLASSE_CONST){
		printf("%d:classe de identificador incompatível [%s].\n", linha, lexema.c_str());
		exit(0);
	}

	string lexe = lexema;

	CasaToken("id");
	CasaToken("=");
	int *tipo = new int(-1);
	int *end = new int(-1);
	Exp(tipo, end);

	bool converter = false;

	if(lex->t->GetSimbolo(lexe)->tipo != *tipo){
		if(lex->t->GetSimbolo(lexe)->tipo == TIPO_INTEIRO && *tipo == TIPO_BYTE){
			converter = true;
		}
		else{
			printf("%d:tipos incompatíveis.\n", linha);
			exit(0);
		}
	}

	CasaToken(";");

	std::stringstream ssA, ssB;
	ssA << std::uppercase << std::hex << lex->t->GetSimbolo(lexe)->endereco;
	ssB << std::uppercase << std::hex << *end;

	string regB;

	if(*tipo == TIPO_INTEIRO){
		regB = "BX";
	}
	else{
		regB = "BL";

		if(converter){
			fprintf(out, "	mov BH, 0\n");
		}
	}

	fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssB.str().c_str());

	if(converter){
		regB = "BX";
	}

	fprintf(out, "	mov DS:[%sh], %s\n", ssA.str().c_str(), regB.c_str());
}

// Write → ( write | writeln ) '(' Exp {, Exp} ')';
bool AnalisadorSintatico::Write(){
	bool quebraLinha = false;

	if(token == "write"){
		CasaToken("write");
	}
	else{
		CasaToken("writeln");
		quebraLinha = true;
	}

	CasaToken("(");

	while (true){
		int *tipo = new int(-1);
		int *end = new int(-1);
		Exp(tipo, end);

		std::stringstream ss;
		ss << std::uppercase << std::hex << *end;

		int r0 = NovoRot();
		int r1 = NovoRot();
		int r2 = NovoRot();

		if(*tipo == TIPO_INTEIRO){
			fprintf(out, "mov ax, DS:[%sh] ;numero\n", ss.str().c_str());
		}
		else if(*tipo == TIPO_BYTE){
			fprintf(out, "mov al, DS:[%sh] ;numero\n", ss.str().c_str());
			fprintf(out, "mov ah, 0 ;numero\n");
		}

		fprintf(out, "mov di, DS:[%sh]; end. string temp\n", ss.str().c_str());
		fprintf(out, "mov cx, 0 ;contador\n");
		fprintf(out, "cmp ax,0 ;verifica sinal\n");
		fprintf(out, "jge R%d  ;salta se número positivo\n", r0);
		fprintf(out, "mov bl, 2Dh ;senão, escreve sinal –\n");
		fprintf(out, "mov ds:[di], bl\n");
		fprintf(out, "add di, 1 ;incrementa índice\n");
		fprintf(out, "neg ax  ;toma módulo do número\n");
		fprintf(out, "R%d:\n", r0);
		fprintf(out, "mov bx, 10 ;divisor\n");
		fprintf(out, "R%d:\n", r1);
		fprintf(out, "add cx, 1 ;incrementa contador\n");
		fprintf(out, "mov dx, 0 ;estende 32bits p/ div.\n");
		fprintf(out, "idiv bx   ;divide DXAX por BX\n");
		fprintf(out, "push dx  ;empilha valor do resto\n");
		fprintf(out, "cmp ax, 0 ;verifica se quoc. é 0\n");
		fprintf(out, "jne R%d  ;se não é 0, continua\n", r1);
		fprintf(out, ";agora, desemp. os valores e escreve o string\n");
		fprintf(out, "R%d:\n", r2);
		fprintf(out, "pop dx  ;desempilha valor\n");
		fprintf(out, "add dx, 30h ;transforma em caractere\n");
		fprintf(out, "mov ds:[di],dl ;escreve caractere\n");
		fprintf(out, "add di, 1 ;incrementa base\n");
		fprintf(out, "add cx, -1 ;decrementa contador\n");
		fprintf(out, "cmp cx, 0 ;verifica pilha vazia\n");
		fprintf(out, "jne R%d  ;se não pilha vazia, loop\n", r2);
		fprintf(out, ";grava fim de string\n");
		fprintf(out, "mov dl, 024h ;fim de string\n");
		fprintf(out, "mov ds:[di], dl ;grava $\n");
		fprintf(out, ";exibe string\n");
		fprintf(out, "mov dx, DS:[%sh]\n", ss.str().c_str());
		fprintf(out, "mov ah, 09h\n");
		fprintf(out, "int 21h\n");

		if(token != ","){
			break;
		}
		else{
			CasaToken(",");
		}
	}

	if(quebraLinha){
		fprintf(out, "mov ah, 02h; quebra de linha\n");
		fprintf(out, "mov dl, 0Dh\n");
		fprintf(out, "int 21h\n");
		fprintf(out, "mov dl, 0Ah\n");
		fprintf(out, "int 21h\n");
	}

	CasaToken(")");
	CasaToken(";");
}

// Read → readln(id);
bool AnalisadorSintatico::Read(){
	CasaToken("readln");
	CasaToken("(");
	CasaToken("id");
	CasaToken(")");
	CasaToken(";");
}

// Rep → while ( Exp ) ( Comm | begin {Comm} end )
bool AnalisadorSintatico::Rep(){
	CasaToken("while");
	CasaToken("(");

	int rotInicio = NovoRot();
	int rotFim = NovoRot();

	fprintf(out, ";[While]\n");
	fprintf(out, "R%d:\n", rotInicio);

	int *tipo = new int(-1);
	int *end = new int(-1);

	Exp(tipo, end);
	CasaToken(")");

	if(*tipo != TIPO_LOGICO){
		printf("%d:tipos incompatíveis.\n", linha);
		exit(0);
	}

	std::stringstream ss;
	ss << std::uppercase << std::hex << *end;

	fprintf(out, "	mov AL, DS:[%sh]\n", ss.str().c_str());
	fprintf(out, "	cmp AL, FFh\n");
	fprintf(out, "	jne R%d\n", rotFim);

	if(token != "begin"){
		Comm();
	}
	else{
		CasaToken("begin");
		while(token != "end"){
			Comm();
		}
		CasaToken("end");
	}

	fprintf(out, "R%d:\n", rotFim);
	fprintf(out, ";[While fim]\n");
}

// T → if ( Exp ) then ( Comm | begin {Comm} end ) [ else ( Comm | begin {Comm} end ) ]
bool AnalisadorSintatico::T(){
	fprintf(out, ";[if]\n");

	CasaToken("if");
	CasaToken("(");

	int rotFalso = NovoRot();
	int rotFim = NovoRot();

	int *tipo = new int(-1);
	int *end = new int(-1);
	Exp(tipo, end);
	CasaToken(")");

	if(*tipo != TIPO_LOGICO){
		printf("%d:tipos incompatíveis.\n", linha);
		exit(0);
	}

	std::stringstream ss;
	ss << std::uppercase << std::hex << *end;

	fprintf(out, "	mov AL, DS:[%sh]\n", ss.str().c_str());
	fprintf(out, "	cmp AL, FFh\n");
	fprintf(out, "	jne R%d\n", rotFalso);

	CasaToken("then");

	if(token == "begin"){
		CasaToken("begin");
		while(token != "end"){
			Comm();
		}
		CasaToken("end");
	}
	else{
		Comm();
	}

	fprintf(out, "	jmp R%d\n", rotFim);
	fprintf(out, "	R%d:\n", rotFalso);

	if(token == "else"){
		CasaToken("else");

		if(token == "begin"){
			CasaToken("begin");
			while(token != "end"){
				Comm();
			}
			CasaToken("end");
		}
		else{
			Comm();
		}
	}

	fprintf(out, "	R%d:\n", rotFim);

	fprintf(out, ";[if fim]\n");
}

// Nulo → ;
bool AnalisadorSintatico::Nulo(){
	CasaToken(";");
}

// Exp → ExpA {comparadorArit ExpA}
bool AnalisadorSintatico::Exp(int *tipo, int *endereco){
	fprintf(out, ";[EXP]\n");
	memoriaTemp = 0;

	ExpA(tipo, endereco);

	while(token == "comp"){
		string lexe = lexema;

		CasaToken("comp");
		int *tipoB = new int(-1);
		int *endB = new int(-1);
		ExpA(tipoB, endB);

		if(*tipo != *tipoB){
			if(!((*tipo == TIPO_INTEIRO || *tipo == TIPO_BYTE) && (*tipoB == TIPO_INTEIRO || *tipoB == TIPO_BYTE))){
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
		}
		else{
			if(*tipo == TIPO_STRING && *tipoB == TIPO_STRING && lexe != "=="){
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
		}

		*tipo = TIPO_LOGICO;
	}

	fprintf(out, ";[fim EXP]\n");
}

// ExpA → [(+|-)] ExpB { (+|-|or) ExpB }
bool AnalisadorSintatico::ExpA(int *tipo, int *endereco){
	bool negativo = false;
	bool converterA = false;

	if(token == "+"){
		CasaToken("+");
	}
	else if(token == "-"){
		CasaToken("-");
		negativo = true;
	}

	ExpB(tipo, endereco);

	if(negativo){
		if(*tipo == TIPO_STRING || *tipo == TIPO_LOGICO){
			printf("%d:tipos incompatíveis.\n", linha);
			exit(0);
		}
		else if(*tipo == TIPO_BYTE){
			*tipo = TIPO_INTEIRO;
			converterA = true;
		}
	}

	while(token == "+" || token == "-" || token == "or"){
		bool soma = false;

		string lexe = token;

		if(token == "+"){
			CasaToken("+");
			soma = true;
		}
		else if(token == "-") CasaToken("-");
		else if(token == "or") CasaToken("or");

		int *tipoB = new int(-1);
		int *endB = new int(-1);
		ExpB(tipoB, endB);

		bool converterB = false;

		if(*tipo != *tipoB){
			if((*tipo != TIPO_BYTE && *tipo != TIPO_INTEIRO) || (*tipoB != TIPO_BYTE && *tipoB != TIPO_INTEIRO)) {
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
			else if(*tipoB == TIPO_BYTE){
				*tipo = TIPO_INTEIRO;
				converterB = true;
			}
		}
		else{
			if(*tipo == TIPO_STRING && *tipoB == TIPO_STRING && soma == false){
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
		}

		if(*tipo == TIPO_LOGICO || *tipoB == TIPO_LOGICO){
			printf("%d:tipos incompatíveis.\n", linha);
			exit(0);
		}

		if(*tipo != TIPO_STRING){
			std::stringstream ssA, ssB;
			ssA << std::uppercase << std::hex << *endereco;
			ssB << std::uppercase << std::hex << *endB;

			string regA, regB;

			if(*tipo == TIPO_INTEIRO){
				if(converterA){
					regA = "AL";
				}
				else{
					regA = "AX";
				}
			}
			else{
				regA = "AL";
			}

			if(*tipoB == TIPO_INTEIRO){
				if(converterB){
					regB = "BL";
				}
				else{
					regB = "BX";
				}
			}
			else{
				regB = "BL";
			}

			fprintf(out, "	;[soma]\n");

			if(negativo){
				int novoNeg = NovoTemp(2);

				std::stringstream ssC;
				ssC << std::uppercase << std::hex << novoNeg;

				if(converterA){
					fprintf(out, "	mov AH, 0\n");
				}

				fprintf(out, "	mov %s, DS:[%sh]\n", regA.c_str(), ssA.str().c_str());
				fprintf(out, "	neg AX\n");
				fprintf(out, "	mov DS:[%sh], AX\n", ssC.str().c_str());

				*endereco = novoNeg;
				ssA.str("");
				ssA << ssC.str();
			}

			fprintf(out, "	mov %s, DS:[%sh]\n", regA.c_str(), ssA.str().c_str());

			if(converterA){
				fprintf(out, "	mov AH, 0\n");
				regA = "AX";
			}

			fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssB.str().c_str());

			if(converterB){
				fprintf(out, "	mov BH, 0\n");
				regB = "BX";
			}

			if(lexe == "+"){
				fprintf(out, "	add %s, %s\n", regA.c_str(), regB.c_str());
			}
			else if(lexe == "-"){
				fprintf(out, "	sub %s, %s\n", regA.c_str(), regB.c_str());
			}

			ssA.str("");

			if(*tipo == TIPO_INTEIRO){
				*endereco = NovoTemp(2);
			}
			else if(*tipo == TIPO_STRING){
				*endereco = NovoTemp(256);
			}
			else{
				*endereco = NovoTemp(1);
			}

			ssA << std::uppercase << std::hex << *endereco;

			fprintf(out, "	mov DS:[%sh], %s\n", ssA.str().c_str(), regA.c_str());
			fprintf(out, "	;[fim soma]\n");
		}
	}
}

// ExpB → ExpC {(*|/|and) ExpC}
bool AnalisadorSintatico::ExpB(int *tipo, int *endereco){
	ExpC(tipo, endereco);

	while(token == "*" || token=="/" || token=="and"){
		string lexe = token;

		if(token == "*") CasaToken("*");
		else if(token == "/") CasaToken("/");
		else if(token == "and") CasaToken("and");

		int *tipoB = new int(-1);
		int *endB = new int(-1);
		ExpC(tipoB, endB);

		if(lexe == "/"){
			if(*tipo == TIPO_BYTE || *tipo == TIPO_INTEIRO){
				*tipo = TIPO_INTEIRO;
			}
			else{
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}

			if(*tipoB == TIPO_BYTE || *tipoB == TIPO_INTEIRO){
				*tipoB = TIPO_INTEIRO;
			}
			else{
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
		}

		if(lexe == "and"){
			if(*tipo != TIPO_LOGICO || *tipoB != TIPO_LOGICO){
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
		}

		if(*tipo != *tipoB){
			if((*tipo != TIPO_BYTE && *tipo != TIPO_INTEIRO) || (*tipoB != TIPO_BYTE && *tipoB != TIPO_INTEIRO)) {
				printf("%d:tipos incompatíveis.\n", linha);
				exit(0);
			}
			else{
				*tipo = TIPO_INTEIRO;
			}
		}

		std::stringstream ssA, ssB;
		std::locale loc;
		ssA << std::uppercase << std::hex << *endereco;
		ssB << std::uppercase << std::hex << *endB;

		string regA, regB;

		if(*tipo == TIPO_INTEIRO){
			regA = "AX";
		}
		else{
			regA = "AL";
		}

		if(*tipoB == TIPO_INTEIRO){
			regB = "BX";
		}
		else{
			regB = "BL";
		}

		fprintf(out, "	mov %s, DS:[%sh]\n", regA.c_str(), ssA.str().c_str());
		fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssB.str().c_str());
		// TODO: Verificar necessidade de conversão?
		if(lexe == "and"){
			fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssA.str().c_str());
		}
		else if(lexe == "*"){
			fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssA.str().c_str());
		}
		else if(lexe == "/"){
			fprintf(out, "	mov %s, DS:[%sh]\n", regB.c_str(), ssA.str().c_str());
		}

		ssA.str("");

		if(*tipo == TIPO_INTEIRO){
			*endereco = NovoTemp(2);
		}
		else if(*tipo == TIPO_STRING){
			*endereco = NovoTemp(256);
		}
		else{
			*endereco = NovoTemp(1);
		}

		ssA << std::uppercase << std::hex << *endereco;

		fprintf(out, "	mov DS:[%sh], %s\n", ssA.str().c_str(), regA.c_str());
	}
}

// ExpC → [not]ExpD
bool AnalisadorSintatico::ExpC(int *tipo, int *endereco){
	bool negado = false;

	if(token == "not"){
		CasaToken("not");
		negado = true;
	}
	ExpD(tipo, endereco);

	if(negado){
		if(*tipo != TIPO_LOGICO) {
			printf("%d:tipos incompatíveis.\n", linha);
			exit(0);
		}

		std::stringstream ss;
		std::locale loc;
		ss << std::uppercase << std::hex << *endereco;

		fprintf(out, "	mov AL, DS:[%sh]\n", ss.str().c_str());
		fprintf(out, "	not AL\n");
		fprintf(out, "	mov DS:[%sh], AL\n", ss.str().c_str());
	}
}

// ExpD → '('Exp')' | id | const
bool AnalisadorSintatico::ExpD(int *tipo, int *endereco){
	if(token == "("){
		CasaToken("(");
		Exp(tipo, endereco);
		CasaToken(")");
	}
	else if(token == "id"){
		if(lex->t->FindSimbolo(lexema) == "NULL"){
			printf("%d:identificador nao declarado [%s].\n", linha, lexema.c_str());
			exit(0);
		}

		*tipo = lex->t->GetSimbolo(lexema)->tipo;
		*endereco = lex->t->GetSimbolo(lexema)->endereco;
		CasaToken("id");
	}
	else{
		*tipo = tipo_constante;

		std::stringstream ss;
		std::locale loc;

		if(tipo_constante == TIPO_STRING){
			ss << std::uppercase << std::hex << memoriaTemp;

			*endereco = NovoTemp(256);

			fprintf(out, "	mov DS:[%sh], \"%s$\"; (Exp) string em %sh\n", ss.str().c_str(), lexema.c_str(), ss.str().c_str());

			memoria += lexema.length() + 1;
		}
		else{
			ss << std::uppercase << std::hex << memoriaTemp;

			if(tipo_constante == TIPO_INTEIRO){
				*endereco = NovoTemp(2);
				fprintf(out, "	mov AX, %s\n", lexema.c_str());
				fprintf(out, "	mov DS:[%sh], AX\n", ss.str().c_str());
			}
			else{
				*endereco = NovoTemp(1);
				fprintf(out, "	mov AL, %s\n", lexema.c_str());
				fprintf(out, "	mov DS:[%sh], AL\n", ss.str().c_str());
			}
		}

		CasaToken("const");
	}
}
