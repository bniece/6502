// 6502insructions.h
//
// Definitions and function prototypes for 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece

#include "em6502.h"

#ifndef INST6502_H
#define INST6502_H

void do_LDA_imm(CPU *cpu, byte *mem);
void do_NOP_impl(CPU *cpu, byte *mem);

#endif
