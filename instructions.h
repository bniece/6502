// insructions.h
//
// Definitions and function prototypes for 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece

#ifndef INST6502_H
#define INST6502_H

#include "cpu.h"

// Return information structure
struct opreturn
{
	char *mnemonic;
	byte bytes;
	byte cycles;
	word operand;
	byte result;
};

// Logging functions
void log_op_start(CPU *cpu, char *op, int bytes);
void log_op_end(CPU *cpu, int address, byte result, int cycles);
void set_print_trace(int pt);

struct opreturn test_op();

// Instruction handler functions
struct opreturn do_BRK_impl(CPU *cpu);	// 0x00
struct opreturn do_ORA_Xind(CPU *cpu);	// 0x01
struct opreturn do_ORA_zpg(CPU *cpu);	// 0x05
struct opreturn do_ASL_zpg(CPU *cpu);	// 0x06
struct opreturn do_PHP_impl(CPU *cpu);	// 0x08
struct opreturn do_ORA_imm(CPU *cpu);	// 0x09
struct opreturn do_ASL_A(CPU *cpu);		// 0x0A
struct opreturn do_ORA_abs(CPU *cpu);	// 0x0D
struct opreturn do_ASL_abs(CPU *cpu);	// 0x0E
struct opreturn do_ORA_absY(CPU *cpu);	// 0x19
struct opreturn do_BPL_rel(CPU *cpu);	// 0x10
struct opreturn do_ORA_indY(CPU *cpu);	// 0x11
struct opreturn do_ORA_zpgR(CPU *cpu);	// 0x15
struct opreturn do_ASL_zpgX(CPU *cpu);	// 0x16
struct opreturn do_ORA_absX(CPU *cpu);	// 0x1D
struct opreturn do_ASL_absX(CPU *cpu);	// 0x1E
struct opreturn do_CLC_impl(CPU *cpu);	// 0x18
struct opreturn do_JSR_abs(CPU *cpu);	// 0x20
struct opreturn do_AND_Xind(CPU *cpu);	// 0x21
struct opreturn do_BIT_zpg(CPU *cpu);	// 0x24
struct opreturn do_AND_zpg(CPU *cpu);	// 0x25
struct opreturn do_ROL_zpg(CPU *cpu);	// 0x26
struct opreturn do_PLP_impl(CPU *cpu);	// 0x28
struct opreturn do_AND_imm(CPU *cpu);	// 0x29
struct opreturn do_ROL_A(CPU *cpu);		// 0x2A
struct opreturn do_BIT_abs(CPU *cpu);	// 0x2C
struct opreturn do_AND_abs(CPU *cpu);	// 0x2D
struct opreturn do_ROL_abs(CPU *cpu);	// 0x2E
struct opreturn do_BMI_rel(CPU *cpu);	// 0x30
struct opreturn do_AND_indY(CPU *cpu);	// 0x31
struct opreturn do_AND_zpgX(CPU *cpu);	// 0x35
struct opreturn do_ROL_zpgX(CPU *cpu);	// 0x36
struct opreturn do_SEC_impl(CPU *cpu);	// 0x38
struct opreturn do_AND_absY(CPU *cpu);	// 0x39
struct opreturn do_AND_absX(CPU *cpu);	// 0x3D
struct opreturn do_ROL_absX(CPU *cpu);	// 0x3E
struct opreturn do_RTI_impl(CPU *cpu);	// 0x40
struct opreturn do_EOR_Xind(CPU *cpu);	// 0x41
struct opreturn do_EOR_zpg(CPU *cpu);	// 0x45
struct opreturn do_LSR_zpg(CPU *cpu);	// 0x46
struct opreturn do_PHA_impl(CPU *cpu);	// 0x48
struct opreturn do_EOR_imm(CPU *cpu);	// 0x49
struct opreturn do_LSR_A(CPU *cpu);		// 0x4A
struct opreturn do_JMP_abs(CPU *cpu);	// 0x4C
struct opreturn do_EOR_abs(CPU *cpu);	// 0x4D
struct opreturn do_LSR_abs(CPU *cpu);	// 0x4E
struct opreturn do_BVC_rel(CPU *cpu);	// 0x50
struct opreturn do_EOR_indY(CPU *cpu);	// 0x51
struct opreturn do_EOR_zpgX(CPU *cpu);	// 0x55
struct opreturn do_LSR_zpgX(CPU *cpu);	// 0x56
struct opreturn do_PHA_impl(CPU *cpu);	// 0x48
struct opreturn do_CLI_impl(CPU *cpu);	// 0x58
struct opreturn do_EOR_absY(CPU *cpu);	// 0x59
struct opreturn do_EOR_absX(CPU *cpu);	// 0x5D
struct opreturn do_LSR_absX(CPU *cpu);	// 0x5E
struct opreturn do_RTS_impl(CPU *cpu);	// 0x60
struct opreturn do_ADC_Xind(CPU *cpu);	// 0x61
struct opreturn do_ADC_zpg(CPU *cpu);	// 0x65
struct opreturn do_PLA_impl(CPU *cpu);	// 0x68
struct opreturn do_ADC_imm(CPU *cpu);	// 0x69
struct opreturn do_ROR_A(CPU *cpu);		// 0x6A
struct opreturn do_JMP_ind(CPU *cpu);	// 0x6C
struct opreturn do_ADC_abs(CPU *cpu);	// 0x6D
struct opreturn do_BVS_rel(CPU *cpu);	// 0x70
struct opreturn do_ADC_indY(CPU *cpu);	// 0x71
struct opreturn do_ADC_zpgX(CPU *cpu);	// 0x75
struct opreturn do_SEI_impl(CPU *cpu);	// 0x78
struct opreturn do_ADC_absY(CPU *cpu);	// 0x79
struct opreturn do_ADC_absX(CPU *cpu);	// 0x7D
struct opreturn do_STA_Xind(CPU *cpu);	// 0x81
struct opreturn do_STY_zpg(CPU *cpu);	// 0x84
struct opreturn do_STA_zpg(CPU *cpu);	// 0x85
struct opreturn do_STX_zpg(CPU *cpu);	// 0x86
struct opreturn do_DEY_impl(CPU *cpu);	// 0x88
struct opreturn do_TXA_impl(CPU *cpu);	// 0x8A
struct opreturn do_STY_abs(CPU *cpu);	// 0x8c
struct opreturn do_STA_abs(CPU *cpu);	// 0x8D
struct opreturn do_STX_abs(CPU *cpu);	// 0x8E
struct opreturn do_BCC_rel(CPU *cpu);	// 0x90
struct opreturn do_STA_indY(CPU *cpu);	// 0x91
struct opreturn do_STY_zpgX(CPU *cpu);	// 0x94
struct opreturn do_STA_zpgX(CPU *cpu);	// 0x95
struct opreturn do_STX_zpgY(CPU *cpu);	// 0x96
struct opreturn do_TYA_impl(CPU *cpu);	// 0x98
struct opreturn do_STA_absY(CPU *cpu);	// 0x99
struct opreturn do_TXS_impl(CPU *cpu);	// 0x9A
struct opreturn do_STA_absX(CPU *cpu);	// 0x9D
struct opreturn do_LDY_imm(CPU *cpu);	// 0xA0
struct opreturn do_LDA_Xind(CPU *cpu);	// 0xA1
struct opreturn do_LDX_imm(CPU *cpu);	// 0xA2
struct opreturn do_LDY_zpg(CPU *cpu);	// 0xA4
struct opreturn do_LDA_zpg(CPU *cpu);	// 0xA5
struct opreturn do_LDX_zpg(CPU *cpu);	// 0xA6
struct opreturn do_TAY_impl(CPU *cpu);	// 0xA8
struct opreturn do_LDA_imm(CPU *cpu);	// 0xA9
struct opreturn do_TAX_impl(CPU *cpu);	// 0xAA
struct opreturn do_LDY_abs(CPU *cpu);	// 0xAC
struct opreturn do_LDA_abs(CPU *cpu);	// 0xAD
struct opreturn do_LDX_abs(CPU *cpu);	// 0xAE
struct opreturn do_LDA_indY(CPU *cpu);	// 0xB1
struct opreturn do_BCS_rel(CPU *cpu);	// 0xB0
struct opreturn do_LDY_zpgX(CPU *cpu);	// 0xB4
struct opreturn do_LDA_zpgX(CPU *cpu);	// 0xB5
struct opreturn do_LDX_zpgY(CPU *cpu);	// 0xB6
struct opreturn do_CLV_impl(CPU *cpu);	// 0xB8
struct opreturn do_LDA_absY(CPU *cpu);	// 0xB9
struct opreturn do_LDY_absX(CPU *cpu);	// 0xBC
struct opreturn do_LDA_absX(CPU *cpu);	// 0xBD
struct opreturn do_LDX_absY(CPU *cpu);	// 0xBE
struct opreturn do_CPY_imm(CPU *cpu);	// 0xC0
struct opreturn do_CMP_Xind(CPU *cpu);	// 0xC1
struct opreturn do_CPY_zpg(CPU *cpu);	// 0xC4
struct opreturn do_CMP_zpg(CPU *cpu);	// 0xC5
struct opreturn do_DEC_zpg(CPU *cpu);	// 0xC6
struct opreturn do_INY_impl(CPU *cpu);	// 0xC8
struct opreturn do_CMP_imm(CPU *cpu);	// 0xC9
struct opreturn do_DEX_impl(CPU *cpu);	// 0xCA
struct opreturn do_CPY_abs(CPU *cpu);	// 0xCC
struct opreturn do_CMP_abs(CPU *cpu);	// 0xCD
struct opreturn do_DEC_abs(CPU *cpu);	// 0xCE
struct opreturn do_BNE_rel(CPU *cpu);	// 0xD0
struct opreturn do_CMP_indY(CPU *cpu);	// 0xD1
struct opreturn do_CMP_zpgX(CPU *cpu);	// 0xD5
struct opreturn do_DEC_zpgX(CPU *cpu);	// 0xD6
struct opreturn do_CLD_impl(CPU *cpu);	// 0xD8
struct opreturn do_CMP_absY(CPU *cpu);	// 0xD9
struct opreturn do_CMP_absX(CPU *cpu);	// 0xDD
struct opreturn do_DEC_absX(CPU *cpu);	// 0xDE
struct opreturn do_CPX_imm(CPU *cpu);	// 0xE0
struct opreturn do_SBC_Xind(CPU *cpu);	// 0xE1
struct opreturn do_CPX_zpg(CPU *cpu);	// 0xE4
struct opreturn do_SBC_zpg(CPU *cpu);	// 0xE5
struct opreturn do_INC_zpg(CPU *cpu);	// 0xE6
struct opreturn do_INX_impl(CPU *cpu);	// 0xE8
struct opreturn do_SBC_imm(CPU *cpu);	// 0xE9
struct opreturn do_NOP_impl(CPU *cpu);	// 0xEA
struct opreturn do_CPX_abs(CPU *cpu);	// 0xEC
struct opreturn do_SBC_abs(CPU *cpu);	// 0xED
struct opreturn do_INC_abs(CPU *cpu);	// 0xEE
struct opreturn do_BEQ_rel(CPU *cpu);	// 0xF0
struct opreturn do_SBC_indY(CPU *cpu);	// 0xF1
struct opreturn do_SBC_zpgX(CPU *cpu);	// 0xF5
struct opreturn do_INC_zpgX(CPU *cpu);	// 0xF6
struct opreturn do_SED_impl(CPU *cpu);	// 0xF8
struct opreturn do_SBC_absY(CPU *cpu);	// 0xF9
struct opreturn do_SBC_absX(CPU *cpu);	// 0xFD
struct opreturn do_INC_absX(CPU *cpu);	// 0xFE

