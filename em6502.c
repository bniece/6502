// em6502.c
//
// 6502 emulator program
//
// Brian K. Niece

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "em6502.h"
#include "cpu.h"
#include "instructions.h"
#include "version.h"

int main(int argc, char *argv[])
{
   int c, opt_idx = 0;	// getopt variables
	int r;					// memory operation result
	int print_log = 1;	// enable/disable code log
	struct opreturn opr = test_op(); // operation result

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
	char *out_file = "out.bin";

	// Output parameters
	int code_pages = 1;
	int data_pages = 1;
	int print_stack = 1;
	int print_zpg = 1;

   // Parse and handle any options
   opterr = 0;

   struct option long_opts[] = 
   {
      {"version", no_argument, 0, 'v'},
      {"code-base", required_argument, 0, 'c'},
		{"data-base", required_argument, 0, 'd'},
		{"program-file", required_argument, 0, 'p'},
		{"input-file", required_argument, 0, 'i'},
		{"output-file", required_argument, 0, '0'},
		{"code-pages", required_argument, 0, 'C'},
		{"data-pages", required_argument, 0, 'D'},
		{"print-stack", required_argument, 0, 'S'},
		{"print-zero-page", required_argument, 0, 'Z'},
		{"print-code-log", required_argument, 0, 'L'},
      {0, 0, 0, 0}
   };

   while ((c = getopt_long(argc, argv, "vc:d:p:i:o:C:D:S:Z:L:", long_opts, &opt_idx)) != -1)
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
	 case 'o':
		 out_file = optarg;
		 break;
	 case 'C':
		 code_pages = atoi(optarg);
		 break;
	 case 'D':
		 data_pages = atoi(optarg);
		 break;
	 case 'S':
		 print_stack = atoi(optarg);
		 break;
	 case 'Z':
		 print_zpg = atoi(optarg);
		 break;
	 case 'L':
		 set_print_trace(atoi(optarg));
		 print_log = atoi(optarg);
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

	// Print current status & requested code pages
	print_registers(&cpu);

	if (code_pages > 0)
	{
		printf("\nCode:\n");
		for (int i = 0; i < code_pages; i++)
		{
			print_mem_page(&bus, code + i*0x100, -1);
		}
	}

	// Read & execute from the code segment until out of instructions
	printf("\nExecuting . . . \n");
	do 
	{
		cpu.IR = read(bus, cpu.PC);
		cycle_count += execute[cpu.IR](&cpu);

		// Log operation to stdout if enabled
		if (print_log == 1)
		{
			log_op(&cpu, opr);
		}
		
		// Execute peripheral code here
		
		// Execute IRQs here
		
	} while (cpu.IR != 0x00);


	// Print new status
	print_registers(&cpu);

	// Print zero page and stack if requested
	if (print_zpg == 1)
	{
		printf("\nZero Page:\n");
		print_mem_page(&bus, zero_page, -1);
	}

	if (print_stack == 1)
	{
		printf("\nStack:\n");
		print_mem_page(&bus, stack, cpu.SP);
	}

	// Print and save requested data pages
	if (data_pages > 0)
	{
		printf("\nData:\n");
		for (int i = 0; i < data_pages; i++)
		{
			print_mem_page(&bus, data + i*0x100, -1);
		}
		r = export_mem(out_file, &bus, data, data_pages);
		switch (r)
		{
			case 0:
				printf("Saving 0x%04x in %s\n", data, out_file);
				break;
			case -1:
				printf("Error opening output file: %s\n", out_file);
				return -1;
				break;
			case -2:
				printf("Error writing output file: %s\n", out_file);
				return -1;
				break;
			default:
				printf("Output file error\n");
				return -1;
				break;
		}
	}

	// print cycles used
	printf("\nCycles: %d\n", cycle_count);

   return 0;
}

void log_op(CPU *cpu, struct opreturn opr)
{
	printf("0x%04X %-9s ", cpu->PC - opr.bytes, opr.mnemonic);

	if (opr.bytes == 1)
	{
		printf("(%d byte)  ", opr.bytes);
	}
	else
	{
		printf("(%d bytes) ", opr.bytes);
	}

	for (cpu->TC = 0; cpu->TC < opr.cycles; cpu->TC++)
	{
		printf(" .");
	}
	for (cpu->TC = opr.cycles - 1; cpu->TC < 7; cpu->TC++)
	{
		printf("  ");
	}

	if (opr.bytes == 1)
	{
		printf("       0x%02X", opr.result);
	}
	else
	{
		printf("0x%04X 0x%02X", opr.operand, opr.result);
	}

	printf(" %c%c%c%c%c%c%c%c", cpu->SR & N ? 'N' : '.', 
			cpu->SR & V ? 'V' : '.', '.', cpu->SR & B ? 'B' : '.', 
			cpu->SR & D ? 'D' : '.', cpu->SR & I ? 'I' : '.',
			cpu->SR & Z ? 'Z' : '.', cpu->SR & C ? 'C' : '.');

	printf("\n");
}
