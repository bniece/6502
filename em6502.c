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
	CPU cpu;
	byte mem[MAX_MEM];

	byte *stack = mem + 0x100;

	print_internals(&cpu);

	printf("\nProgram:\n");

	import_mem("simple.bin", mem + 0x200);

	print_mem_page(mem + 0x200, 14);

	execute[0](&cpu, mem);
	execute[0xA9](&cpu, mem);

   return 0;
}

void import_mem(char *filename, byte *addr)
{
	int filebytes, readbytes;

	FILE *file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("Error opening file %s\n", filename);
		return;
	}

	fseek(file, 0, SEEK_END);
	filebytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	readbytes = fread(addr, sizeof(byte), filebytes, file);
}

void print_mem_page(byte *page, int mark)
{
	for (int row = 0; row < 16; row++)
	{
		for (int col = 0; col < 16; col++)
		{
			int offset = row * 16 + col;
			printf("%02X", page[offset]);
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

