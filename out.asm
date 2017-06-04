dseg SEGMENT PUBLIC; inicio seg. dados;
	byte 4000h DUP(?); temporarios
	sword ?; inteiro [A] em 4000h
	sword ?; inteiro [B] em 4002h
	byte "Oi, mundo.$"; byte [C] em 4004h
	byte 100h DUP(?); string [D] em 400Fh
	byte ?; byte [E] em 410Fh
	byte ?; boolean [F] em 4110h
	byte ?; boolean [G] em 4111h
	byte ?; boolean [H] em 4112h
	sword ?; inteiro [n] em 4112h
	byte 100h DUP(?); string [nome] em 4114h
	byte ?; boolean [naoTerminou] em 4213h
	byte ?; byte [MAXITER] em 4215h
	sword ?; inteiro [x] em 4215h
	sword ?; inteiro [a] em 4217h
	sword ?; inteiro [y] em 4219h
	sword ?; inteiro [b] em 421Bh
	sword ?; inteiro [c] em 421Dh
	sword ?; inteiro [w] em 421Fh
dseg ENDS; fim seg. dados
