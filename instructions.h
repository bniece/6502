// 6502insructions.h
//
// Definitions and function prototypes for 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece

#include "em6502.h"

#ifndef INST6502_H
#define INST6502_H

// Logging functions
void log_op_start(CPU *cpu, char *op, int bytes);
void lob_op_end(CPU *cpu, byte result, int cycles);

// Instruction handler functions
void do_BRK_impl(CPU *cpu);
void do_ADC_imm(CPU *cpu);
void do_STA_abs(CPU *cpu);
void do_LDA_imm(CPU *cpu);
void do_TAX_impl(CPU *cpu);
void do_INX_impl(CPU *cpu);
void do_NOP_impl(CPU *cpu);

#endif
