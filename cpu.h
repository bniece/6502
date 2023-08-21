// cpu.h
//
// Definitions and function prototypes for 6502 emulator program
// 	cpu structure and functions
//
// Brian K. Niece

#ifndef CPU_H
#define CPU_H

// Status flags
#define N 128	// 10000000
#define v 64	// 01000000
#define B 16	// 00010000
#define D 8		// 00001000
#define I 4		// 00000100
#define Z 2		// 00000010
#define c 1		// 00000001

// Type definitions
typedef unsigned char byte;
typedef unsigned short word;

typedef struct CPU
{
	byte TC;			// Timing control
	word PC;			// Program counter
	byte IR;			// Instruction register
	byte SP;			// Stack pointer

	byte A;			// Accumulator
	byte X;			// Index register X
	byte Y;			// Index register Y
	
	byte PS;			// Processor status register
} CPU;

// CPU control functions
void reset(CPU *pcpu);

// Program Functions
void print_internals(CPU *cpu);

#endif
