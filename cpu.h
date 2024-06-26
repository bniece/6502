// cpu.h
//
// Definitions and function prototypes for 6502 emulator program
// 	cpu structure and functions
//
// Brian K. Niece

#ifndef CPU_H
#define CPU_H

#include "membus.h"

// Status flags & test bits
#define N 128		//  10000000
#define V 64		//  01000000
#define B 16		//  00010000
#define D 8			//  00001000
#define I 4			//  00000100
#define Z 2			//  00000010
#define C 1			//  00000001
#define bit8 256	// 100000000

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
	
	byte SR;			// Processor status register
	
	membus *bus;	// Pointer to the memory "bus"
} CPU;

// CPU control functions
void initialize_cpu(CPU *cpu, membus *bus);
void reset(CPU *cpu);
void set_N(CPU *cpu, byte reg);
void set_V(CPU *cpu, byte op1, byte op2, byte result);
void set_Z(CPU *cpu, byte reg);
void set_C(CPU *cpu, int result);

#endif
