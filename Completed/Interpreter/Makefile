CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: kplrun

kplrun: main.o instructions.o VM.o
	${CC} main.o instructions.o VM.o -lm -lncurses -o kplrun

main.o: main.c
	${CC} ${CFLAGS} main.c

instructions.o: instructions.c
	${CC} ${CFLAGS} instructions.c

VM.o: VM.c
	${CC} ${CFLAGS} VM.c

clean:
	rm -f *.o *~

