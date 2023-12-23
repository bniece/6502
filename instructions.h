// insructions.h
//
// Definitions and function prototypes for 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece

#ifndef INST6502_H
#define INST6502_H

#include "cpu.h"

// Logging functions
void log_op_start(CPU *cpu, char *op, int bytes);
void lob_op_end(CPU *cpu, byte result, int cycles);

// Instruction handler functions
int do_BRK_impl(CPU *cpu);	// 0x00
int do_ORA_Xind(CPU *cpu);	// 0x01
int do_ORA_zpg(CPU *cpu);	// 0x05
int do_PHP_impl(CPU *cpu);	// 0x08
int do_ORA_imm(CPU *cpu);	// 0x09
int do_ORA_abs(CPU *cpu);	// 0x0D
int do_ORA_absY(CPU *cpu);	// 0x19
int do_BPL_rel(CPU *cpu);	// 0x10
int do_ORA_indY(CPU *cpu);	// 0x11
int do_ORA_zpgR(CPU *cpu);	// 0x15
int do_ORA_absX(CPU *cpu);	// 0x1D
int do_CLC_impl(CPU *cpu);	// 0x18
int do_JSR_abs(CPU *cpu);	// 0x20
int do_AND_Xind(CPU *cpu);	// 0x21
int do_BIT_zpg(CPU *cpu);	// 0x24
int do_AND_zpg(CPU *cpu);	// 0x25
int do_PLP_impl(CPU *cpu);	// 0x28
int do_AND_imm(CPU *cpu);	// 0x29
int do_BIT_abs(CPU *cpu);	// 0x2C
int do_AND_abs(CPU *cpu);	// 0x2D
int do_BMI_rel(CPU *cpu);	// 0x30
int do_AND_indY(CPU *cpu);	// 0x31
int do_AND_zpgX(CPU *cpu);	// 0x35
int do_SEC_impl(CPU *cpu);	// 0x38
int do_AND_absY(CPU *cpu);	// 0x39
int do_AND_absX(CPU *cpu);	// 0x3D
int do_RTI_impl(CPU *cpu);	// 0x40
int do_EOR_Xind(CPU *cpu);	// 0x41
int do_EOR_zpg(CPU *cpu);	// 0x45
int do_PHA_impl(CPU *cpu);	// 0x48
int do_EOR_imm(CPU *cpu);	// 0x49
int do_JMP_abs(CPU *cpu);	// 0x4C
int do_EOR_abs(CPU *cpu);	// 0x4D
int do_BVC_rel(CPU *cpu);	// 0x50
int do_EOR_indY(CPU *cpu);	// 0x51
int do_EOR_zpgX(CPU *cpu);	// 0x55
int do_PHA_impl(CPU *cpu);	// 0x48
int do_CLI_impl(CPU *cpu);	// 0x58
int do_EOR_absY(CPU *cpu);	// 0x59
int do_EOR_absX(CPU *cpu);	// 0x5D
int do_RTS_impl(CPU *cpu);	// 0x60
int do_ADC_Xind(CPU *cpu);	// 0x61
int do_ADC_zpg(CPU *cpu);	// 0x65
int do_PLA_impl(CPU *cpu);	// 0x68
int do_ADC_imm(CPU *cpu);	// 0x69
int do_JMP_ind(CPU *cpu);	// 0x6C
int do_ADC_abs(CPU *cpu);	// 0x6D
int do_BVS_rel(CPU *cpu);	// 0x70
int do_ADC_indY(CPU *cpu);	// 0x71
int do_ADC_zpgX(CPU *cpu);	// 0x75
int do_SEI_impl(CPU *cpu);	// 0x78
int do_ADC_absY(CPU *cpu);	// 0x79
int do_ADC_absX(CPU *cpu);	// 0x7D
int do_STA_Xind(CPU *cpu);	// 0x81
int do_STY_zpg(CPU *cpu);	// 0x84
int do_STA_zpg(CPU *cpu);	// 0x85
int do_STX_zpg(CPU *cpu);	// 0x86
int do_DEY_impl(CPU *cpu);	// 0x88
int do_TXA_impl(CPU *cpu);	// 0x8A
int do_STY_abs(CPU *cpu);	// 0x8c
int do_STA_abs(CPU *cpu);	// 0x8D
int do_STX_abs(CPU *cpu);	// 0x8E
int do_BCC_rel(CPU *cpu);	// 0x90
int do_STA_indY(CPU *cpu);	// 0x91
int do_STY_zpgX(CPU *cpu);	// 0x94
int do_STA_zpgX(CPU *cpu);	// 0x95
int do_STX_zpgY(CPU *cpu);	// 0x96
int do_TYA_impl(CPU *cpu);	// 0x98
int do_STA_absY(CPU *cpu);	// 0x99
int do_TXS_impl(CPU *cpu);	// 0x9A
int do_STA_absX(CPU *cpu);	// 0x9D
int do_LDY_imm(CPU *cpu);	// 0xA0
int do_LDA_Xind(CPU *cpu);	// 0xA1
int do_LDX_imm(CPU *cpu);	// 0xA2
int do_LDY_zpg(CPU *cpu);	// 0xA4
int do_LDA_zpg(CPU *cpu);	// 0xA5
int do_LDX_zpg(CPU *cpu);	// 0xA6
int do_TAY_impl(CPU *cpu);	// 0xA8
int do_LDA_imm(CPU *cpu);	// 0xA9
int do_TAX_impl(CPU *cpu);	// 0xAA
int do_LDY_abs(CPU *cpu);	// 0xAC
int do_LDA_abs(CPU *cpu);	// 0xAD
int do_LDX_abs(CPU *cpu);	// 0xAE
int do_LDA_indY(CPU *cpu);	// 0xB1
int do_BCS_rel(CPU *cpu);	// 0xB0
int do_LDY_zpgX(CPU *cpu);	// 0xB4
int do_LDA_zpgX(CPU *cpu);	// 0xB5
int do_LDX_zpgY(CPU *cpu);	// 0xB6
int do_CLV_impl(CPU *cpu);	// 0xB8
int do_LDA_absY(CPU *cpu);	// 0xB9
int do_LDY_absX(CPU *cpu);	// 0xBC
int do_LDA_absX(CPU *cpu);	// 0xBD
int do_LDX_absY(CPU *cpu);	// 0xBE
int do_CPY_imm(CPU *cpu);	// 0xC0
int do_CMP_Xind(CPU *cpu);	// 0xC1
int do_CPY_zpg(CPU *cpu);	// 0xC4
int do_CMP_zpg(CPU *cpu);	// 0xC5
int do_DEC_zpg(CPU *cpu);	// 0xC6
int do_INY_impl(CPU *cpu);	// 0xC8
int do_CMP_imm(CPU *cpu);	// 0xC9
int do_DEX_impl(CPU *cpu);	// 0xCA
int do_CPY_abs(CPU *cpu);	// 0xCC
int do_CMP_abs(CPU *cpu);	// 0xCD
int do_DEC_abs(CPU *cpu);	// 0xCE
int do_BNE_rel(CPU *cpu);	// 0xD0
int do_CMP_indY(CPU *cpu);	// 0xD1
int do_CMP_zpgX(CPU *cpu);	// 0xD5
int do_DEC_zpgX(CPU *cpu);	// 0xD6
int do_CLD_impl(CPU *cpu);	// 0xD8
int do_CMP_absY(CPU *cpu);	// 0xD9
int do_CMP_absX(CPU *cpu);	// 0xDD
int do_DEC_absX(CPU *cpu);	// 0xDE
int do_CPX_imm(CPU *cpu);	// 0xE0
int do_SBC_Xind(CPU *cpu);	// 0xE1
int do_CPX_zpg(CPU *cpu);	// 0xE4
int do_SBC_zpg(CPU *cpu);	// 0xE5
int do_INC_zpg(CPU *cpu);	// 0xE6
int do_INX_impl(CPU *cpu);	// 0xE8
int do_SBC_imm(CPU *cpu);	// 0xE9
int do_NOP_impl(CPU *cpu);	// 0xEA
int do_CPX_abs(CPU *cpu);	// 0xEC
int do_SBC_abs(CPU *cpu);	// 0xED
int do_INC_abs(CPU *cpu);	// 0xEE
int do_BEQ_rel(CPU *cpu);	// 0xF0
int do_SBC_indY(CPU *cpu);	// 0xF1
int do_SBC_zpgX(CPU *cpu);	// 0xF5
int do_INC_zpgX(CPU *cpu);	// 0xF6
int do_SED_impl(CPU *cpu);	// 0xF8
int do_SBC_absY(CPU *cpu);	// 0xF9
int do_SBC_absX(CPU *cpu);	// 0xFD
int do_INC_absX(CPU *cpu);	// 0xFE

// Alternate handlers for BCD mode
int do_ADC_imm_BCD(CPU *cpu);		// 0x69
int do_ADC_abs_BCD(CPU *cpu);		// 0x6D
int do_ADC_absX_BCD(CPU *cpu);	// 0x7D
int do_ADC_absY_BCD(CPU *cpu);	// 0x79
int do_ADC_zpg_BCD(CPU *cpu);		// 0x65
int do_ADC_zpgX_BCD(CPU *cpu);	// 0x75
int do_ADC_Xind_BCD(CPU *cpu);	// 0x61
int do_ADC_indY_BCD(CPU *cpu);	// 0x71
int do_SBC_imm_BCD(CPU *cpu);		// 0xE9
int do_SBC_abs_BCD(CPU *cpu);		// 0xED
int do_SBC_absX_BCD(CPU *cpu);	// 0xFD
int do_SBC_absY_BCD(CPU *cpu);	// 0xF9
int do_SBC_zpg_BCD(CPU *cpu);		// 0xE5
int do_SBC_zpgX_BCD(CPU *cpu);	// 0xF5
int do_SBC_Xind_BCD(CPU *cpu);	// 0xE1
int do_SBC_indY_BCD(CPU *cpu);	// 0xF1

#endif
