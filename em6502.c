// em6502.c
//
// 6502 emulator program
//
// Brian K. Niece

#include <getopt.h>
#include <stdio.h>

#include "em6502.h"
#include "cpu.h"
#include "instructions.h"
#include "version.h"

int main(int argc, char *argv[])
{
   int c, opt_idx = 0;	// getopt variables
	int r;					// memory operation result

	// Track performance
	int cycle_count = 0;

	// Addresses of memory segments for quick reference
	word zero_page = 0x00;
	word stack = 0x100;
	word data = DEF_DATA_ADDR;
	word code = DEF_CODE_ADDR;

	// Default file names
	char *code_file = "code.bin";
	char *data_file = "data.bin";

   // Parse and handle any options
   opterr = 0;

   struct option long_opts[] = 
   {
      {"version", no_argument, 0, 'v'},
      {"code-base", required_argument, 0, 'c'},
		{"data-base", required_argument, 0, 'd'},
		{"program-file", required_argument, 0, 'p'},
		{"input-file", required_argument, 0, 'i'},
      {0, 0, 0, 0}
   };

   while ((c = getopt_long(argc, argv, "vc:d:p:i:", long_opts, &opt_idx)) != -1)
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
	 case 'p':
		 code_file = optarg;
		 break;
	 case 'i':
		 data_file = optarg;
		 break;
      }

	// Create processor and memory
	membus bus;
	CPU cpu;
	initialize_bus(&bus);
	initialize_cpu(&cpu, &bus);

	// Load code, bail out on error
	r = import_mem(code_file, &bus, code);
	switch (r)
	{
		case 0:
			printf("Loading %s at 0x%04x\n", code_file, code);
			break;
		case -1:
			printf("Error opening code file: %s\n", code_file);
			return -1;
			break;
		case -2:
			printf("Error reading code file: %s\n", code_file);
			return -1;
			break;
		default:
			printf("Code file error\n");
			return -1;
			break;
	}

	// Load data if found
	r = import_mem(data_file, &bus, data);
	switch (r)
	{
		case 0:
			printf("Loading %s at 0x%04x\n", data_file, data);
			break;
		case -1:
			printf("No data file found\n");
			break;
		case -2:
			printf("Error reading data file: %s\n", data_file);
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
		cycle_count += execute[cpu.IR](&cpu);

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
	printf("\nCycles: %d\n", cycle_count);

   return 0;
}

