dseg SEGMENT PUBLIC; inicio seg. dados;
	byte 4000h DUP(?); temporarios
dseg ENDS;
cseg SEGMENT PUBLIC; inicio do seg. código
ASSUME CS:cseg, DS:dseg
_strt: ; inicio do programa
mov ax, dseg
mov ds, ax
dseg SEGMENT PUBLIC
	sword ?; inteiro [A] em 4000h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [B] em 4002h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [C] em 4004h
dseg ENDS;
;[Att]
;[EXP]
	mov AL, 0
	mov DS:[0h], AL
;[fim EXP]
	mov BH, 0
	mov BL, DS:[0h]
	mov DS:[4000h], BX
;[Att]
;[EXP]
	mov AL, 2
	mov DS:[0h], AL
	;[soma]
	mov AX, DS:[4000h]
	mov BL, DS:[0h]
	mov BH, 0
	add AX, BX
	mov DS:[1h], AX
	;[fim soma]
;[fim EXP]
	mov BX, DS:[1h]
	mov DS:[4002h], BX
;[Att]
;[EXP]
	mov AL, 36
	mov DS:[0h], AL
;[fim EXP]
	mov BH, 0
	mov BL, DS:[0h]
	mov DS:[4004h], BX
;[EXP]
;[fim EXP]
mov ax, DS:[4000h] ;numero
mov di, DS:[4000h]; end. string temp
mov cx, 0 ;contador
cmp ax,0 ;verifica sinal
jge R0  ;salta se número positivo
mov bl, 2Dh ;senão, escreve sinal –
mov ds:[di], bl
add di, 1 ;incrementa índice
neg ax  ;toma módulo do número
R0:
mov bx, 10 ;divisor
R1:
add cx, 1 ;incrementa contador
mov dx, 0 ;estende 32bits p/ div.
idiv bx   ;divide DXAX por BX
push dx  ;empilha valor do resto
cmp ax, 0 ;verifica se quoc. é 0
jne R1  ;se não é 0, continua
;agora, desemp. os valores e escreve o string
R2:
pop dx  ;desempilha valor
add dx, 30h ;transforma em caractere
mov ds:[di],dl ;escreve caractere
add di, 1 ;incrementa base
add cx, -1 ;decrementa contador
cmp cx, 0 ;verifica pilha vazia
jne R2  ;se não pilha vazia, loop
;grava fim de string
mov dl, 024h ;fim de string
mov ds:[di], dl ;grava $
;exibe string
mov dx, DS:[4000h]
mov ah, 09h
int 21h
;[EXP]
;[fim EXP]
mov ax, DS:[4002h] ;numero
mov di, DS:[4002h]; end. string temp
mov cx, 0 ;contador
cmp ax,0 ;verifica sinal
jge R3  ;salta se número positivo
mov bl, 2Dh ;senão, escreve sinal –
mov ds:[di], bl
add di, 1 ;incrementa índice
neg ax  ;toma módulo do número
R3:
mov bx, 10 ;divisor
R4:
add cx, 1 ;incrementa contador
mov dx, 0 ;estende 32bits p/ div.
idiv bx   ;divide DXAX por BX
push dx  ;empilha valor do resto
cmp ax, 0 ;verifica se quoc. é 0
jne R4  ;se não é 0, continua
;agora, desemp. os valores e escreve o string
R5:
pop dx  ;desempilha valor
add dx, 30h ;transforma em caractere
mov ds:[di],dl ;escreve caractere
add di, 1 ;incrementa base
add cx, -1 ;decrementa contador
cmp cx, 0 ;verifica pilha vazia
jne R5  ;se não pilha vazia, loop
;grava fim de string
mov dl, 024h ;fim de string
mov ds:[di], dl ;grava $
;exibe string
mov dx, DS:[4002h]
mov ah, 09h
int 21h
;[EXP]
;[fim EXP]
mov ax, DS:[4004h] ;numero
mov di, DS:[4004h]; end. string temp
mov cx, 0 ;contador
cmp ax,0 ;verifica sinal
jge R6  ;salta se número positivo
mov bl, 2Dh ;senão, escreve sinal –
mov ds:[di], bl
add di, 1 ;incrementa índice
neg ax  ;toma módulo do número
R6:
mov bx, 10 ;divisor
R7:
add cx, 1 ;incrementa contador
mov dx, 0 ;estende 32bits p/ div.
idiv bx   ;divide DXAX por BX
push dx  ;empilha valor do resto
cmp ax, 0 ;verifica se quoc. é 0
jne R7  ;se não é 0, continua
;agora, desemp. os valores e escreve o string
R8:
pop dx  ;desempilha valor
add dx, 30h ;transforma em caractere
mov ds:[di],dl ;escreve caractere
add di, 1 ;incrementa base
add cx, -1 ;decrementa contador
cmp cx, 0 ;verifica pilha vazia
jne R8  ;se não pilha vazia, loop
;grava fim de string
mov dl, 024h ;fim de string
mov ds:[di], dl ;grava $
;exibe string
mov dx, DS:[4004h]
mov ah, 09h
int 21h
mov ah, 4Ch; termina o programa
int 21h
cseg ENDS; fim seg. código
END _strt
