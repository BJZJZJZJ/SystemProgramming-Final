CC = gcc

all : main.o
	gcc -o app main.o

main.o : method.h header.h main.c
	gcc -c -o main.o main.c

clean :
	rm -f *.o
