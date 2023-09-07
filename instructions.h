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
void do_BRK_impl(CPU *cpu);	// 0x00
void do_CLC_impl(CPU *cpu);	// 0x18
void do_SEC_impl(CPU *cpu);	// 0x38
void do_ADC_zpg(CPU *cpu);		// 0x65
void do_ADC_imm(CPU *cpu);		// 0x69
void do_STA_zpg(CPU *cpu);		// 0x85
void do_TXA_impl(CPU *cpu);	// 0x8A
void do_STA_abs(CPU *cpu);		// 0x8D
void do_TYA_impl(CPU *cpu);	// 0x98
void do_TAY_impl(CPU *cpu);	// 0xA8
void do_LDA_imm(CPU *cpu);		// 0xA9
void do_TAX_impl(CPU *cpu);	// 0xAA
void do_INY_impl(CPU *cpu);	// 0xC8
void do_SBC__zpg(CPU *cpu);	// 0xE5
void do_INX_impl(CPU *cpu);	// 0xE8
void do_SBC__imm(CPU *cpu);	// 0xE9
void do_NOP_impl(CPU *cpu);	// 0xEA

#endif
