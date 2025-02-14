CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: smc

smc: main.o instructions.o 
	${CC} main.o instructions.o -lm -o smc

main.o: main.c
	${CC} ${CFLAGS} main.c

instructions.o: instructions.c
	${CC} ${CFLAGS} instructions.c

clean:
	rm -f *.o *~

