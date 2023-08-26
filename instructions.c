// 6502instructions.c
//
// 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece

#include <stdio.h>

#include "instructions.h"

void log_op_start(CPU *cpu, char *op, int bytes)
{
// Print operation address, mnemonic, and bytes consumed
	printf("0x%04X %s\t(%d bytes) ", cpu->PC, op, bytes);
}

void log_op_end(CPU *cpu, byte result, int cycles)
// Print operation result & status register, count cycles
{
	printf(" 0x%02X", result);

	printf(" %c%c%c%c%c%c%c", cpu->SR & N ? 'N' : 'n', 
			cpu->SR & V ? 'V' : 'v', cpu->SR & B ? 'B' : 'b', 
			cpu->SR & D ? 'D' : 'd', cpu->SR & I ? 'I' : 'i',
			cpu->SR & Z ? 'Z' : 'z', cpu->SR & C ? 'C' : 'c');

	for (cpu->TC = 0; cpu->TC < cycles; cpu->TC++)
	{
		printf(" %d", cpu->TC);
	}

	printf("\n");
}

void do_BRK_impl(CPU *cpu)
// This is not a correct implementation of this instruction.  It's
// just a convenient way to end programs
{
	int nbytes = 1;
	int ncycles = 7;

	log_op_start(cpu, "BRK   ", nbytes);
	log_op_end(cpu, 0, ncycles);
}

void do_ADC_imm(CPU *cpu)
// Load Accumulator with immediate addressing
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "ADC # ", nbytes);

	// Cycle 0: fetch instruction and increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the addition
	// 	Set N,V,Z,C if necessary
	// 	Store in A
	byte operand = fetch(cpu, cpu->PC);
	cpu->PC++;

	int result = cpu->A + operand;

	set_N(cpu, result);
	set_V(cpu, cpu->A, operand, result);
	set_Z(cpu, result);
	set_C(cpu, result);

	cpu->A = result;

	log_op_end(cpu, cpu->A, ncycles);

	return;
}

void do_STA_abs(CPU *cpu)
// Store Accumulator in memory with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "STA abs", nbytes);

	// Cycle 0: fetch instruction and increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store A at addr
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return;
}

void do_LDA_imm(CPU *cpu)
// Load Accumulator with immediate addressing
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "LDA # ", nbytes);

	// Cycle 0: fetch instruction and increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in A, increment PC
	// 	set N,Z if necessary
	cpu->A = fetch(cpu, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return;
}

void do_TAX_impl(CPU *cpu)
// Transfer Accumulator to X
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "TAX   ", nbytes);

	// Cycle 0: fetch instruction and increment PC
	cpu->PC++;

	// Cycle 1: copy byte from A to X
	// 	set N,Z if necessary
	cpu->X = cpu->A;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return;
}

void do_INX_impl(CPU *cpu)
// Increment X register
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "INX   ", nbytes);

	// Cycle 0: fetch instruction and increment PC
	cpu->PC++;

	// Cycle 1: Increment X
	// 	set N,Z if necessary
	cpu->X++;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return;
}

void do_NOP_impl(CPU *cpu)
// No operation
{
	int nbytes = 1;
	int ncycles = 2;
	
	log_op_start(cpu, "NOP   ", nbytes);

	// Cycle 0: fetch instruction and increment PC
	cpu->PC++;

	// Cycle 1: Do nothing
	
	log_op_end(cpu, 0, ncycles);

	return;
}

