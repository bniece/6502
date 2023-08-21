// em6502.h
//
// Definitions and function prototypes for 6502 emulator program
//
// Brian K. Niece

#include "cpu.h"

#ifndef EM6502_H
#define EM6502_H

// Size definitions
#define MAX_MEM 1024 * 64

// Memory management funtions
void import_mem(char *filename, byte *addr);
void print_mem_page(byte *page, int mark);

// Get array of instruction functions
extern void (*execute[])(CPU *cpu, byte *mem);

#endif
