// membus.c
//
// 6502 emulator program
// 	Memory bus structure and functions
//
// Brian K. Niece

#include <stdlib.h>
#include <stdio.h>

#include "membus.h"

byte read(membus bus, word addr)
{
	return bus.mem[addr];
}

void write(membus bus, word addr, byte data)
{
	bus.mem[addr] = data;
}

void print_blocks(memory_block *blocks)
{
	memory_block *list;
	list = blocks;

	while (list != NULL)
	{
		printf("Begin: %x\tEnd: %x\tNext: %p\n", list->begin,
				list->end, list->next);
		list = list->next;
	}

}

void initialize_bus(membus *bus)
{
	bus->mem = malloc(MAX_MEM);
	bus->ro_blocks = NULL;
	bus->wo_blocks = NULL;
}

void add_block(memory_block **blocks, word begin_addr, word end_addr)
// Add ro or wo memory block to list
{
	memory_block *new_block = malloc(sizeof(memory_block));
	new_block->begin = begin_addr;
	new_block->end = end_addr;
	new_block->next = NULL;

	// Insert new block at beginning of list
	// 	Order doesn't matter, and this is easy.
	new_block->next = *blocks;
	*blocks = new_block;
}

int import_mem(char *filename, membus *bus, word addr)
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
	readbytes = fread(&bus->mem[addr], sizeof(byte), filebytes, file);
	if (readbytes != filebytes)
	{
		printf("Error reading file %s\n", filename);
		return -2;
	}

	return 0;
}

void print_mem_page(membus *bus, word addr, int mark)
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
			printf("%02X", bus->mem[addr + offset]);
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
