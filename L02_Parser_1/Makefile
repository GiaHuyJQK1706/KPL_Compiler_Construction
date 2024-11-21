CFLAGS = -c -Wall
CC = gcc
LIBS = -lm
OBJ = scanner.o reader.o charcode.o token.o error.o parser.o main.o
TARGET = parser

all: $(TARGET)

debug: CFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ $(TARGET)

clean-window:
	del /Q *.o $(TARGET)