// membus.h
//
// Definitions and function prototypes for 6502 emulator program
// 	Memory bus structure and functions
//
// Brian K. Niece

#ifndef MEMBUS_H
#define MEMBUS_H

// Definitions for emulator constants
#define MAX_MEM 64*1024

// Type definitions
typedef unsigned char byte;
typedef unsigned short word;

typedef struct memory_block
{
	word begin;
	word end;
	struct memory_block *next;
} memory_block;

typedef struct membus
{
	byte *mem;
	memory_block *ro_blocks;
	memory_block *wo_blocks;
} membus;

// Bus actions
byte read(membus bus, word addr);
void write(membus bus, word addr, byte data);

// Setup Functions
void initialize_bus(membus *bus);
void add_block(memory_block **blocks, word begin_addr, word end_addr);

// I/O functions
int import_mem(char *filename, membus *bus, word addr);
	// returns 0 on success
	// 		-1 on file epen error
	// 		-2 on file read error
int export_mem(char *filename, membus *bus, word addr, int npages);
	// returns 0 on success
	// 		-1 on file epen error
	// 		-2 on file write error
void print_mem_page(membus *mem, word addr, int mark);

#endif
