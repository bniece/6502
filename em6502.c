// em6502.c
//
// 6502 emulator program
//
// Brian K. Niece

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "em6502.h"
#include "cpu.h"
#include "instructions.h"
#include "version.h"

int main(int argc, char *argv[])
{
   int c, opt_idx = 0;	// getopt variables
	int r;					// memory operation result

	// Track performance
	int cycleCount = 0;

	// Addresses of memory segments for quick reference
	word zero_page = 0x00;
	word stack = 0x100;
	word data = DEF_DATA_ADDR;
	word code = DEF_CODE_ADDR;

   // Parse and handle any options
   opterr = 0;

   struct option long_opts[] = 
   {
      {"version", no_argument, 0, 'v'},
      {"code-base", required_argument, 0, 'c'},
		{"data-base", required_argument, 0, 'd'},
      {0, 0, 0, 0}
   };

   while ((c = getopt_long(argc, argv, "vc:d:", long_opts, &opt_idx)) != -1)
      switch (c)
      {
	 case 'v':
	    printf("em6502 (6502 cpu emulator) %.1f\n", VERSION);
	    printf("Copyright (c) %d Brian K. Niece\n", COPYRIGHT);
	    printf("Built %s\n", __DATE__);
	    return 0;
	    break;
	 case 'c':
		 int code_in;
		 sscanf(optarg, "%x", &code_in);
		 code = code_in;
	    break;
	 case 'd':
		 int data_in;
		 sscanf(optarg, "%x", &data_in);
		 data = data_in;
	    break;
      }

	// Create processor and memory
	membus bus;
	CPU cpu;
	initialize_bus(&bus);
	initialize_cpu(&cpu, &bus);

	// Load code, bail out on error
	r = import_mem("code.bin", &bus, code);
	switch (r)
	{
		case 0:
			printf("Loading code.bin at 0x%04x\n", code);
			break;
		case -1:
			printf("Error opening code file: code.bin\n");
			return -1;
			break;
		case -2:
			printf("Error reading code file: code.bin\n");
			return -1;
			break;
		default:
			printf("Code file error\n");
			return -1;
			break;
	}

	// Load data if found
	r = import_mem("data.bin", &bus, data);
	switch (r)
	{
		case 0:
			printf("Loading data.bin at 0x%04x\n", data);
			break;
		case -1:
			printf("No data file found\n");
			break;
		case -2:
			printf("Error reading data file: data.bin\n");
			return -1;
			break;
		default:
			printf("Data file error\n");
			return -1;
			break;
	}

	// Set code location here so reset can find it.
	// A full 64k ROM would presumably provide this.
	write(bus, 0xFFFC, code & 0xFF);
	write(bus, 0xFFFD, code >> 8);

	// Write protect the reset/irq vectors
	// 	Note, the code isn't protected, so self-modifying code is possible
	add_block(&bus.ro_blocks, 0xFFFA, 0xFFFF);

	// Read protect output peripherals

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

		// Execute peripheral code here
		
		// Execute IRQs here
		
	} while (cpu.IR != 0x00);


	// Print new status, stack, memory, cycles used
	print_registers(&cpu);
	printf("\nZero Page:\n");
	print_mem_page(&bus, zero_page, -1);
	printf("\nStack:\n");
	print_mem_page(&bus, stack, cpu.SP);
	printf("\nData:\n");
	print_mem_page(&bus, data, -1);
	printf("\nCycles: %d\n", cycleCount);

   return 0;
}

