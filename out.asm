dseg SEGMENT PUBLIC; inicio seg. dados;
	byte 4000h DUP(?); temporarios
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [A] em 4000h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [B] em 4002h
dseg ENDS;
;[EXP]
dseg SEGMENT PUBLIC
	byte "Oi, mundo.$"; (Exp) string em 4004h
dseg ENDS
;[fim EXP]
dseg SEGMENT PUBLIC
	byte "Oi, mundo.$"; byte [C] em 400Fh
dseg ENDS;
	mov BL, DS:[4004]
	mov DS:[400F], BL
dseg SEGMENT PUBLIC
	byte 100h DUP(?); string [D] em 401Ah
dseg ENDS;
;[EXP]
	mov AL, 1
	mov DS:[0], AL
;[fim EXP]
dseg SEGMENT PUBLIC
	byte ?; byte [E] em 4119h
dseg ENDS;
	mov BL, DS:[0]
	mov DS:[4119], BL
;[EXP]
;[fim EXP]
dseg SEGMENT PUBLIC
	byte ?; boolean [F] em 411Ah
dseg ENDS;
	mov BL, DS:[0]
	mov DS:[411A], BL
;[EXP]
;[fim EXP]
dseg SEGMENT PUBLIC
	byte ?; boolean [G] em 411Bh
dseg ENDS;
	mov BL, DS:[1]
	mov DS:[411B], BL
;[EXP]
;[fim EXP]
dseg SEGMENT PUBLIC
	byte ?; boolean [H] em 411Ch
dseg ENDS;
	mov BL, DS:[0]
	mov DS:[411C], BL
dseg SEGMENT PUBLIC
	byte ?; boolean [n] em 411Dh
dseg ENDS;
dseg SEGMENT PUBLIC
	byte 100h DUP(?); string [nome] em 411Eh
dseg ENDS;
dseg SEGMENT PUBLIC
	byte ?; boolean [naoTerminou] em 421Dh
dseg ENDS;
;[EXP]
	mov AL, 10
	mov DS:[0], AL
;[fim EXP]
dseg SEGMENT PUBLIC
	byte ?; byte [MAXITER] em 421Eh
dseg ENDS;
	mov BL, DS:[0]
	mov DS:[421E], BL
dseg SEGMENT PUBLIC
	byte ?; boolean [x] em 421Fh
dseg ENDS;
;[EXP]
;[fim EXP]
	mov BL, DS:[0]
	mov DS:[421F], BL
dseg SEGMENT PUBLIC
	sword ?; inteiro [a] em 4220h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [y] em 4222h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [b] em 4224h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [c] em 4226h
dseg ENDS;
dseg SEGMENT PUBLIC
	sword ?; inteiro [w] em 4228h
dseg ENDS;
;[EXP]
	mov AL, 1
	mov DS:[0], AL
;[fim EXP]
	mov BX, DS:[0]
	mov DS:[4228], BX
;[Att]
;[EXP]
	mov AL, 5
	mov DS:[0], AL
;[fim EXP]
	mov BH, 0
	mov BL, DS:[0]
	mov DS:[0], BX
;[EXP]
dseg SEGMENT PUBLIC
	byte "Digite seu nome: $"; (Exp) string em 422Ah
dseg ENDS
;[fim EXP]
;[Att]
;[EXP]
;[fim EXP]
	mov BL, DS:[0]
	mov DS:[411D], BL
;[Att]
;[EXP]
	mov AL, DS:[411D]
	not AL
	mov DS:[411D], AL
;[fim EXP]
	mov BL, DS:[411D]
	mov DS:[0], BL
;[Att]
;[EXP]
	mov AL, 0
	mov DS:[0], AL
	mov AL, 2
	mov DS:[1], AL
	mov AL, 2
	mov DS:[2], AL
	mov AL, DS:[1]
	mov BL, DS:[2]
	mov BL, DS:[1]
	mov DS:[1], AL
;[EXP]
	mov AL, 3
	mov DS:[0], AL
	mov AL, 3
	mov DS:[1], AL
	mov AL, DS:[0]
	mov BL, DS:[1]
	mov BL, DS:[0]
	mov DS:[0], AL
	mov AL, 2
	mov DS:[2], AL
	mov AX, DS:[0]
	mov BX, DS:[2]
	mov BX, DS:[0]
	mov DS:[0], AX
	mov AL, 2
	mov DS:[3], AL
;[fim EXP]
;[fim EXP]
	mov BX, DS:[0]
	mov DS:[0], BX
;[if]
;[EXP]
	mov AL, DS:[411D]
	mov BL, DS:[421F]
	mov BL, DS:[411D]
	mov DS:[411D], AL
;[fim EXP]
	mov AL, DS:[411D]
	cmp AL, FFh
	jne R0
;[Att]
;[EXP]
;[fim EXP]
	mov BL, DS:[0]
	mov DS:[0], BL
	jmp R1
	R0:
;[Att]
;[EXP]
;[fim EXP]
	mov BL, DS:[1]
	mov DS:[0], BL
	R1:
;[if fim]
;[if]
;[EXP]
;[fim EXP]
	mov AL, DS:[421F]
	cmp AL, FFh
	jne R2
;[if]
;[EXP]
	mov AL, DS:[421F]
	mov BL, DS:[411D]
	mov BL, DS:[421F]
	mov DS:[421F], AL
;[fim EXP]
	mov AL, DS:[421F]
	cmp AL, FFh
	jne R4
;[Att]
;[EXP]
	mov AL, 3
	mov DS:[0], AL
;[fim EXP]
	mov BH, 0
	mov BL, DS:[0]
	mov DS:[0], BX
	jmp R5
	R4:
	R5:
;[if fim]
	jmp R3
	R2:
	R3:
;[if fim]
;[While]
R6:
;[EXP]
;[fim EXP]
	mov AL, DS:[421D]
	cmp AL, FFh
	jne R7
;[EXP]
dseg SEGMENT PUBLIC
	byte "Ola' $"; (Exp) string em 423Ch
dseg ENDS
;[fim EXP]
;[EXP]
;[fim EXP]
;[Att]
;[EXP]
;[fim EXP]
	mov BL, DS:[1]
	mov DS:[0], BL
;[if]
;[EXP]
;[fim EXP]
	mov AL, DS:[411E]
	cmp AL, FFh
	jne R8
	jmp R9
	R8:
	R9:
;[if fim]
;[Att]
;[EXP]
;[EXP]
;[fim EXP]
;[fim EXP]
	mov BL, DS:[4222]
	mov DS:[0], BL
R7:
;[While fim]
