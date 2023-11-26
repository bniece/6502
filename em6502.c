// em6502.c
//
// 6502 emulator program
//
// Brian K. Niece

#include <stdio.h>
#include <string.h>

#include "em6502.h"
#include "cpu.h"
#include "instructions.h"

int main(int argc, char *argv[])
{
	// Track performance
	int cycleCount = 0;

	// Create processor and memory
	membus bus;
	CPU cpu;
	initialize_bus(&bus);
	initialize_cpu(&cpu, &bus);

	// Addresses of memory segments for quick reference
	word zero_page = 0x00;
	word stack = 0x100;
	word data = DEF_DATA_ADDR;
	word code = DEF_CODE_ADDR;

	// Set default code location here so reset can find it.
	// A full 64k ROM would presumably provide this.
	write(bus, 0xFFFC, DEF_CODE_ADDR & 0xFF);
	write(bus, 0xFFFD, DEF_CODE_ADDR >> 8);

	// Load data and code, bail out on error
	if (import_mem("code.bin", &bus, code) != 0)
	{
		return -1;
	}

	// "Boot"
	reset(&cpu);

	// Print current status & code
	print_registers(&cpu);
	printf("\nCode:\n");
	print_mem_page(&bus, code, -1);

	// Read from the code segment until out of instructions
	do 
	{
		cpu.IR = read(bus, cpu.PC);
		cycleCount += execute[cpu.IR](&cpu);
	} while (cpu.IR != 0x00);

	// Print new status, stack, memory, cycles used
	print_registers(&cpu);
	printf("\nZero Page:\n");
	print_mem_page(&bus, zero_page, -1);
	printf("\nStack:\n");
	print_mem_page(&bus, stack, cpu.SP);
	printf("\nData:\n");
	print_mem_page(&bus, code, -1);
	printf("\nCycles: %d\n", cycleCount);

   return 0;
}

