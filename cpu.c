// cpu.c
//
// 6502 emulator program
// 	CPU structure and functions
//
// Brian K. Niece

#include "cpu.h"
#include "membus.h"

void initialize_cpu(CPU *cpu, membus *bus)
{
	// Link the processor to the memory bus so it can access data
	// 	and peripherals
	cpu->bus = bus;

	// Set bit 5 of the status register because it is always 1
	cpu->SR = cpu->SR | 32;
}

void reset(CPU *cpu)
// Performs hardware reset as described in the Synertek 
// 	SY6500 data sheet
{
	// 6 clock cycles

	// Set interrupt mask flag
	cpu->SR = cpu->SR | I;
	
	// Load PC from 0xFFFC & 0xFFFD
	//
	// Note, this does not initialize the stack pointer.
	// Word on the internet is that if you care where it is, 
	// 	you should set it yourself, i.e., start your code 
	// 	with LDX #$FF, TXS
	// 	and possibly also CLD
	cpu->PC = read(*cpu->bus, 0xFFFC) + (read(*cpu->bus, 0xFFFD) << 8);
}

void set_N(CPU *cpu, byte reg)
// Check bit 7 of reg and set or clear negative flag
{
	// N is bit 7 of the status register.  
	// That can't be an accident, so let's use it.
	if((reg & N) == 0)
	{
		cpu->SR &= ~N;
	}
	else
	{
		cpu->SR |= N;
	}
}

void set_V(CPU *cpu, byte op1, byte op2, byte result)
// Set or clear the overflow flag baed on any change in bit 7
// 	This is the hardware logic invoked after arithmetic, not to be
// 	confused with the opcode CLV
{
	//	Set overflow if both operands have the same sign
	//		and the result has the opposite
	//	Works for subtraction also, as long as the operand is passed in
	//		as the negated value that was added.
	//	This is not the method used by the hardware, which would require 
	//		access to the carry from bit 6 to 7, which we don't have.
	if (((op1^result) & (op2^result) & 0x80) == 0)
			{
				cpu->SR &= ~V;
			} 
			else
			{
				cpu->SR |= V;
			}
}

void set_Z(CPU *cpu, byte reg)
// Set or clear zero flag
{
	if(reg == 0)
	{
		cpu->SR |= Z;
	}
	else
	{
		cpu->SR &= ~Z;
	}
}

void set_C(CPU *cpu, int result)
// Set or clear carry flag 
// 	This is the hardware logic invoked after arithmetic, not to be
// 	confused with the opcodes SEC and CLC
{
	// The C int has the extra bit to make this check easy
	if ((result & bit8) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
}

