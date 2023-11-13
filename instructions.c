// 6502instructions.c
//
// 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece
//
// Note:  The functions in this file are in pseudo-alphabetical order,
// 	not in opcode order like in the header

#include <stdio.h>

#include "instructions.h"

void log_op_start(CPU *cpu, char *op, int bytes)
{
// Print operation address, mnemonic, and bytes consumed
	printf("0x%04X %-9s (%d bytes) ", cpu->PC, op, bytes);
}

void log_op_end(CPU *cpu, byte result, int cycles)
// Count cycles, print operation result & status register
{
	for (cpu->TC = 0; cpu->TC < cycles; cpu->TC++)
	{
		printf(" .");
	}
	for (cpu->TC = cycles - 1; cpu->TC < 7; cpu->TC++)
	{
		printf("  ");
	}

	printf(" 0x%02X", result);

	printf(" %c%c%c%c%c%c%c", cpu->SR & N ? 'N' : 'n', 
			cpu->SR & V ? 'V' : 'v', cpu->SR & B ? 'B' : 'b', 
			cpu->SR & D ? 'D' : 'd', cpu->SR & I ? 'I' : 'i',
			cpu->SR & Z ? 'Z' : 'z', cpu->SR & C ? 'C' : 'c');

	printf("\n");
}

int do_ADC_zpg(CPU *cpu)
// Add with Carry, zero page addressing
// 	A, C = A + M + C
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "ADC zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = fetch(cpu, addr);

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_ADC_imm(CPU *cpu)
// Add with Carry, immediate addressing
// 	A, C = A + M + C
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "ADC # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_BCC_rel(CPU *cpu)
// Branch on carry clear
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BCC rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if C = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & C) == 0)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BCS_rel(CPU *cpu)
// Branch on carry set
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BCS rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if C = 1
	//		Branch adds 1 cycle		
	if ((cpu->SR & C) == C)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BEQ_rel(CPU *cpu)
// Branch on result equal t0 zero
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BEQ rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if Z = 1
	//		Branch adds 1 cycle		
	if ((cpu->SR & Z) == Z)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BMI_rel(CPU *cpu)
// Branch on result minus
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BMI rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if N = 1
	//		Branch adds 1 cycle		
	if ((cpu->SR & N) == N)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BNE_rel(CPU *cpu)
// Branch on result not zero
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BNE rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if Z = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & Z) == 0)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BPL_rel(CPU *cpu)
// Branch on result plus
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BPL rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if N = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & N) == 0)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BRK_impl(CPU *cpu)
// This is not a correct implementation of this instruction.  It's
// just a convenient way to end programs
{
	int nbytes = 1;
	int ncycles = 7;

	log_op_start(cpu, "BRK   ", nbytes);
	log_op_end(cpu, 0, ncycles);

	return ncycles;
}

int do_BVC_rel(CPU *cpu)
// Branch on overflow clear
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BVC rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if V = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & V) == 0)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_BVS_rel(CPU *cpu)
// Branch on overflow set
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "BVS rel", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if V = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & V) == V)
	{
		ncycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			ncycles += 1;

			// Increment ADH if carry is set
			if ((ADL & bit8) != 0)
			{
				ADH += 1;
			}
			// or decrement ADH if carry is cleared
			else
			{
				ADH -= 1;
			}

		}

		// Store new address in PC
		// 	Mask halves to clear out any carry bits
		cpu->PC = ((ADH & 0xFF)<<8) + (ADL & 0xFF);
	}

	log_op_end(cpu, M, ncycles);

	return ncycles;
}

int do_CLC_impl(CPU *cpu)
// Clear Carry flag
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "CLC   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Clear C
	cpu->SR &= ~C;

	log_op_end(cpu, cpu->SR, ncycles);

	return ncycles;
}

