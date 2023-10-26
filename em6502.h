// em6502.h
//
// Definitions and function prototypes for 6502 emulator program
//
// Brian K. Niece

#include "cpu.h"

#ifndef EM6502_H
#define EM6502_H

// Definitions for emulator defaults
#define MAX_MEM 1024 * 64
#define DEF_DATA_ADDR 0x0200
#define DEF_CODE_ADDR 0x0600

// Memory management funtions
int import_mem(char *filename, byte *mem, word addr);
	// Returns 0 on success
void print_mem_page(byte *mem, word addr, int mark);

// Get array of instruction functions
extern int (*execute[])(CPU *cpu);

#endif
