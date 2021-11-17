# C Compiler
CC = gcc

# Compile time flags
CFLAGS = -Wall -Wextra -pedantic

# Files used for the main program
CFILES = smsz80.c sms_runner.c mem.c z80.c
CDEPS := $(CFILES) mem.h smsz80_common.h sms_runner.h z80.h

.PHONY: clean
.DEFAULT_GOAL: smsz80

smsz80: $(CDEPS)
	$(CC) $(CFILES) -o smsz80 $(CFLAGS)

clean:
	$(RM) *.o smsz80