int do_CMP_imm(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "CMP # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_CPX_imm(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = X + ~M + C
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "CPX # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	int result = cpu->X + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_CPY_imm(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = Y + ~M + C
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "CPY # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	int result = cpu->Y + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_DEX_impl(CPU *cpu)
// Decrement X register
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "DEX   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Decrement X
	// 	set N,Z if necessary
	cpu->X--;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_DEY_impl(CPU *cpu)
// Decrement Y register
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "DEY   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Decrement Y
	// 	set N,Z if necessary
	cpu->Y--;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_INX_impl(CPU *cpu)
// Increment X register
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "INX   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment X
	// 	set N,Z if necessary
	cpu->X++;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_INY_impl(CPU *cpu)
// Increment Y register
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "INY   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment Y
	// 	set N,Z if necessary
	cpu->Y++;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_JMP_abs(CPU *cpu)
// Jump with absolute addressing
{
	int nbytes = 3;
	int ncycles = 3;

	log_op_start(cpu, "JMP abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	byte adl = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	//		set PC for jump
	byte adh = fetch(cpu, cpu->PC);
	cpu->PC++;
	cpu->PC = adl + (adh << 8);

	log_op_end(cpu, adl, ncycles);

	return ncycles;
}

int do_JMP_ind(CPU *cpu)
// Jump with absolute indirect addressing
{
	int nbytes = 3;
	int ncycles = 5;

	log_op_start(cpu, "JMP ind", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address location, incement PC
	byte all = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address location, increment PC
	byte alh = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 3: fetch low byte of address
	byte adl = fetch(cpu, all + (alh << 8));

	// Cycle 4: fetch high byte of address
	//		set PC for jump
	//		This will not cross page boundaries when all = 0xff -- normal 
	//			behavior for a MOS 6502.
	all = all + 1;
	byte adh = fetch(cpu, all + (alh << 8));
	cpu->PC = adl + (adh << 8);

	log_op_end(cpu, adl, ncycles);

	return ncycles;
}

int do_LDA_imm(CPU *cpu)
// Load Accumulator with immediate addressing
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "LDA # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in A, increment PC
	// 	set N,Z if necessary
	cpu->A = fetch(cpu, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_abs(CPU *cpu)
// Load Accumulator with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDA abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = cpu->mem[addr];

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_absX(CPU *cpu)
// Load Accumulator with x-indexed absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDA absX", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = fetch(cpu, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load A and be done 
	if (bal < 256)
	{
		word addr = (bah << 8) + bal;
		cpu->A = cpu->mem[addr];
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		ncycles += 1;

		// Cycle 4:  store byte in A
		word addr = (bah << 8) + bal;
		cpu->A = cpu->mem[addr]; 
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_absY(CPU *cpu)
// Load Accumulator with Y-indexed absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDA absY", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = fetch(cpu, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load A and be done 
	if (bal < 256)
	{
		word addr = (bah << 8) + bal;
		cpu->A = cpu->mem[addr];
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		ncycles += 1;

		// Cycle 4:  store byte in A
		word addr = (bah << 8) + bal;
		cpu->A = cpu->mem[addr]; 
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_zpg(CPU *cpu)
// Load Accumulator with zero page addressing
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "LDA zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = cpu->mem[addr];

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_zpgX(CPU *cpu)
// Load Accumulator with X-indexed zero page addressing
{
	int nbytes = 2;
	int ncycles = 4;

	log_op_start(cpu, "LDA zpg,X", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = cpu->mem[addr];

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_Xind(CPU *cpu)
// Load Accumulator with X-indexed zero page indirect addressing
{
	int nbytes = 2;
	int ncycles = 6;

	log_op_start(cpu, "LDA X,ind", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte zad = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = fetch(cpu, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = fetch(cpu, zad);

	// Cycle 5:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = cpu->mem[(adh << 8) + adl];

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDA_indY(CPU *cpu)
// Load Accumulator with zero page indirect Y-indexed addressing
{
	int nbytes = 2;
	int ncycles = 5;

	log_op_start(cpu, "LDA ind,Y", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = fetch(cpu, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = fetch(cpu, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, load A and be done 
	if (bal < 256)
	{
		word addr = (bah << 8) + bal;
		cpu->A = cpu->mem[addr];
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		ncycles += 1;

		// Cycle 4:  store byte in A
		word addr = (bah << 8) + bal;
		cpu->A = cpu->mem[addr]; 
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_LDX_imm(CPU *cpu)
// Load X register with immediate addressing
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "LDX # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in X, increment PC
	// 	set N,Z if necessary
	cpu->X = fetch(cpu, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_LDX_abs(CPU *cpu)
// Load X register with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDX abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store byte in X
	// 	Set N,Z if necessary
	cpu->X = cpu->mem[addr];

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_LDX_absY(CPU *cpu)
// Load X register with Y-indexed absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDX absY", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = fetch(cpu, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load X and be done 
	if (bal < 256)
	{
		word addr = (bah << 8) + bal;
		cpu->X = cpu->mem[addr];
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		ncycles += 1;

		// Cycle 4:  store byte in X
		word addr = (bah << 8) + bal;
		cpu->X = cpu->mem[addr]; 
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_LDX_zpg(CPU *cpu)
// Load X register with zero page addressing
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "LDX zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store byte in X
	// 	Set N,Z if necessary
	cpu->X = cpu->mem[addr];

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_LDX_zpgY(CPU *cpu)
// Load X register with Y-indexed zero page addressing
{
	int nbytes = 2;
	int ncycles = 4;

	log_op_start(cpu, "LDX zpg,Y", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add Y to address
	addr = addr + cpu->Y;

	// Cycle 3:  store byte in X
	// 	Set N,Z if necessary
	cpu->X = cpu->mem[addr];

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_LDY_imm(CPU *cpu)
// Load Y register with immediate addressing
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "LDY # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in Y, increment PC
	// 	set N,Z if necessary
	cpu->Y = fetch(cpu, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_LDY_abs(CPU *cpu)
// Load Y register with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDY abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store byte in Y
	// 	Set N,Z if necessary
	cpu->Y = cpu->mem[addr];

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_LDY_absX(CPU *cpu)
// Load Y register with X-indexed absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "LDY absX", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = fetch(cpu, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load Y and be done 
	if (bal < 256)
	{
		word addr = (bah << 8) + bal;
		cpu->Y = cpu->mem[addr];
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		ncycles += 1;

		// Cycle 4:  store byte in Y
		word addr = (bah << 8) + bal;
		cpu->Y = cpu->mem[addr]; 
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_LDY_zpg(CPU *cpu)
// Load Y register with zero page addressing
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "LDY zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store byte in Y
	// 	Set N,Z if necessary
	cpu->Y = cpu->mem[addr];

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_LDY_zpgX(CPU *cpu)
// Load Y register with X-indexed zero page addressing
{
	int nbytes = 2;
	int ncycles = 4;

	log_op_start(cpu, "LDY zpg,X", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store byte in Y
	// 	Set N,Z if necessary
	cpu->Y = cpu->mem[addr];

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_NOP_impl(CPU *cpu)
// No operation
{
	int nbytes = 1;
	int ncycles = 2;
	
	log_op_start(cpu, "NOP   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Do nothing
	
	log_op_end(cpu, 0, ncycles);

	return ncycles;
}

int do_SBC_zpg(CPU *cpu)
// Subtract with Carry, zero page addressing
// 	A, C = A + ~M + C
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "SBC zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = fetch(cpu, addr);

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_SBC_imm(CPU *cpu)
// Subtract with Carry, immediate addressing
// 	A, C = A + ~M + C
{
	int nbytes = 2;
	int ncycles = 2;

	log_op_start(cpu, "SBC # ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = fetch(cpu, cpu->PC);
	cpu->PC++;

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_SEC_impl(CPU *cpu)
// Set Carry flag
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "SEC   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Set C
	cpu->SR |= C;

	log_op_end(cpu, cpu->SR, ncycles);

	return ncycles;
}

int do_STA_abs(CPU *cpu)
// Store Accumulator in memory with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "STA abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store A at addr
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STA_absX(CPU *cpu)
// Store Accumulator in memory with X-indexed absolute addressing
{
	int nbytes = 3;
	int ncycles = 5;

	log_op_start(cpu, "STA absX", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = fetch(cpu, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4:  store A at addr
	word addr = (bah << 8) + bal;
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STA_absY(CPU *cpu)
// Store Accumulator in memory with Y-indexed absolute addressing
{
	int nbytes = 3;
	int ncycles = 5;

	log_op_start(cpu, "STA absY", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = fetch(cpu, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4:  store A at addr
	word addr = (bah << 8) + bal;
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STA_zpg(CPU *cpu)
// Store Accumulator in memory with zero page addressing
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "STA zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store A at addr
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STA_zpgX(CPU *cpu)
// Store Accumulator in memory with X-indexed zero page addressing
{
	int nbytes = 2;
	int ncycles = 4;

	log_op_start(cpu, "STA zpg,X", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store A at addr
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STA_Xind(CPU *cpu)
// Store Accumulator in memory with X-indexed zero page indirect addressing
{
	int nbytes = 2;
	int ncycles = 6;

	log_op_start(cpu, "STA X,ind", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte zad = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address
	byte adl = fetch(cpu, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = fetch(cpu, zad);

	// Cycle 5:  store A at addr
	cpu->mem[(adh << 8) + adl] = cpu->A;

	log_op_end(cpu, cpu->mem[(adh << 8) + adl], ncycles);

	return ncycles;
}

int do_STA_indY(CPU *cpu)
// Store Accumulator in memory with zero page indirect Y-indexed addressing
{
	int nbytes = 2;
	int ncycles = 6;

	log_op_start(cpu, "STA ind,Y", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte zad = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = fetch(cpu, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = fetch(cpu, zad);
	bal = bal + cpu->Y;

	// Cycle 4: add carry to high byte of address if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF; // Trim off carry bit
	}

	// Cycle 5:  store A at addr
	word addr = (bah << 8) + bal;
	cpu->mem[addr] = cpu->A;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STX_abs(CPU *cpu)
// Store X register in memory with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "STX abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store X at addr
	cpu->mem[addr] = cpu->X;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STX_zpg(CPU *cpu)
// Store X register in memory with zero page addressing
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "STX zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store X at addr
	cpu->mem[addr] = cpu->X;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STX_zpgY(CPU *cpu)
// Store X register in memory with Y-indexed zero page addressing
{
	int nbytes = 2;
	int ncycles = 4;

	log_op_start(cpu, "STX zpg,Y", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add Y to address
	addr = addr + cpu->Y;

	// Cycle 3:  store X at addr
	cpu->mem[addr] = cpu->X;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STY_abs(CPU *cpu)
// Store Y register in memory with absolute addressing
{
	int nbytes = 3;
	int ncycles = 4;

	log_op_start(cpu, "STY abs", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (fetch(cpu, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store Y at addr
	cpu->mem[addr] = cpu->Y;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STY_zpg(CPU *cpu)
// Store Y register in memory with absolute addressing
{
	int nbytes = 2;
	int ncycles = 3;

	log_op_start(cpu, "STY zpg", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store Y at addr
	cpu->mem[addr] = cpu->Y;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_STY_zpgX(CPU *cpu)
// Store Y register in memory with X-indexed zero page addressing
{
	int nbytes = 2;
	int ncycles = 4;

	log_op_start(cpu, "STY zpg,X", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = fetch(cpu, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store Y at addr
	cpu->mem[addr] = cpu->Y;

	log_op_end(cpu, cpu->mem[addr], ncycles);

	return ncycles;
}

int do_TAX_impl(CPU *cpu)
// Transfer Accumulator to X
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "TAX   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from A to X
	// 	set N,Z if necessary
	cpu->X = cpu->A;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	log_op_end(cpu, cpu->X, ncycles);

	return ncycles;
}

int do_TAY_impl(CPU *cpu)
// Transfer Accumulator to Y
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "TAY   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from A to Y
	// 	set N,Z if necessary
	cpu->Y = cpu->A;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	log_op_end(cpu, cpu->Y, ncycles);

	return ncycles;
}

int do_TXA_impl(CPU *cpu)
// Transfer X to Accumulator
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "TXA   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from X to A
	// 	set N,Z if necessary
	cpu->A = cpu->X;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int do_TYA_impl(CPU *cpu)
// Transfer Y to Accumulator
{
	int nbytes = 1;
	int ncycles = 2;

	log_op_start(cpu, "TYA   ", nbytes);

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from Y to A
	// 	set N,Z if necessary
	cpu->A = cpu->Y;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	log_op_end(cpu, cpu->A, ncycles);

	return ncycles;
}

int (*execute[])(CPU *cpu) =
{
do_BRK_impl, 	// 0x00
do_NOP_impl, 	// 0x01
do_NOP_impl, 	// 0x02
do_NOP_impl, 	// 0x03
do_NOP_impl, 	// 0x04
do_NOP_impl, 	// 0x05
do_NOP_impl, 	// 0x06
do_NOP_impl, 	// 0x07
do_NOP_impl, 	// 0x08
do_NOP_impl, 	// 0x09
do_NOP_impl, 	// 0x0A
do_NOP_impl, 	// 0x0B
do_NOP_impl, 	// 0x0C
do_NOP_impl, 	// 0x0D
do_NOP_impl, 	// 0x0E
do_NOP_impl, 	// 0x0F
do_BPL_rel, 	// 0x10
do_NOP_impl, 	// 0x11
do_NOP_impl, 	// 0x12
do_NOP_impl, 	// 0x13
do_NOP_impl, 	// 0x14
do_NOP_impl, 	// 0x15
do_NOP_impl, 	// 0x16
do_NOP_impl, 	// 0x17
do_CLC_impl, 	// 0x18
do_NOP_impl, 	// 0x19
do_NOP_impl, 	// 0x1A
do_NOP_impl, 	// 0x1B
do_NOP_impl, 	// 0x1C
do_NOP_impl, 	// 0x1D
do_NOP_impl, 	// 0x1E
do_NOP_impl, 	// 0x1F
do_NOP_impl, 	// 0x20
do_NOP_impl, 	// 0x21
do_NOP_impl, 	// 0x22
do_NOP_impl, 	// 0x23
do_NOP_impl, 	// 0x24
do_NOP_impl, 	// 0x25
do_NOP_impl, 	// 0x26
do_NOP_impl, 	// 0x27
do_NOP_impl, 	// 0x28
do_NOP_impl, 	// 0x29
do_NOP_impl, 	// 0x2A
do_NOP_impl, 	// 0x2B
do_NOP_impl, 	// 0x2C
do_NOP_impl, 	// 0x2D
do_NOP_impl, 	// 0x2E
do_NOP_impl, 	// 0x2F
do_BMI_rel, 	// 0x30
do_NOP_impl, 	// 0x31
do_NOP_impl, 	// 0x32
do_NOP_impl, 	// 0x33
do_NOP_impl, 	// 0x34
do_NOP_impl, 	// 0x35
do_NOP_impl, 	// 0x36
do_NOP_impl, 	// 0x37
do_SEC_impl, 	// 0x38
do_NOP_impl, 	// 0x39
do_NOP_impl, 	// 0x3A
do_NOP_impl, 	// 0x3B
do_NOP_impl, 	// 0x3C
do_NOP_impl, 	// 0x3D
do_NOP_impl, 	// 0x3E
do_NOP_impl, 	// 0x3F
do_NOP_impl, 	// 0x40
do_NOP_impl, 	// 0x41
do_NOP_impl, 	// 0x42
do_NOP_impl, 	// 0x43
do_NOP_impl, 	// 0x44
do_NOP_impl, 	// 0x45
do_NOP_impl, 	// 0x46
do_NOP_impl, 	// 0x47
do_NOP_impl, 	// 0x48
do_NOP_impl, 	// 0x49
do_NOP_impl, 	// 0x4A
do_NOP_impl, 	// 0x4B
do_JMP_abs, 	// 0x4C
do_NOP_impl, 	// 0x4D
do_NOP_impl, 	// 0x4E
do_NOP_impl, 	// 0x4F
do_BVC_rel, 	// 0x50
do_NOP_impl, 	// 0x51
do_NOP_impl, 	// 0x52
do_NOP_impl, 	// 0x53
do_NOP_impl, 	// 0x54
do_NOP_impl, 	// 0x55
do_NOP_impl, 	// 0x56
do_NOP_impl, 	// 0x57
do_NOP_impl, 	// 0x58
do_NOP_impl, 	// 0x59
do_NOP_impl, 	// 0x5A
do_NOP_impl, 	// 0x5B
do_NOP_impl, 	// 0x5C
do_NOP_impl, 	// 0x5D
do_NOP_impl, 	// 0x5E
do_NOP_impl, 	// 0x5F
do_NOP_impl, 	// 0x60
do_NOP_impl, 	// 0x61
do_NOP_impl, 	// 0x62
do_NOP_impl, 	// 0x63
do_NOP_impl, 	// 0x64
do_ADC_zpg, 	// 0x65
do_NOP_impl, 	// 0x66
do_NOP_impl, 	// 0x67
do_NOP_impl, 	// 0x68
do_ADC_imm, 	// 0x69
do_NOP_impl, 	// 0x6A
do_NOP_impl, 	// 0x6B
do_JMP_ind, 	// 0x6C
do_NOP_impl, 	// 0x6D
do_NOP_impl, 	// 0x6E
do_NOP_impl, 	// 0x6F
do_BVS_rel, 	// 0x70
do_NOP_impl, 	// 0x71
do_NOP_impl, 	// 0x72
do_NOP_impl, 	// 0x73
do_NOP_impl, 	// 0x74
do_NOP_impl, 	// 0x75
do_NOP_impl, 	// 0x76
do_NOP_impl, 	// 0x77
do_NOP_impl, 	// 0x78
do_NOP_impl, 	// 0x79
do_NOP_impl, 	// 0x7A
do_NOP_impl, 	// 0x7B
do_NOP_impl, 	// 0x7C
do_NOP_impl, 	// 0x7D
do_NOP_impl, 	// 0x7E
do_NOP_impl, 	// 0x7F
do_NOP_impl, 	// 0x80
do_STA_Xind, 	// 0x81
do_NOP_impl, 	// 0x82
do_NOP_impl, 	// 0x83
do_STY_zpg, 	// 0x84
do_STA_zpg, 	// 0x85
do_STX_zpg, 	// 0x86
do_NOP_impl, 	// 0x87
do_DEY_impl, 	// 0x88
do_NOP_impl, 	// 0x89
do_TXA_impl, 	// 0x8A
do_NOP_impl, 	// 0x8B
do_STY_abs, 	// 0x8C
do_STA_abs, 	// 0x8D
do_STX_abs, 	// 0x8E
do_NOP_impl, 	// 0x8F
do_BCC_rel, 	// 0x90
do_STA_indY, 	// 0x91
do_NOP_impl, 	// 0x92
do_NOP_impl, 	// 0x93
do_STY_zpgX, 	// 0x94
do_STA_zpgX, 	// 0x95
do_STX_zpgY, 	// 0x96
do_NOP_impl, 	// 0x97
do_TYA_impl, 	// 0x98
do_STA_absY, 	// 0x99
do_NOP_impl, 	// 0x9A
do_NOP_impl, 	// 0x9B
do_NOP_impl, 	// 0x9C
do_STA_absX, 	// 0x9D
do_NOP_impl, 	// 0x9E
do_NOP_impl, 	// 0x9F
do_LDY_imm, 	// 0xA0
do_LDA_Xind, 	// 0xA1
do_LDX_imm, 	// 0xA2
do_NOP_impl, 	// 0xA3
do_LDY_zpg, 	// 0xA4
do_LDA_zpg, 	// 0xA5
do_LDX_zpg, 	// 0xA6
do_NOP_impl, 	// 0xA7
do_TAY_impl, 	// 0xA8
do_LDA_imm, 	// 0xA9
do_TAX_impl, 	// 0xAA
do_NOP_impl, 	// 0xAB
do_LDY_abs, 	// 0xAC
do_LDA_abs, 	// 0xAD
do_LDX_abs, 	// 0xAE
do_NOP_impl, 	// 0xAF
do_BCS_rel, 	// 0xB0
do_LDA_indY, 	// 0xB1
do_NOP_impl, 	// 0xB2
do_NOP_impl, 	// 0xB3
do_LDY_zpgX, 	// 0xB4
do_LDA_zpgX, 	// 0xB5
do_LDX_zpgY, 	// 0xB6
do_NOP_impl, 	// 0xB7
do_NOP_impl, 	// 0xB8
do_LDA_absY, 	// 0xB9
do_NOP_impl, 	// 0xBA
do_NOP_impl, 	// 0xBB
do_LDY_absX, 	// 0xBC
do_LDA_absX, 	// 0xBD
do_LDX_absY, 	// 0xBE
do_NOP_impl, 	// 0xBF
do_CPY_imm, 	// 0xC0
do_NOP_impl, 	// 0xC1
do_NOP_impl, 	// 0xC2
do_NOP_impl, 	// 0xC3
do_NOP_impl, 	// 0xC4
do_NOP_impl, 	// 0xC5
do_NOP_impl, 	// 0xC6
do_NOP_impl, 	// 0xC7
do_INY_impl, 	// 0xC8
do_CMP_imm, 	// 0xC9
do_DEX_impl, 	// 0xCA
do_NOP_impl, 	// 0xCB
do_NOP_impl, 	// 0xCC
do_NOP_impl, 	// 0xCD
do_NOP_impl, 	// 0xCE
do_NOP_impl, 	// 0xCF
do_BNE_rel, 	// 0xD0
do_NOP_impl, 	// 0xD1
do_NOP_impl, 	// 0xD2
do_NOP_impl, 	// 0xD3
do_NOP_impl, 	// 0xD4
do_NOP_impl, 	// 0xD5
do_NOP_impl, 	// 0xD6
do_NOP_impl, 	// 0xD7
do_NOP_impl, 	// 0xD8
do_NOP_impl, 	// 0xD9
do_NOP_impl, 	// 0xDA
do_NOP_impl, 	// 0xDB
do_NOP_impl, 	// 0xDC
do_NOP_impl, 	// 0xDD
do_NOP_impl, 	// 0xDE
do_NOP_impl, 	// 0xDF
do_CPX_imm, 	// 0xE0
do_NOP_impl, 	// 0xE1
do_NOP_impl, 	// 0xE2
do_NOP_impl, 	// 0xE3
do_NOP_impl, 	// 0xE4
do_SBC_zpg, 	// 0xE5
do_NOP_impl, 	// 0xE6
do_NOP_impl, 	// 0xE7
do_INX_impl, 	// 0xE8
do_SBC_imm, 	// 0xE9
do_NOP_impl, 	// 0xEA
do_NOP_impl, 	// 0xEB
do_NOP_impl, 	// 0xEC
do_NOP_impl, 	// 0xED
do_NOP_impl, 	// 0xEE
do_NOP_impl, 	// 0xEF
do_BEQ_rel, 	// 0xF0
do_NOP_impl, 	// 0xF1
do_NOP_impl, 	// 0xF2
do_NOP_impl, 	// 0xF3
do_NOP_impl, 	// 0xF4
do_NOP_impl, 	// 0xF5
do_NOP_impl, 	// 0xF6
do_NOP_impl, 	// 0xF7
do_NOP_impl, 	// 0xF8
do_NOP_impl, 	// 0xF9
do_NOP_impl, 	// 0xFA
do_NOP_impl, 	// 0xFB
do_NOP_impl, 	// 0xFC
do_NOP_impl, 	// 0xFD
do_NOP_impl, 	// 0xFE
do_NOP_impl  	// 0xFF
};
