qwirkle.o : qwirkle.c
	gcc -o qwirkle.o -c qwirkle.c

funcoes.o : funcoes.c
	gcc -o funcoes.o -c funcoes.c

qwirkle : qwirkle.o funcoes.o
	gcc -o qwirkle qwirkle.o funcoes.o

all : qwirkle

run : qwirkle
	./qwirkle

clean :
	rm qwirkle qwirkle.o funcoes.o


