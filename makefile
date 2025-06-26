CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET = tp4_parte2
OBJ_FILES = main.o huff.o bmh.o

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET)

main.o: main.c bmh.h huff.h
	$(CC) $(CFLAGS) -c $<

huff.o: huff.c huff.h
	$(CC) $(CFLAGS) -c $<

bmh.o: bmh.c bmh.h huff.h
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJ_FILES) $(TARGET)
.PHONY: all