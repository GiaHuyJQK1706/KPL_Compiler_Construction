CC = gcc
CFLAGS = -Wall -g
TARGET = Warm_up

FILESRC = main.c tree.c

FILEO = $(FILESRC:.c=.o)

all: $(TARGET)

$(TARGET): $(FILEO)
	$(CC) $(CFLAGS) -o $(TARGET) $(FILEO)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(FILEO) $(TARGET)