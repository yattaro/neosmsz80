# C Compiler
CC = gcc

# Compile time flags
CFLAGS = -Wall -Wextra -Werror -pedantic

# Files used for the main program
CFILES = smsz80.c mem.c
CDEPS := $(CFILES) mem.h smsz80_common.h

.PHONY: clean
.DEFAULT_GOAL: smsz80

smsz80: $(CDEPS)
	$(CC) $(CFILES) -o smsz80 $(CFLAGS)

clean:
	$(RM) *.o smsz80