####################################
####         SETUP              ####
####################################

CC = gcc
CC_FLAGS = -Wall -pthread
LINKER_OPTIONS = -lrt -lm -g -pthread

SOURCES = $(wildcard *.c)
SOURCES_AUX := $(SOURCES)
SOURCES = $(filter-out $(VIEW),$(filter-out $(SLAVE),$(SOURCES_AUX))) # Removes source files that do not belong to the main process
OBJECTS = $(foreach source, $(SOURCES:.c=.o), Binaries/$(source)) # Appends string for object files to be stored in Binaries directory
EXEC = run # Binary name

SLAVE = slave.c Tests/testSlave.c
SLAVE_OBJECTS = $(foreach source, $(SLAVE:.c=.o), Binaries/$(source)) # Same as line 12
EXEC_SLAVE = slave # Binary name

VIEW = view.c
VIEW_OBJECTS = $(foreach source, $(VIEW:.c=.o), Binaries/$(source)) # Same as line 12
EXEC_VIEW = view # Binary name

SOURCES_TEST = $(wildcard Tests/*.c)
SOURCES_TEST_AUX := $(SOURCES_TEST)
SOURCES_TEST = $(subst Tests/,,$(SOURCES_TEST_AUX))     #$(foreach source, $(SOURCES_TEST_AUX:Tests/=), $(source))
OBJECTS_TEST = $(foreach source, $(SOURCES_TEST:.c=.o), Binaries/$(source))

####################################
####      COMPILATION           ####
####################################

Binaries/%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

Binaries/%.o: Tests/%.c
	$(CC) -c $(CC_FLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

all: binaries_setup hashdump_setup clean main_binary slave view

clean:
	rm -f Binaries/*

main_binary: clean $(OBJECTS) $(OBJECTS_TEST)
	$(CC) $(OBJECTS) $(OBJECTS_TEST) -o Binaries/$(EXEC) $(LINKER_OPTIONS)

slave: $(SLAVE_OBJECTS)
	$(CC) $(SLAVE_OBJECTS) Binaries/testLib.o Binaries/messageQueue.o -o Binaries/$(EXEC_SLAVE) $(LINKER_OPTIONS)

view: $(VIEW_OBJECTS)
	$(CC) $(VIEW_OBJECTS) Binaries/testLib.o Binaries/messageQueue.o -o Binaries/$(EXEC_VIEW) $(LINKER_OPTIONS)

binaries_setup:
	if [ -d "Binaries" ]; then echo "Binaries directory found, proceeding..."; else mkdir Binaries; fi

hashdump_setup:
	if [ -d "HashDump" ]; then echo "HashDump directory found, proceeding..."; else mkdir HashDump; fi
