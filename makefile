all:
	g++ src/*.cpp -o LC

debug:
	g++ src/*.cpp -D DEBUG -o LC

test:
	./LC exemplo.l
