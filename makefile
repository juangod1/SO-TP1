####################################
####         SETUP              ####
####################################

CC = gcc
CC_FLAGS = -Wall

SOURCES = $(wildcard *.c)
SOURCES_AUX := $(SOURCES)
SOURCES = $(filter-out $(SECONDARY_BINARIES_SOURCES),$(SOURCES_AUX)) # Removes source files that do not belong to the main process

OBJECTS = $(foreach source, $(SOURCES:.c=.o), Binaries/$(source)) # Appends string for object files to be stored in Binaries directory

EXEC = run

SECONDARY_BINARIES_SOURCES = # Source files that do not belong to the main process
SECONDARY_BINARIES_OBJECTS = $(foreach source, $(SECONDARY_BINARIES_SOURCES:.c=.o), Binaries/$(source)) # Same as line 12


####################################
####      COMPILATION           ####
####################################

Binaries/%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

all: clean main_binary secondary_binaries

clean:
	rm -f Binaries/*

main_binary: clean $(OBJECTS)
			$(CC) $(OBJECTS) -o Binaries/$(EXEC)

secondary_binaries: $(SECONDARY_BINARIES_OBJECTS) # Iterates over binaries that do no tbelong to the main process compiling them separately
	@- $(foreach file,$(SECONDARY_BINARIES_OBJECTS), \
					echo Compiled $(file) as a separate binary file: $(subst .o,,$(file));\
					$(CC) -o $(subst .o,,$(file)) $(file) ; \
	    )