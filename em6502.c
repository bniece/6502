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
	// Create processor and memory
	CPU cpu;
	byte mem[MAX_MEM];
	setup(&cpu, mem);

	// Addresses of memory segments for quick reference
	word stack = 0x100;
	word data = DEF_DATA_ADDR;
	word code = DEF_CODE_ADDR;

	// Set default code location here so reset can find it.
	// A full 64k ROM would presumably provide this.
	mem[0xFFFC] = DEF_CODE_ADDR & 0xFF;
	mem[0xFFFD] = DEF_CODE_ADDR >> 8;

	// Load data and code, bail out on error
	if (import_mem("code.bin", mem, code) != 0)
	{
		return -1;
	}

	// "Boot"
	reset(&cpu);

	// Print current status & code
	print_registers(&cpu);
	printf("\nCode:\n");
	print_mem_page(mem, code, -1);

	// Read from the code segment until out of instructions
	do 
	{
		cpu.IR = fetch(&cpu, cpu.PC);
		execute[cpu.IR](&cpu);
	} while (cpu.IR != 0x00);

	// Print new status, stack, memory
	print_registers(&cpu);
	printf("\nStack:\n");
	print_mem_page(mem, stack, cpu.SP);
	printf("\nData:\n");
	print_mem_page(mem, data, -1);

   return 0;
}

int import_mem(char *filename, byte *mem, word addr)
// Read contents of binary file into memory at specified location
// 	This is an emulator function, so it doesn't need to go through the CPU
{
	int filebytes, readbytes;

	// Open file
	FILE *file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Error opening file %s\n", filename);
		return -1;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	filebytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read data into addr
	readbytes = fread(&mem[addr], sizeof(byte), filebytes, file);
	if (readbytes != filebytes)
	{
		printf("Error reading file %s\n", filename);
		return -2;
	}

	return 0;
}

void print_mem_page(byte *mem, word addr, int mark)
// Print memory page as hex bytes in 16 x 16 grid, marking selected byte
// 	Prefixed with address of first byte on line
// 	Any value < 0 or > 255 will cause no mark
{
	for (int row = 0; row < 16; row++)
	{
		printf("0x%04X: ", addr + row * 16);
		for (int col = 0; col < 16; col++)
		{
			int offset = row * 16 + col;
			printf("%02X", mem[addr + offset]);
			if (offset == mark)
			{
				printf("* ");
			}
			else
			{
				printf("  ");
			}
		}
		printf("\n");
	}
}

