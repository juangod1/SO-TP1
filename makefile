CC = gcc
CC_FLAGS = -Wall

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
SOURCES_TEST = $(wildcard Tests/*.c)
OBJECTS_TEST = $(SOURCES_TEST:.c=.o)
EXEC = BINARY_NAME_HERE

all: clean test

no_test: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)
 
clean:
	rm -f $(EXEC) $(OBJECTS)

test: clean $(OBJECTS) $(OBJECTS_TEST)
	$(CC) $(OBJECTS) $(OBJECTS_TEST) -o $(EXEC)

%.o: %.c Test/%.c
	$(CC) -c $(CC_FLAGS) $< -o $@