// Alternate handlers for BCD mode
struct opreturn do_ADC_imm_BCD(CPU *cpu);		// 0x69
struct opreturn do_ADC_abs_BCD(CPU *cpu);		// 0x6D
struct opreturn do_ADC_absX_BCD(CPU *cpu);	// 0x7D
struct opreturn do_ADC_absY_BCD(CPU *cpu);	// 0x79
struct opreturn do_ADC_zpg_BCD(CPU *cpu);		// 0x65
struct opreturn do_ADC_zpgX_BCD(CPU *cpu);	// 0x75
struct opreturn do_ADC_Xind_BCD(CPU *cpu);	// 0x61
struct opreturn do_ADC_indY_BCD(CPU *cpu);	// 0x71
struct opreturn do_SBC_imm_BCD(CPU *cpu);		// 0xE9
struct opreturn do_SBC_abs_BCD(CPU *cpu);		// 0xED
struct opreturn do_SBC_absX_BCD(CPU *cpu);	// 0xFD
struct opreturn do_SBC_absY_BCD(CPU *cpu);	// 0xF9
struct opreturn do_SBC_zpg_BCD(CPU *cpu);		// 0xE5
struct opreturn do_SBC_zpgX_BCD(CPU *cpu);	// 0xF5
struct opreturn do_SBC_Xind_BCD(CPU *cpu);	// 0xE1
struct opreturn do_SBC_indY_BCD(CPU *cpu);	// 0xF1

#endif
