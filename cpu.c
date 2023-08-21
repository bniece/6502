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

void reset(CPU *pcpu)
{

}

void print_internals(CPU *cpu)
{
	printf("6502 CPU Status:\n");
	printf("PC: 0x%04X\n", cpu->PC);
	printf("SP: 0x%02X\n", cpu->SP);
	printf("IR: 0x%02X\tTC: 0x%02X\n", cpu->IR, cpu->TC);
	printf("A:  0x%02X\tX:  0x%02X\tY:  0x%02X\n", cpu->A, cpu->X, cpu->Y);
}

