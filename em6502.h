// em6502.h
//
// Definitions and function prototypes for 6502 emulator program
//
// Brian K. Niece

#include "cpu.h"
#include "instructions.h"
#include "membus.h"

#ifndef EM6502_H
#define EM6502_H

// Definitions for emulator defaults
#define DEF_DATA_ADDR 0x0200
#define DEF_CODE_ADDR 0x0600

// Get array of instruction functions
extern struct opreturn (*execute[])(CPU *cpu);

// IO functions
void print_registers(CPU *cpu);
void print_mem_page(membus *mem, word addr, int mark);
void log_PC(CPU *cpu);
void log_op(CPU *cpu, struct opreturn opr);

#endif
