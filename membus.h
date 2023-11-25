// membus.h
//
// Definitions and function prototypes for 6502 emulator program
// 	Memory bus structure and functions
//
// Brian K. Niece

#ifndef MEMBUS_H
#define MEMBUS_H

// Definitions for emulator constants
#define MAX_MEM 1<<16

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

// I/O functions
byte read(membus bus, word addr);
void write(membus bus, word addr, byte data);
void print_blocks(memory_block *blocks);

// Setup Functions
void initialize(membus *bus);
void add_block(memory_block **blocks, word begin_addr, word end_addr);

#endif
