# MSVC makefile for compilation tests
CC = cl
LD = link

COPTS = /Zi /MDd /W4
LOPTS = /DEBUG

INCDIR = ..\msvc\include
LIBDIR = ..\msvc\lib

em6502: em6502.obj cpu.obj instructions.obj
	$(LD) $(LOPTS) /OUT:em6502.exe em6502.obj cpu.obj instructions.obj /LIBPATH:$(LIBDIR) getopt.lib

em6502.obj: em6502.c em6502.h version.h
	$(CC) $(COPTS) /I$(INCDIR) /c em6502.c

cpu.obj: cpu.c cpu.h
	$(CC) $(COPTS) /c cpu.c

instructions.obj: instructions.c instructions.h
	$(CC) $(COPTS) /c instructions.c

all: em6502

install: em6502
	copy em6502.exe ..\msvc\bin

clean: 
	del *.obj *.pdb *.ilk *.exe *.res *.manifest
