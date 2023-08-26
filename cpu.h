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
#define V 64	// 01000000
#define B 16	// 00010000
#define D 8		// 00001000
#define I 4		// 00000100
#define Z 2		// 00000010
#define C 1		// 00000001

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
	
	byte *mem;		// Pointer to the memory "bus"
} CPU;

// CPU control functions
void setup(CPU *cpu, byte *mem);
void reset(CPU *cpu);
byte fetch(CPU *cpu, word addr);
void set_N(CPU *cpu, byte reg);
void set_V(CPU *cpu, byte op1, byte op2, byte result);
void set_Z(CPU *cpu, byte reg);
void set_C(CPU *cpu, int result);

// Program Functions
void print_registers(CPU *cpu);

#endif
