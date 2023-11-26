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

