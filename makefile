# GCC Makefile for compilation tests
CC = gcc

PREFIX := ../$(shell awk -F'=' /^ID=/'{print $$2}' /etc/os-release | \
	 sed -e 's/"//g')

ifeq ($(PREFIX), ../msys2)
ALLTARGETS = em6502
else
ALLTARGETS = em6502
endif

OPTS = -g -Wall

em6502: em6502.o cpu.o instructions.o membus.o
	$(CC) $(OPTS) -o em6502 em6502.o cpu.o instructions.o membus.o

em6502.o: em6502.c em6502.h version.h
	$(CC) $(OPTS) -c em6502.c

cpu.o: cpu.c cpu.h
	$(CC) $(OPTS) -c cpu.c

instructions.o: instructions.c instructions.h
	$(CC) $(OPTS) -c instructions.c

membus.o: membus.c membus.h
	$(CC) $(OPTS) -c membus.c

all: $(ALLTARGETS)

install: all
	cp em6502 $(PREFIX)/bin
ifeq ($(PREFIX), ../msys2)

endif

clean: 
	-rm -f *.o *.exe *.gch $(ALLTARGETS)