void (*execute[])(CPU *cpu) =
{
do_BRK_impl, 	// 0x00
do_NOP_impl, 	// 0x01
do_NOP_impl, 	// 0x02
do_NOP_impl, 	// 0x03
do_NOP_impl, 	// 0x04
do_NOP_impl, 	// 0x05
do_NOP_impl, 	// 0x06
do_NOP_impl, 	// 0x07
do_NOP_impl, 	// 0x08
do_NOP_impl, 	// 0x09
do_NOP_impl, 	// 0x0A
do_NOP_impl, 	// 0x0B
do_NOP_impl, 	// 0x0C
do_NOP_impl, 	// 0x0D
do_NOP_impl, 	// 0x0E
do_NOP_impl, 	// 0x0F
do_NOP_impl, 	// 0x10
do_NOP_impl, 	// 0x11
do_NOP_impl, 	// 0x12
do_NOP_impl, 	// 0x13
do_NOP_impl, 	// 0x14
do_NOP_impl, 	// 0x15
do_NOP_impl, 	// 0x16
do_NOP_impl, 	// 0x17
do_NOP_impl, 	// 0x18
do_NOP_impl, 	// 0x19
do_NOP_impl, 	// 0x1A
do_NOP_impl, 	// 0x1B
do_NOP_impl, 	// 0x1C
do_NOP_impl, 	// 0x1D
do_NOP_impl, 	// 0x1E
do_NOP_impl, 	// 0x1F
do_NOP_impl, 	// 0x20
do_NOP_impl, 	// 0x21
do_NOP_impl, 	// 0x22
do_NOP_impl, 	// 0x23
do_NOP_impl, 	// 0x24
do_NOP_impl, 	// 0x25
do_NOP_impl, 	// 0x26
do_NOP_impl, 	// 0x27
do_NOP_impl, 	// 0x28
do_NOP_impl, 	// 0x29
do_NOP_impl, 	// 0x2A
do_NOP_impl, 	// 0x2B
do_NOP_impl, 	// 0x2C
do_NOP_impl, 	// 0x2D
do_NOP_impl, 	// 0x2E
do_NOP_impl, 	// 0x2F
do_NOP_impl, 	// 0x30
do_NOP_impl, 	// 0x31
do_NOP_impl, 	// 0x32
do_NOP_impl, 	// 0x33
do_NOP_impl, 	// 0x34
do_NOP_impl, 	// 0x35
do_NOP_impl, 	// 0x36
do_NOP_impl, 	// 0x37
do_NOP_impl, 	// 0x38
do_NOP_impl, 	// 0x39
do_NOP_impl, 	// 0x3A
do_NOP_impl, 	// 0x3B
do_NOP_impl, 	// 0x3C
do_NOP_impl, 	// 0x3D
do_NOP_impl, 	// 0x3E
do_NOP_impl, 	// 0x3F
do_NOP_impl, 	// 0x40
do_NOP_impl, 	// 0x41
do_NOP_impl, 	// 0x42
do_NOP_impl, 	// 0x43
do_NOP_impl, 	// 0x44
do_NOP_impl, 	// 0x45
do_NOP_impl, 	// 0x46
do_NOP_impl, 	// 0x47
do_NOP_impl, 	// 0x48
do_NOP_impl, 	// 0x49
do_NOP_impl, 	// 0x4A
do_NOP_impl, 	// 0x4B
do_NOP_impl, 	// 0x4C
do_NOP_impl, 	// 0x4D
do_NOP_impl, 	// 0x4E
do_NOP_impl, 	// 0x4F
do_NOP_impl, 	// 0x50
do_NOP_impl, 	// 0x51
do_NOP_impl, 	// 0x52
do_NOP_impl, 	// 0x53
do_NOP_impl, 	// 0x54
do_NOP_impl, 	// 0x55
do_NOP_impl, 	// 0x56
do_NOP_impl, 	// 0x57
do_NOP_impl, 	// 0x58
do_NOP_impl, 	// 0x59
do_NOP_impl, 	// 0x5A
do_NOP_impl, 	// 0x5B
do_NOP_impl, 	// 0x5C
do_NOP_impl, 	// 0x5D
do_NOP_impl, 	// 0x5E
do_NOP_impl, 	// 0x5F
do_NOP_impl, 	// 0x60
do_NOP_impl, 	// 0x61
do_NOP_impl, 	// 0x62
do_NOP_impl, 	// 0x63
do_NOP_impl, 	// 0x64
do_NOP_impl, 	// 0x65
do_NOP_impl, 	// 0x66
do_NOP_impl, 	// 0x67
do_NOP_impl, 	// 0x68
do_ADC_imm, 	// 0x69
do_NOP_impl, 	// 0x6A
do_NOP_impl, 	// 0x6B
do_NOP_impl, 	// 0x6C
do_NOP_impl, 	// 0x6D
do_NOP_impl, 	// 0x6E
do_NOP_impl, 	// 0x6F
do_NOP_impl, 	// 0x70
do_NOP_impl, 	// 0x71
do_NOP_impl, 	// 0x72
do_NOP_impl, 	// 0x73
do_NOP_impl, 	// 0x74
do_NOP_impl, 	// 0x75
do_NOP_impl, 	// 0x76
do_NOP_impl, 	// 0x77
do_NOP_impl, 	// 0x78
do_NOP_impl, 	// 0x79
do_NOP_impl, 	// 0x7A
do_NOP_impl, 	// 0x7B
do_NOP_impl, 	// 0x7C
do_NOP_impl, 	// 0x7D
do_NOP_impl, 	// 0x7E
do_NOP_impl, 	// 0x7F
do_NOP_impl, 	// 0x80
do_NOP_impl, 	// 0x81
do_NOP_impl, 	// 0x82
do_NOP_impl, 	// 0x83
do_NOP_impl, 	// 0x84
do_NOP_impl, 	// 0x85
do_NOP_impl, 	// 0x86
do_NOP_impl, 	// 0x87
do_NOP_impl, 	// 0x88
do_NOP_impl, 	// 0x89
do_NOP_impl, 	// 0x8A
do_NOP_impl, 	// 0x8B
do_NOP_impl, 	// 0x8C
do_STA_abs, 	// 0x8D
do_NOP_impl, 	// 0x8E
do_NOP_impl, 	// 0x8F
do_NOP_impl, 	// 0x90
do_NOP_impl, 	// 0x91
do_NOP_impl, 	// 0x92
do_NOP_impl, 	// 0x93
do_NOP_impl, 	// 0x94
do_NOP_impl, 	// 0x95
do_NOP_impl, 	// 0x96
do_NOP_impl, 	// 0x97
do_NOP_impl, 	// 0x98
do_NOP_impl, 	// 0x99
do_NOP_impl, 	// 0x9A
do_NOP_impl, 	// 0x9B
do_NOP_impl, 	// 0x9C
do_NOP_impl, 	// 0x9D
do_NOP_impl, 	// 0x9E
do_NOP_impl, 	// 0x9F
do_NOP_impl, 	// 0xA0
do_NOP_impl, 	// 0xA1
do_NOP_impl, 	// 0xA2
do_NOP_impl, 	// 0xA3
do_NOP_impl, 	// 0xA4
do_NOP_impl, 	// 0xA5
do_NOP_impl, 	// 0xA6
do_NOP_impl, 	// 0xA7
do_NOP_impl, 	// 0xA8
do_LDA_imm, 	// 0xA9
do_TAX_impl, 	// 0xAA
do_NOP_impl, 	// 0xAB
do_NOP_impl, 	// 0xAC
do_NOP_impl, 	// 0xAD
do_NOP_impl, 	// 0xAE
do_NOP_impl, 	// 0xAF
do_NOP_impl, 	// 0xB0
do_NOP_impl, 	// 0xB1
do_NOP_impl, 	// 0xB2
do_NOP_impl, 	// 0xB3
do_NOP_impl, 	// 0xB4
do_NOP_impl, 	// 0xB5
do_NOP_impl, 	// 0xB6
do_NOP_impl, 	// 0xB7
do_NOP_impl, 	// 0xB8
do_NOP_impl, 	// 0xB9
do_NOP_impl, 	// 0xBA
do_NOP_impl, 	// 0xBB
do_NOP_impl, 	// 0xBC
do_NOP_impl, 	// 0xBD
do_NOP_impl, 	// 0xBE
do_NOP_impl, 	// 0xBF
do_NOP_impl, 	// 0xC0
do_NOP_impl, 	// 0xC1
do_NOP_impl, 	// 0xC2
do_NOP_impl, 	// 0xC3
do_NOP_impl, 	// 0xC4
do_NOP_impl, 	// 0xC5
do_NOP_impl, 	// 0xC6
do_NOP_impl, 	// 0xC7
do_NOP_impl, 	// 0xC8
do_NOP_impl, 	// 0xC9
do_NOP_impl, 	// 0xCA
do_NOP_impl, 	// 0xCB
do_NOP_impl, 	// 0xCC
do_NOP_impl, 	// 0xCD
do_NOP_impl, 	// 0xCE
do_NOP_impl, 	// 0xCF
do_NOP_impl, 	// 0xD0
do_NOP_impl, 	// 0xD1
do_NOP_impl, 	// 0xD2
do_NOP_impl, 	// 0xD3
do_NOP_impl, 	// 0xD4
do_NOP_impl, 	// 0xD5
do_NOP_impl, 	// 0xD6
do_NOP_impl, 	// 0xD7
do_NOP_impl, 	// 0xD8
do_NOP_impl, 	// 0xD9
do_NOP_impl, 	// 0xDA
do_NOP_impl, 	// 0xDB
do_NOP_impl, 	// 0xDC
do_NOP_impl, 	// 0xDD
do_NOP_impl, 	// 0xDE
do_NOP_impl, 	// 0xDF
do_NOP_impl, 	// 0xE0
do_NOP_impl, 	// 0xE1
do_NOP_impl, 	// 0xE2
do_NOP_impl, 	// 0xE3
do_NOP_impl, 	// 0xE4
do_NOP_impl, 	// 0xE5
do_NOP_impl, 	// 0xE6
do_NOP_impl, 	// 0xE7
do_INX_impl, 	// 0xE8
do_NOP_impl, 	// 0xE9
do_NOP_impl, 	// 0xEA
do_NOP_impl, 	// 0xEB
do_NOP_impl, 	// 0xEC
do_NOP_impl, 	// 0xED
do_NOP_impl, 	// 0xEE
do_NOP_impl, 	// 0xEF
do_NOP_impl, 	// 0xF0
do_NOP_impl, 	// 0xF1
do_NOP_impl, 	// 0xF2
do_NOP_impl, 	// 0xF3
do_NOP_impl, 	// 0xF4
do_NOP_impl, 	// 0xF5
do_NOP_impl, 	// 0xF6
do_NOP_impl, 	// 0xF7
do_NOP_impl, 	// 0xF8
do_NOP_impl, 	// 0xF9
do_NOP_impl, 	// 0xFA
do_NOP_impl, 	// 0xFB
do_NOP_impl, 	// 0xFC
do_NOP_impl, 	// 0xFD
do_NOP_impl, 	// 0xFE
do_NOP_impl  	// 0xFF
};
