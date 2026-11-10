CC = gcc
GFLAGS = -std=c23 -Wall -Wextra -pedantic -fsanitize=address,undefined
TARGET = forth
SOURCES = main.c stack.c dictionary.c interpreter.c vocabulary.c error.c util.c

.PHOYY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(GFLAGS)

clean:
	$(RM) $(TARGET)
