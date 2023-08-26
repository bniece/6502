// cpu.c
//
// 6502 emulator program
// 	CPU structure and functions
//
// Brian K. Niece

#include <stdio.h>
#include <string.h>

#include "em6502.h"
#include "cpu.h"

void setup(CPU *cpu, byte *mem)
// Set up the "physical" connections of the processor, 
// 	i.e. link it to its memory.
// (In the future, can this be made more general -- 
// 	to connect peripherals, interrupt lines?)
{
	cpu->mem = mem;
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
	cpu->PC = fetch(cpu, 0xFFFC) + (fetch(cpu, 0xFFFD) << 8);
}

byte fetch(CPU *cpu, word addr)
// Retrieve a byte from the requested address
{
	return cpu->mem[addr];
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
// Set the overflow flag baed on any change in bit 7
{
	switch (cpu->IR)
	{
		// Addition operations
		//		Set overflow if both operands have the same sign
		//		and the result has the opposite
		case 0x69:
			if ((((op1 & N) == N) && ((op2 & N) == N)
							&& ((result & N) ==0))
						|| (((op1 & N) == 0) && ((op2 & N) == 0)
								&& ((result & N) == N)))
			{
				cpu->SR |= V;
			}
			else
			{
				cpu->SR &= ~V;
			}

			break;
		// Subtraction operations
		case 0xE9:

			break;
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
{
	// The C int has the extra bit to make this check easy
	if (result > 255)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}
}

void print_registers(CPU *cpu)
// List contents of CPU on stdout
{
	printf("6502 CPU Status:\n");
	printf("PC: 0x%04X\n", cpu->PC);
	printf("SP: 0x%02X\n", cpu->SP);
	printf("N V B D I Z C\n");
	printf("%d %d %d %d %d %d %d\n", 
			(cpu->SR & N)/N, (cpu->SR & V)/V, (cpu->SR & B)/B, 
			(cpu->SR & D)/D, (cpu->SR & I)/I, (cpu->SR & Z)/Z, 
			(cpu->SR & C)/C);
	printf("IR: 0x%02X\tTC: 0x%02X\n", cpu->IR, cpu->TC);
	printf("A:  0x%02X\tX:  0x%02X\tY:  0x%02X\n", cpu->A, 
			cpu->X, cpu->Y);
}

