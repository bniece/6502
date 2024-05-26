// instructions.c
//
// 6502 emulator program
// 	Instruction structure and functions
//
// Brian K. Niece
//
// Note:  The functions in this file are in pseudo-alphabetical order,
// 	not in opcode order like in the header

#include "instructions.h"

struct opreturn do_ADC_imm(CPU *cpu)
// Add with Carry, immediate addressing
// 	A, C = A + M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "ADC # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = M;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_abs(CPU *cpu)
// Add with Carry, absolute addressing
// 	A, C = A + M + C
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ADC abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_absX(CPU *cpu)
// Add with Carry, x-indexed absolute addressing
// 	A, C = A + M + C
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ADC absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_absY(CPU *cpu)
// Add with Carry, y-indexed absolute addressing
// 	A, C = A + M + C
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ADC absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_zpg(CPU *cpu)
// Add with Carry, zero page addressing
// 	A, C = A + M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "ADC zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_zpgX(CPU *cpu)
// Add with Carry, X indexed zero page addressing
// 	A, C = A + M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "ADC zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_Xind(CPU *cpu)
// Add with Carry, X indexed zero page indirect addressing
// 	A, C = A + M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "ADC X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_indY(CPU *cpu)
// Add with Carry, zero page indirect Y indexed addressing
// 	A, C = A + M + C
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "ADC ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 5:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	// 	Do the addition and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	int result = cpu->A + M + ((cpu->SR & C)?1:0);
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_imm(CPU *cpu)
// AND, immediate addressing
// 	A = A & M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "AND # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = M;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_abs(CPU *cpu)
// AND, absolute addressing
// 	A = A & M
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "AND abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_absX(CPU *cpu)
// AND, x-indexed absolute addressing
// 	A = A & M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "AND absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_absY(CPU *cpu)
// AND, y-indexed absolute addressing
// 	A = A & M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "AND absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_zpg(CPU *cpu)
// AND, zero page addressing
// 	A = A & M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "AND zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_zpgX(CPU *cpu)
// AND, X indexed zero page addressing
// 	A = A & M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "AND zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_Xind(CPU *cpu)
// AND, X indexed zero page indirect addressing
// 	A = A & M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "AND X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_AND_indY(CPU *cpu)
// AND, zero page indirect Y indexed addressing
// 	A = A & M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "AND ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 5:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	// 	Do the AND operation
	// 	Set N,Z if necessary
	// 	Store in A
	int result = cpu->A & M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ASL_A(CPU *cpu)
// Arithmetic shift left accumulator
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "ASL A ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: Copy bit 7 (N) to carry and shift left
	if ((cpu->A & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	cpu->A <<= 1;

	//   Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ASL_abs(CPU *cpu)
// Arithmetic shift left absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "ASL abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Copy bit 7 (N) to carry and shift left
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ASL_absX(CPU *cpu)
// Arithmetic shift left x-indexed absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 7;
	opr.mnemonic = "ASL absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4: fetch byte
	word addr = (bah << 8) + bal;
	byte M = read(*cpu->bus, addr);

	// Cycle 5: Copy bit 7 (N) to carry and shift left
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 6: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ASL_zpg(CPU *cpu)
// Arithmetic shift left zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "ASL zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 3: Copy bit 7 (N) to carry and shift left
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 4: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ASL_zpgX(CPU *cpu)
// Arithmetic shift left x-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "ASL zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Copy bit 7 (N) to carry and shift left
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_BCC_rel(CPU *cpu)
// Branch on carry clear
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BCC rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if C = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & C) == 0)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = cpu->PC;
	opr.result = M;

	return opr;
}

struct opreturn do_BCS_rel(CPU *cpu)
// Branch on carry set
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BCS rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if C = 1
	//		Branch adds 1 cycle		
	if ((cpu->SR & C) == C)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = cpu->PC;
	opr.result = M;

	return opr;
}

struct opreturn do_BEQ_rel(CPU *cpu)
// Branch on result equal to zero
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BEQ rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if Z = 1
	//		Branch adds 1 cycle		
	if ((cpu->SR & Z) == Z)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = cpu->PC;
	opr.result = M;

	return opr;
}

struct opreturn do_BIT_abs(CPU *cpu)
// BIT, absolute addressing
// 	N, V from M, Z from M & A
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "BIT abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the AND operation
	// 	Set N,V,Z as necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->A & M;

	set_N(cpu, M);
	if ((M & V) == 0)
	{
		cpu->SR &= ~V;
	}
	else
	{
		cpu->SR |= V;
	}
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_BIT_zpg(CPU *cpu)
// BIT, zero page addressing
// 	N, V from M, Z from M & A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "BIT zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the AND operation
	// 	Set N,V,Z as necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->A & M;

	set_N(cpu, M);
	if ((M & V) == 0)
	{
		cpu->SR &= ~V;
	}
	else
	{
		cpu->SR |= V;
	}
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_BMI_rel(CPU *cpu)
// Branch on result minus
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BMI rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if N = 1
	//		Branch adds 1 cycle		
	if ((cpu->SR & N) == N)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = cpu->PC;
	opr.result = M;

	return opr;
}

struct opreturn do_BNE_rel(CPU *cpu)
// Branch on result not zero
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BNE rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if Z = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & Z) == 0)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = cpu->PC;
	opr.result = M;

	return opr;
}

struct opreturn do_BPL_rel(CPU *cpu)
// Branch on result plus
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BPL rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if N = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & N) == 0)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = cpu->PC;
	opr.result = M;

	return opr;
}

struct opreturn do_BRK_impl(CPU *cpu)
// This is not a correct implementation of this instruction.  It's
// just a convenient way to end programs
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 7;
	opr.mnemonic = "BRK   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	opr.operand = -1;
	opr.result = 0;

	return opr;
}

struct opreturn do_BVC_rel(CPU *cpu)
// Branch on overflow clear
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BVC rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if V = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & V) == 0)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = -1;
	opr.result = M;

	return opr;
}

struct opreturn do_BVS_rel(CPU *cpu)
// Branch on overflow set
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "BVS rel";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//	Add offset to program counter if V = 0
	//		Branch adds 1 cycle		
	if ((cpu->SR & V) == V)
	{
		opr.cycles += 1;

		int ADL = (cpu->PC & 0xFF) + M;
		int ADH = (cpu->PC & 0xFF00)>>8;


		// Check for crossing page boundary, add 1 cycle if so
		//		Page crossed when (carry XOR sign of M) is true
		if ((ADL & bit8)>>8 != (M & N)>>7)
		{
			opr.cycles += 1;

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

	opr.operand = -1;
	opr.result = M;

	return opr;
}

struct opreturn do_CLC_impl(CPU *cpu)
// Clear Carry flag
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "CLC   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Clear C
	cpu->SR &= ~C;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}

struct opreturn do_CLI_impl(CPU *cpu)
// Clear interrupt disable
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "CLI   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Clear I
	cpu->SR &= ~I;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}

// CLD is below the execute array assignments

struct opreturn do_CLV_impl(CPU *cpu)
// Clear Overflow flag
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "CLV   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Clear V
	cpu->SR &= ~V;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}

struct opreturn do_CMP_imm(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "CMP # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_abs(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "CMP abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_absX(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "CMP absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_absY(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "CMP absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_zpg(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "CMP zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_zpgX(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "CMP zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_Xind(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "CMP Xind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CMP_indY(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = A + ~M + C
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "CMP indY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 5:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	int result = cpu->A + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_CPX_imm(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = X + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "CPX # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->X + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = -1;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_CPX_abs(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = X + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "CPX abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->X + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_CPX_zpg(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = X + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "CPX zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->X + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_CPY_imm(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = Y + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "CPY # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->Y + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = -1;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_CPY_abs(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = Y + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "CPY abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->Y + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_CPY_zpg(CPU *cpu)
// Subtract but don't update A, set Z, C, N
// 	Z,C,N = Y + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "CPY zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,Z if necessary
	byte M = read(*cpu->bus, addr);

	int result = cpu->Y + (~M&0xFF) + 1;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	//		Subtraction is done as if preceded by SEC, so just add 1 to make the
	//		twos complement
	set_C(cpu, result);

	set_N(cpu, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_DEC_abs(CPU *cpu)
// Decrement memory
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "DEC abs ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Decrement byte
	// 	set N,Z if necessary
	M--;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_DEC_absX(CPU *cpu)
// Decrement memory
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 7;
	opr.mnemonic = "DEC absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4: fetch byte
	word addr = (bah << 8) + bal;
	byte M = read(*cpu->bus, addr);

	// Cycle 5: Decrement byte
	// 	set N,Z if necessary
	M--;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 6: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_DEC_zpg(CPU *cpu)
// Decrement memory
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "DEC zpg ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 3: Decrement byte
	// 	set N,Z if necessary
	M--;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 4: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_DEC_zpgX(CPU *cpu)
// Decrement memory
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "DEC zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Decrement byte
	// 	set N,Z if necessary
	M--;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_EOR_imm(CPU *cpu)
// XOR, immediate addressing
// 	A = A ^ M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "EOR # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_abs(CPU *cpu)
// XOR, absolute addressing
// 	A = A ^ M
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "EOR abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_absX(CPU *cpu)
// XOR, x-indexed absolute addressing
// 	A = A ^ M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "EOR absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_absY(CPU *cpu)
// XOR, y-indexed absolute addressing
// 	A = A ^ M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "EOR absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_zpg(CPU *cpu)
// XOR, zero page addressing
// 	A = A ^ M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "EOR zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_zpgX(CPU *cpu)
// XOR, X indexed zero page addressing
// 	A = A ^ M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "EOR zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_Xind(CPU *cpu)
// XOR, X indexed zero page indirect addressing
// 	A = A ^ M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "EOR X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_EOR_indY(CPU *cpu)
// XOR, zero page indirect Y indexed addressing
// 	A = A ^ M
{
	word addr;
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "EOR ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 5:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	// 	Do the XOR operation
	// 	Set N,Z if necessary
	// 	Store in A
	int result = cpu->A ^ M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_INC_abs(CPU *cpu)
// Increment memory
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "INC abs ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Increment byte
	// 	set N,Z if necessary
	M++;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_INC_absX(CPU *cpu)
// Increment memory
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 7;
	opr.mnemonic = "INC absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4: fetch byte
	word addr = (bah << 8) + bal;
	byte M = read(*cpu->bus, addr);

	// Cycle 5: Decrement byte
	// 	set N,Z if necessary
	M++;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 6: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_INC_zpg(CPU *cpu)
// Increment memory
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "INC zpg ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 3: Decrement byte
	// 	set N,Z if necessary
	M++;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 4: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_INC_zpgX(CPU *cpu)
// Increment memory
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "INC zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Decrement byte
	// 	set N,Z if necessary
	M++;

	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store byte back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_DEX_impl(CPU *cpu)
// Decrement X register
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "DEX   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Decrement X
	// 	set N,Z if necessary
	cpu->X--;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = -1;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_DEY_impl(CPU *cpu)
// Decrement Y register
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "DEY   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Decrement Y
	// 	set N,Z if necessary
	cpu->Y--;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = -1;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_INX_impl(CPU *cpu)
// Increment X register
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "INX   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment X
	// 	set N,Z if necessary
	cpu->X++;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = -1;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_INY_impl(CPU *cpu)
// Increment Y register
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "INY   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment Y
	// 	set N,Z if necessary
	cpu->Y++;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = -1;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_JMP_abs(CPU *cpu)
// Jump with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 3;
	opr.mnemonic = "JMP abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	byte adl = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	//		set PC for jump
	byte adh = read(*cpu->bus, cpu->PC);
	word addr = adl + (adh << 8);
	cpu->PC++;
	cpu->PC = addr;

	opr.operand = addr;
	opr.result = adl;

	return opr;
}

struct opreturn do_JMP_ind(CPU *cpu)
// Jump with absolute indirect addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 5;
	opr.mnemonic = "JMP ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address location, incement PC
	byte all = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address location, increment PC
	byte alh = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 3: fetch low byte of address
	byte adl = read(*cpu->bus, all + (alh << 8));

	// Cycle 4: fetch high byte of address
	//		set PC for jump
	//		This will not cross page boundaries when all = 0xff -- normal 
	//			behavior for a MOS 6502.
	all = all + 1;
	byte adh = read(*cpu->bus, all + (alh << 8));
	word addr = adl + (adh << 8);
	cpu->PC = addr;

	opr.operand = addr;
	opr.result = adl;

	return opr;
}

struct opreturn do_JSR_abs(CPU *cpu)
// Jump to subroutine with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "JSR abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: push high byte of return address on stack, decrement SP
	//    This is actually the final byte of the instruction
	write(*cpu->bus, 0x100 + cpu->SP, (cpu->PC + 1) >> 8);
	cpu->SP--;

	// Cycle 2: push low byte of return address on stack, decrement SP
	write(*cpu->bus, 0x0100 + cpu->SP, (cpu->PC + 1) & 0xFF);
	cpu->SP--;
	
	// Cycle 3: fetch low byte of address, incement PC
	byte adl = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 4:  fetch high byte of address, increment PC
	byte adh = read(*cpu->bus, cpu->PC);
	word addr = adl + (adh << 8);
	cpu->PC++;

	// Cycle 5:  Set PC for jump
	cpu->PC = addr;

	opr.operand = addr;
	opr.result = adl;

	return opr;
}

struct opreturn do_ORA_imm(CPU *cpu)
// OR, immediate addressing
// 	A = A | M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "ORA # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_abs(CPU *cpu)
// OR, absolute addressing
// 	A = A | M
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ORA abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_absX(CPU *cpu)
// OR, x-indexed absolute addressing
// 	A = A | M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ORA absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_absY(CPU *cpu)
// OR, y-indexed absolute addressing
// 	A = A | M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ORA absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_zpg(CPU *cpu)
// OR, zero page addressing
// 	A = A | M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "ORA zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_zpgX(CPU *cpu)
// OR, X indexed zero page addressing
// 	A = A | M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "ORA zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_Xind(CPU *cpu)
// OR, X indexed zero page indirect addressing
// 	A = A | M
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "ORA X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ORA_indY(CPU *cpu)
// OR, zero page indirect Y indexed addressing
// 	A = A | M
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "ORA ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 5:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	// 	Do the OR operation
	// 	Set N,Z if necessary
	// 	Store in A
	int result = cpu->A | M;

	set_N(cpu, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_PHA_impl(CPU *cpu)
// Push accumulator on stack
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 3;
	opr.mnemonic = "PHA   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: copy A to stack
	write(*cpu->bus, 0x0100 + cpu->SP, cpu->A);

	// Cycle 2: Decrement stack pointer
	cpu->SP--;

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_PHP_impl(CPU *cpu)
// Push processor status on stack
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 3;
	opr.mnemonic = "PHP   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: copy SR to stack, setting B
	write(*cpu->bus, 0x0100 + cpu->SP, cpu->SR | 0x10);

	// Cycle 2: Decrement stack pointer
	cpu->SP--;

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_PLA_impl(CPU *cpu)
// Pull accumulator from stack
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 4;
	opr.mnemonic = "PLA   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment stack pointer
	cpu->SP++;

	// Cycle 2: copy byte from stack to A
	cpu->A = read(*cpu->bus, 0x0100 + cpu->SP);

	// Cycle 3: set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_PLP_impl(CPU *cpu)
// Pull processor status from stack
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 4;
	opr.mnemonic = "PLP   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment stack pointer
	cpu->SP++;

	// Cycle 2: copy byte from stack to A
	cpu->SR = read(*cpu->bus, 0x0100 + cpu->SP);

	// Cycle 3: Not sure what happens here.  Flags should be set

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_imm(CPU *cpu)
// Load Accumulator with immediate addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "LDA # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in A, increment PC
	// 	set N,Z if necessary
	cpu->A = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_abs(CPU *cpu)
// Load Accumulator with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDA abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = read(*cpu->bus, addr);

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_absX(CPU *cpu)
// Load Accumulator with x-indexed absolute addressing
{
	word addr; 				// Memory location
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDA absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load A and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		cpu->A = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  store byte in A
		addr = (bah << 8) + bal;
		cpu->A = read(*cpu->bus, addr);
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_absY(CPU *cpu)
// Load Accumulator with Y-indexed absolute addressing
{
	word addr;				// Memory location
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDA absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load A and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		cpu->A = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  store byte in A
		addr = (bah << 8) + bal;
		cpu->A = read(*cpu->bus, addr);
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_zpg(CPU *cpu)
// Load Accumulator with zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "LDA zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = read(*cpu->bus, addr);

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_zpgX(CPU *cpu)
// Load Accumulator with X-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "LDA zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = read(*cpu->bus, addr);

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_Xind(CPU *cpu)
// Load Accumulator with X-indexed zero page indirect addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "LDA X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5:  store byte in A
	// 	Set N,Z if necessary
	cpu->A = read(*cpu->bus, addr);

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDA_indY(CPU *cpu)
// Load Accumulator with zero page indirect Y-indexed addressing
{
	word addr;				// Memory location
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "LDA ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, load A and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		cpu->A = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  store byte in A
		addr = (bah << 8) + bal;
		cpu->A = read(*cpu->bus, addr);
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LDX_imm(CPU *cpu)
// Load X register with immediate addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "LDX # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in X, increment PC
	// 	set N,Z if necessary
	cpu->X = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = -1;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_LDX_abs(CPU *cpu)
// Load X register with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDX abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store byte in X
	// 	Set N,Z if necessary
	cpu->X = read(*cpu->bus, addr);

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_LDX_absY(CPU *cpu)
// Load X register with Y-indexed absolute addressing
{
	word addr;				// Memory location
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDX absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load X and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		cpu->X = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  store byte in X
		addr = (bah << 8) + bal;
		cpu->X = read(*cpu->bus, addr);
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_LDX_zpg(CPU *cpu)
// Load X register with zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "LDX zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store byte in X
	// 	Set N,Z if necessary
	cpu->X = read(*cpu->bus, addr);

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_LDX_zpgY(CPU *cpu)
// Load X register with Y-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "LDX zpg,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add Y to address
	addr = addr + cpu->Y;

	// Cycle 3:  store byte in X
	// 	Set N,Z if necessary
	cpu->X = read(*cpu->bus, addr);

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_LDY_imm(CPU *cpu)
// Load Y register with immediate addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "LDY # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and store in Y, increment PC
	// 	set N,Z if necessary
	cpu->Y = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = -1;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_LDY_abs(CPU *cpu)
// Load Y register with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDY abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store byte in Y
	// 	Set N,Z if necessary
	cpu->Y = read(*cpu->bus, addr);

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_LDY_absX(CPU *cpu)
// Load Y register with X-indexed absolute addressing
{
	word addr;				// Memory location
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "LDY absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, load Y and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		cpu->Y = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do load on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  store byte in Y
		addr = (bah << 8) + bal;
		cpu->Y = read(*cpu->bus, addr);
	}

	// Set N,Z if necessary
	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_LDY_zpg(CPU *cpu)
// Load Y register with zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "LDY zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store byte in Y
	// 	Set N,Z if necessary
	cpu->Y = read(*cpu->bus, addr);

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_LDY_zpgX(CPU *cpu)
// Load Y register with X-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "LDY zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store byte in Y
	// 	Set N,Z if necessary
	cpu->Y = read(*cpu->bus, addr);

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_LSR_A(CPU *cpu)
// Logical shift right accumulator
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "LSR A ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: Copy bit 0 to carry and shift right
	if ((cpu->A & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	cpu->A >>= 1;

	//   Clear N
	cpu->SR &= ~N;
	//   Set Z if necessary
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_LSR_abs(CPU *cpu)
// Logical shift right absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "LSR abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Copy bit 0 to carry and shift right
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;

	//   Clear N
	cpu->SR &= ~N;
	//   Set Z if necessary
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_LSR_absX(CPU *cpu)
// Logical shift rigth x-indexed absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 7;
	opr.mnemonic = "LSR absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4: fetch byte
	word addr = (bah << 8) + bal;
	byte M = read(*cpu->bus, addr);

	// Cycle 5: Copy bit 0 to carry and shift right
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;

	//   Clear N
	cpu->SR &= ~N;
	//   Set Z if necessary
	set_Z(cpu, M);

	// Cycle 6: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_LSR_zpg(CPU *cpu)
// Logical shift right zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "LSR zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 3: Copy bit 0 to carry and shift right
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;

	//   Clear N
	cpu->SR &= ~N;
	//   Set Z if necessary
	set_Z(cpu, M);

	// Cycle 4: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_LSR_zpgX(CPU *cpu)
// Logical shift right x-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "LSR zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Copy bit 0 to carry and shift rigth
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;

	//   Clear N
	cpu->SR &= ~N;
	//   Set Z if necessary
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_NOP_impl(CPU *cpu)
// No operation
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "NOP   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Do nothing
	
	opr.operand = -1;
	opr.result = 0;

	return opr;
}

struct opreturn do_ROL_A(CPU *cpu)
// Rotate left accumulator
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "ROL A ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: Save current carry state, copy bit 7 (N) to carry,
	// 	shift left, and put carry in bit 0
	int og_carry = cpu->SR & C;
	if ((cpu->A & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	cpu->A <<= 1;
	cpu->A |= og_carry;

	//   Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ROL_abs(CPU *cpu)
// Rotate left absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "ROL abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Save current carry state, copy bit 7 (N) to carry,
	// 	shift left, and put carry in bit 0
	int og_carry = cpu->SR & C;
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;
	M |= og_carry;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROL_absX(CPU *cpu)
// Rotate left x-indexed absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 7;
	opr.mnemonic = "ROL absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4: fetch byte
	word addr = (bah << 8) + bal;
	byte M = read(*cpu->bus, addr);

	// Cycle 5: Save current carry state, copy bit 7 (N) to carry,
	// 	shift left, and put carry in bit 0
	int og_carry = cpu->SR & C;
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;
	M |= og_carry;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 6: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROL_zpg(CPU *cpu)
// Rotate left zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "ROL zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 3: Save current carry state, copy bit 7 (N) to carry,
	// 	shift left, and put carry in bit 0
	int og_carry = cpu->SR & C;
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;
	M |= og_carry;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 4: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROL_zpgX(CPU *cpu)
// Rotate left x-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "ROL zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Save current carry state, copy bit 7 (N) to carry,
	// 	shift left, and put carry in bit 0
	int og_carry = cpu->SR & C;
	if ((M & N) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M <<= 1;
	M |= og_carry;

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROR_A(CPU *cpu)
// Rotate right accumulator
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "ROR A ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: Save current carry state, copy bit 0 to carry,
	// 	shift right, and put carry in bit 7
	int og_carry = cpu->SR & C;
	if ((cpu->A & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	cpu->A >>= 1;
	cpu->A |= (og_carry << 7);

	//   Set N,Z if necessary
	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ROR_abs(CPU *cpu)
// Rotate right absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 6;
	opr.mnemonic = "ROR abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Save current carry state, copy bit 0 to carry,
	// 	shift right, and put carry in bit 7
	int og_carry = cpu->SR & C;
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;
	M |= (og_carry << 7);

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROR_absX(CPU *cpu)
// Rotate right x-indexed absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 7;
	opr.mnemonic = "ROR absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  add carry to high byte if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
	}

	// Cycle 4: fetch byte
	word addr = (bah << 8) + bal;
	byte M = read(*cpu->bus, addr);

	// Cycle 5: Save current carry state, copy bit 0 to carry,
	// 	shift right, and put carry in bit 7
	int og_carry = cpu->SR & C;
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;
	M |= (og_carry << 7);

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 6: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROR_zpg(CPU *cpu)
// Rotate right zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "ROR zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 3: Save current carry state, copy bit 0 to carry,
	// 	shift right, and put carry in bit 7
	int og_carry = cpu->SR & C;
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;
	M |= (og_carry << 7);

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 4: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}

struct opreturn do_ROR_zpgX(CPU *cpu)
// Rotate right x-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "ROR zpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// Cycle 4: Save current carry state, copy bit 0 to carry,
	// 	shift right, and put carry in bit 7
	int og_carry = cpu->SR & C;
	if ((M & 0x1) == 0)
	{
		cpu->SR &= ~C;
	}
	else
	{
		cpu->SR |= C;
	}
	M >>= 1;
	M |= (og_carry << 7);

	//   Set N,Z if necessary
	set_N(cpu, M);
	set_Z(cpu, M);

	// Cycle 5: Store back in memory
	write(*cpu->bus, addr, M);

	opr.operand = addr;
	opr.result = M;

	return opr;
}
struct opreturn do_RTI_impl(CPU *cpu)
// Return from interupt with implied addressing
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 6;
	opr.mnemonic = "RTI impl";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: internal operation
	
	// Cycle 2: internal operation

	// Cycle 3: increment stack pointer, pull status register
	cpu->SP++;
	cpu->SR = read(*cpu->bus, 0x100 + cpu->SP);

	// Cycle 4: increment stack pointer, pull low byte of return address
	cpu->SP++;
	byte adl = read(*cpu->bus, 0x100 + cpu->SP);

	// Cycle 5: pull high byte of return address from stack
	cpu->SP++;
	byte adh = read(*cpu->bus, 0x100 + cpu->SP);

	//   Put return address into PC
	cpu->PC = adl + (adh << 8);

	opr.operand = -1;
	opr.result = adl;

	return opr;
}

struct opreturn do_RTS_impl(CPU *cpu)
// Return from subroutine with implied addressing
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 6;
	opr.mnemonic = "RTS impl";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Increment stack pointer
	cpu->SP++;

	// Cycle 2: pull low byte of return address from stack
	byte adl = read(*cpu->bus, 0x100 + cpu->SP);

	// Cycle 3:  Increment stack pointer
	cpu->SP++;

	// Cycle 4: pull high byte of return address from stack
	byte adh = read(*cpu->bus, 0x100 + cpu->SP);

	// Cycle 5: Put return address into PC (add 1 for next instruction)
	cpu->PC = adl + (adh << 8) + 1;

	opr.operand = -1;
	opr.result = adl;

	return opr;
}

struct opreturn do_SBC_imm(CPU *cpu)
// Subtract with Carry, immediate addressing
// 	A, C = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "SBC # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_abs(CPU *cpu)
// Subtract with Carry, absolute addressing
// 	A, C = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_absX(CPU *cpu)
// Subtract with Carry, x-indexed absolute addressing
// 	A, C = A + ~M + C
{
	word addr;			// Memory location
	byte M;				// Subtrahend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_absY(CPU *cpu)
// Subtract with Carry, y-indexed absolute addressing
// 	A, C = A + ~M + C
{
	word addr;			// Memory location
	byte M;				// Subtrahend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_zpg(CPU *cpu)
// Subtract with Carry, zero page addressing
// 	A, C = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "SBC zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_zpgX(CPU *cpu)
// Subtract with Carry, X indexed zero page addressing
// 	A, C = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "SBC zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_Xind(CPU *cpu)
// Subtract with Carry, X indexed zero page indirect addressing
// 	A, C = A + ~M + C
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "SBC X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A
	byte M = read(*cpu->bus, addr);

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_indY(CPU *cpu)
// Subtract with Carry, zero page indirect Y indexed addressing
// 	A, C = A + ~M + C
{
	word addr;			// Memory location
	byte M;				// Subtrahend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "SBC ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do fetch on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	// 	Do the subtraction and update C
	// 	Set N,V,Z if necessary
	// 	Store in A

	int result = cpu->A + (~M&0xFF) + ((cpu->SR & C)?1:0);
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);

	set_N(cpu, result);
	set_V(cpu, cpu->A, ~M, result);
	set_Z(cpu, result);

	cpu->A = result;

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SEC_impl(CPU *cpu)
// Set Carry flag
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "SEC   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Set C
	cpu->SR |= C;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}

struct opreturn do_SEI_impl(CPU *cpu)
// Set interrupt disable
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "SEI   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Set I
	cpu->SR |= I;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}

// SED is below the execute array assignment

struct opreturn do_STA_abs(CPU *cpu)
// Store Accumulator in memory with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "STA abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store A at addr
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STA_absX(CPU *cpu)
// Store Accumulator in memory with X-indexed absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 5;
	opr.mnemonic = "STA absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
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
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STA_absY(CPU *cpu)
// Store Accumulator in memory with Y-indexed absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 5;
	opr.mnemonic = "STA absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add Y to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
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
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STA_zpg(CPU *cpu)
// Store Accumulator in memory with zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "STA zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store A at addr
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STA_zpgX(CPU *cpu)
// Store Accumulator in memory with X-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "STA zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store A at addr
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STA_Xind(CPU *cpu)
// Store Accumulator in memory with X-indexed zero page indirect addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "STA X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5:  store A at addr
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STA_indY(CPU *cpu)
// Store Accumulator in memory with zero page indirect Y-indexed addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "STA ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4: add carry to high byte of address if necessary
	if (bal > 255)
	{
		bah = bah + 1;
		bal = bal & 0xFF; // Trim off carry bit
	}

	// Cycle 5:  store A at addr
	word addr = (bah << 8) + bal;
	write(*cpu->bus, addr, cpu->A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STX_abs(CPU *cpu)
// Store X register in memory with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "STX abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store X at addr
	write(*cpu->bus, addr, cpu->X);

	opr.operand = addr;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_STX_zpg(CPU *cpu)
// Store X register in memory with zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "STX zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store X at addr
	write(*cpu->bus, addr, cpu->X);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STX_zpgY(CPU *cpu)
// Store X register in memory with Y-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "STX zpg,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add Y to address
	addr = addr + cpu->Y;

	// Cycle 3:  store X at addr
	write(*cpu->bus, addr, cpu->X);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STY_abs(CPU *cpu)
// Store Y register in memory with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "STY abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  store Y at addr
	write(*cpu->bus, addr, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STY_zpg(CPU *cpu)
// Store Y register in memory with absolute addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "STY zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  store Y at addr
	write(*cpu->bus, addr, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_STY_zpgX(CPU *cpu)
// Store Y register in memory with X-indexed zero page addressing
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "STY zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;
	
	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  store Y at addr
	write(*cpu->bus, addr, cpu->Y);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_TAX_impl(CPU *cpu)
// Transfer Accumulator to X
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "TAX   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from A to X
	// 	set N,Z if necessary
	cpu->X = cpu->A;

	set_N(cpu, cpu->X);
	set_Z(cpu, cpu->X);

	opr.operand = -1;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_TSX_impl(CPU *cpu)
// Transfer Stack Pointer to X
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "TSX   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from A to X
	cpu->X = cpu->SP;

	opr.operand = -1;
	opr.result = cpu->X;

	return opr;
}

struct opreturn do_TAY_impl(CPU *cpu)
// Transfer Accumulator to Y
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "TAY   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from A to Y
	// 	set N,Z if necessary
	cpu->Y = cpu->A;

	set_N(cpu, cpu->Y);
	set_Z(cpu, cpu->Y);

	opr.operand = -1;
	opr.result = cpu->Y;

	return opr;
}

struct opreturn do_TXA_impl(CPU *cpu)
// Transfer X to Accumulator
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "TXA   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from X to A
	// 	set N,Z if necessary
	cpu->A = cpu->X;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_TXS_impl(CPU *cpu)
// Transfer X to Stack Pointers
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "TXS   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from X to SP
	cpu->SP = cpu->X;

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_TYA_impl(CPU *cpu)
// Transfer Y to Accumulator
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "TYA   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: copy byte from Y to A
	// 	set N,Z if necessary
	cpu->A = cpu->Y;

	set_N(cpu, cpu->A);
	set_Z(cpu, cpu->A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_imm_BCD(CPU *cpu)
// Add with Carry, immediate addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "ADC # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_abs_BCD(CPU *cpu)
// Add with Carry, absolute addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ADC abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3:  fetch byte
	byte M = read(*cpu->bus, addr);

	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_absX_BCD(CPU *cpu)
// Add with Carry, x-indexed absolute addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ADC absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_absY_BCD(CPU *cpu)
// Add with Carry, y-indexed absolute addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "ADC absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_zpg_BCD(CPU *cpu)
// Add with Carry, zero page addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "ADC # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch byte
	byte M = read(*cpu->bus, addr);

	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_zpgX_BCD(CPU *cpu)
// Add with Carry, X indexed zero page addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "ADC zpg,X";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3:  fetch byte
	byte M = read(*cpu->bus, addr);

	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_Xind_BCD(CPU *cpu)
// Add with Carry, X indexed zero page indirect addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "ADC X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5:  fetch byte
	byte M = read(*cpu->bus, addr);

	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_ADC_indY_BCD(CPU *cpu)
// Add with Carry, zero page indirect Y indexed addressing - BCD mode
// 	A, C = A + M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	word addr;			// Memory location
	byte M;				// Addend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 5;
	opr.mnemonic = "ADC ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do fetch on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}

	//		Store values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// 	Do the addition
	int AL = (old_A & 0x0F) + (M & 0x0F) + old_C;
	if (AL >= 0x0A)
	{
		AL = ((AL + 0x06) & 0x0F) + 0x10;
	}
	int new_A = (old_A & 0xF0) + (M & 0xF0) + AL;

	// Set N
	set_N(cpu, new_A);
	
	// Set V (doesn't work correctly)
	if ((new_A < -128) || (new_A > 127))
	{
		cpu->SR |= V;
	}
	else
	{
		cpu->SR &= ~V;
	}
	// then continue

	if (new_A >= 0xA0)
	{
		new_A = new_A + 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// Set the carry bit
	if (new_A >= 0x100)
	{
		cpu->SR |= C;
	}
	else
	{
		cpu->SR &= ~C;
	}

	// 	Redo the math in true binary and set Z
	new_A = old_A + M + old_C;
	set_Z(cpu, new_A);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_imm_BCD(CPU *cpu)
// Subtract with Carry, immediate addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 2;
	opr.mnemonic = "SBC # ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch byte and increment PC
	byte M = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = -1;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_abs_BCD(CPU *cpu)
// Subtract with Carry, absolute addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC abs";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of address, incement PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of address, increment PC
	addr = addr + (read(*cpu->bus, cpu->PC) << 8);
	cpu->PC++;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_absX_BCD(CPU *cpu)
// Subtract with Carry, x-indexed absolute addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	word addr;			// Memory location
	byte M;				// Subtrahend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC absX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->X;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_absY_BCD(CPU *cpu)
// Subtract with Carry, y-indexed absolute addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	word addr;			// Memory location
	byte M;				// Subtrahend from memory
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC absY";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch low byte of base address, incement PC
	// 	use two bytes for bal so we can catch the carry
	word bal = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2:  fetch high byte of base address, add X to low byte
	// 	increment PC
	byte bah = read(*cpu->bus, cpu->PC);
	bal = bal + cpu->Y;
	cpu->PC++;

	// Cycle 3:  if no carry on bal, fetch byte and move on 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and fetch byte on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4: fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_zpg_BCD(CPU *cpu)
// Subtract with Carry, zero page addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 3;
	opr.mnemonic = "SBC zpg";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address and increment PC
	word addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch byte
	byte M = read(*cpu->bus, addr);

	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_zpgX_BCD(CPU *cpu)
// Subtract with Carry, X indexed zero page addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 3;
	opr.cycles = 4;
	opr.mnemonic = "SBC xpgX";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte addr = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to address
	addr = addr + cpu->X;

	// Cycle 3: fetch byte
	byte M = read(*cpu->bus, addr);

	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_Xind_BCD(CPU *cpu)
// Subtract with Carry, X indexed zero page indirect addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 6;
	opr.mnemonic = "SBC X,ind";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: Add X to zpg address
	zad = zad + cpu->X;

	// Cycle 3: Fetch low byte of address, increment zpg address 
	byte adl = read(*cpu->bus, zad);
	zad = zad + 1;

	// Cycle 4: Fetch high byte of address
	byte adh = read(*cpu->bus, zad);
	word addr = (adh << 8) + adl;

	// Cycle 5: fetch byte
	byte M = read(*cpu->bus, addr);

	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn do_SBC_indY_BCD(CPU *cpu)
// Subtract with Carry, zero page indirect Y indexed addressing - BCD mode
// 	A, C = A + ~M + C
// 	Algorithm from http://www.6502.org/tutorials/decimal_mode.html#A
{
	word addr;			// Memory location
	byte M;				// Subtrahend from memory
	struct opreturn opr;

	opr.bytes = 2;
	opr.cycles = 4;
	opr.mnemonic = "SBC ind,Y";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: fetch zpg address, incement PC
	byte zad = read(*cpu->bus, cpu->PC);
	cpu->PC++;

	// Cycle 2: fetch low byte of base address
	word bal = read(*cpu->bus, zad);

	// Cycle 3: fetch high byte of base address, add Y to low byte
	zad = zad + 1;
	byte bah = read(*cpu->bus, zad);
	bal = bal + cpu->Y;

	// Cycle 4:  if no carry on bal, fetch byte and be done 
	if (bal < 256)
	{
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}	
	else //	otherwise, add carry to bah and do fetch on next cycle
	{
		bah = bah + 1;
		bal = bal & 0xFF;	// Trim off carry bit
		opr.cycles += 1;

		// Cycle 4:  fetch byte
		addr = (bah << 8) + bal;
		M = read(*cpu->bus, addr);
	}
	
	// 	Store old values for flag checks at end
	byte old_A = cpu->A;
	byte old_C = (cpu->SR & C)?1:0;

	// Do the subtraction:
	int AL = (old_A & 0x0F) - (M & 0x0F) + (old_C - 1);
	if (AL < 0)
	{
		AL = ((AL - 0x06) & 0x0F) - 0x10;
	}
	int new_A = (old_A & 0xF0) - (M & 0xF0) + AL;
	if (new_A < 0)
	{
		new_A = new_A - 0x60;
	}

	// 	Store in A
	cpu->A = new_A & 0xFF;

	// 	Redo the math in true binary and set C,N,V,Z
	//			The V flag isn't right.
	int result = old_A + (~M&0xFF) + old_C;
	//	(Trim ~M to 8 bits so the carry bit doesn't get lost 24 bits to the left)
	set_C(cpu, result);
	set_N(cpu, result);
	set_V(cpu, old_A, ~M, result);
	set_Z(cpu, result);

	opr.operand = addr;
	opr.result = cpu->A;

	return opr;
}

struct opreturn (*execute[])(CPU *cpu) =
{
do_BRK_impl, 	// 0x00
do_ORA_Xind, 	// 0x01
do_NOP_impl, 	// 0x02
do_NOP_impl, 	// 0x03
do_NOP_impl, 	// 0x04
do_ORA_zpg, 	// 0x05
do_ASL_zpg, 	// 0x06
do_NOP_impl, 	// 0x07
do_PHP_impl, 	// 0x08
do_ORA_imm, 	// 0x09
do_ASL_A,	 	// 0x0A
do_NOP_impl, 	// 0x0B
do_NOP_impl, 	// 0x0C
do_ORA_abs, 	// 0x0D
do_ASL_abs, 	// 0x0E
do_NOP_impl, 	// 0x0F
do_BPL_rel, 	// 0x10
do_ORA_indY, 	// 0x11
do_NOP_impl, 	// 0x12
do_NOP_impl, 	// 0x13
do_NOP_impl, 	// 0x14
do_ORA_zpgX, 	// 0x15
do_ASL_zpgX, 	// 0x16
do_NOP_impl, 	// 0x17
do_CLC_impl, 	// 0x18
do_ORA_absY, 	// 0x19
do_NOP_impl, 	// 0x1A
do_NOP_impl, 	// 0x1B
do_NOP_impl, 	// 0x1C
do_ORA_absX, 	// 0x1D
do_ASL_absX, 	// 0x1E
do_NOP_impl, 	// 0x1F
do_JSR_abs, 	// 0x20
do_AND_Xind, 	// 0x21
do_NOP_impl, 	// 0x22
do_NOP_impl, 	// 0x23
do_BIT_zpg, 	// 0x24
do_AND_zpg, 	// 0x25
do_ROL_zpg, 	// 0x26
do_NOP_impl, 	// 0x27
do_PLP_impl, 	// 0x28
do_AND_imm, 	// 0x29
do_ROL_A, 		// 0x2A
do_NOP_impl, 	// 0x2B
do_BIT_abs, 	// 0x2C
do_AND_abs, 	// 0x2D
do_ROL_abs, 	// 0x2E
do_NOP_impl, 	// 0x2F
do_BMI_rel, 	// 0x30
do_AND_indY, 	// 0x31
do_NOP_impl, 	// 0x32
do_NOP_impl, 	// 0x33
do_NOP_impl, 	// 0x34
do_AND_zpgX, 	// 0x35
do_ROL_zpgX, 	// 0x36
do_NOP_impl, 	// 0x37
do_SEC_impl, 	// 0x38
do_AND_absY, 	// 0x39
do_NOP_impl, 	// 0x3A
do_NOP_impl, 	// 0x3B
do_NOP_impl, 	// 0x3C
do_AND_absX, 	// 0x3D
do_ROL_absX, 	// 0x3E
do_NOP_impl, 	// 0x3F
do_RTI_impl, 	// 0x40
do_EOR_Xind, 	// 0x41
do_NOP_impl, 	// 0x42
do_NOP_impl, 	// 0x43
do_NOP_impl, 	// 0x44
do_EOR_zpg, 	// 0x45
do_LSR_zpg, 	// 0x46
do_NOP_impl, 	// 0x47
do_PHA_impl, 	// 0x48
do_EOR_imm, 	// 0x49
do_LSR_A, 		// 0x4A
do_NOP_impl, 	// 0x4B
do_JMP_abs, 	// 0x4C
do_EOR_abs, 	// 0x4D
do_LSR_abs, 	// 0x4E
do_NOP_impl, 	// 0x4F
do_BVC_rel, 	// 0x50
do_EOR_indY, 	// 0x51
do_NOP_impl, 	// 0x52
do_NOP_impl, 	// 0x53
do_NOP_impl, 	// 0x54
do_EOR_zpgX, 	// 0x55
do_LSR_zpgX, 	// 0x56
do_NOP_impl, 	// 0x57
do_CLI_impl, 	// 0x58
do_EOR_absY, 	// 0x59
do_NOP_impl, 	// 0x5A
do_NOP_impl, 	// 0x5B
do_NOP_impl, 	// 0x5C
do_EOR_absX, 	// 0x5D
do_LSR_absX, 	// 0x5E
do_NOP_impl, 	// 0x5F
do_RTS_impl, 	// 0x60
do_ADC_Xind, 	// 0x61
do_NOP_impl, 	// 0x62
do_NOP_impl, 	// 0x63
do_NOP_impl, 	// 0x64
do_ADC_zpg, 	// 0x65
do_ROR_zpg, 	// 0x66
do_NOP_impl, 	// 0x67
do_PLA_impl, 	// 0x68
do_ADC_imm, 	// 0x69
do_ROR_A, 		// 0x6A
do_NOP_impl, 	// 0x6B
do_JMP_ind, 	// 0x6C
do_ADC_abs, 	// 0x6D
do_ROR_abs, 	// 0x6E
do_NOP_impl, 	// 0x6F
do_BVS_rel, 	// 0x70
do_ADC_indY, 	// 0x71
do_NOP_impl, 	// 0x72
do_NOP_impl, 	// 0x73
do_NOP_impl, 	// 0x74
do_ADC_zpgX, 	// 0x75
do_ROR_zpgX, 	// 0x76
do_NOP_impl, 	// 0x77
do_SEI_impl, 	// 0x78
do_ADC_absY, 	// 0x79
do_NOP_impl, 	// 0x7A
do_NOP_impl, 	// 0x7B
do_NOP_impl, 	// 0x7C
do_ADC_absX, 	// 0x7D
do_ROR_absX, 	// 0x7E
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
do_TXS_impl, 	// 0x9A
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
do_CLV_impl, 	// 0xB8
do_LDA_absY, 	// 0xB9
do_TSX_impl, 	// 0xBA
do_NOP_impl, 	// 0xBB
do_LDY_absX, 	// 0xBC
do_LDA_absX, 	// 0xBD
do_LDX_absY, 	// 0xBE
do_NOP_impl, 	// 0xBF
do_CPY_imm, 	// 0xC0
do_CMP_Xind, 	// 0xC1
do_NOP_impl, 	// 0xC2
do_NOP_impl, 	// 0xC3
do_CPY_zpg, 	// 0xC4
do_CMP_zpg, 	// 0xC5
do_DEC_zpg, 	// 0xC6
do_NOP_impl, 	// 0xC7
do_INY_impl, 	// 0xC8
do_CMP_imm, 	// 0xC9
do_DEX_impl, 	// 0xCA
do_NOP_impl, 	// 0xCB
do_CPY_abs, 	// 0xCC
do_CMP_abs, 	// 0xCD
do_DEC_abs, 	// 0xCE
do_NOP_impl, 	// 0xCF
do_BNE_rel, 	// 0xD0
do_CMP_indY, 	// 0xD1
do_NOP_impl, 	// 0xD2
do_NOP_impl, 	// 0xD3
do_NOP_impl, 	// 0xD4
do_CMP_zpgX, 	// 0xD5
do_DEC_zpgX, 	// 0xD6
do_NOP_impl, 	// 0xD7
do_CLD_impl, 	// 0xD8
do_CMP_absY, 	// 0xD9
do_NOP_impl, 	// 0xDA
do_NOP_impl, 	// 0xDB
do_NOP_impl, 	// 0xDC
do_CMP_absX, 	// 0xDD
do_DEC_absX, 	// 0xDE
do_NOP_impl, 	// 0xDF
do_CPX_imm, 	// 0xE0
do_SBC_Xind, 	// 0xE1
do_NOP_impl, 	// 0xE2
do_NOP_impl, 	// 0xE3
do_CPX_zpg, 	// 0xE4
do_SBC_zpg, 	// 0xE5
do_INC_zpg, 	// 0xE6
do_NOP_impl, 	// 0xE7
do_INX_impl, 	// 0xE8
do_SBC_imm, 	// 0xE9
do_NOP_impl, 	// 0xEA
do_NOP_impl, 	// 0xEB
do_CPX_abs, 	// 0xEC
do_SBC_abs, 	// 0xED
do_INC_abs, 	// 0xEE
do_NOP_impl, 	// 0xEF
do_BEQ_rel, 	// 0xF0
do_SBC_indY, 	// 0xF1
do_NOP_impl, 	// 0xF2
do_NOP_impl, 	// 0xF3
do_NOP_impl, 	// 0xF4
do_SBC_zpgX, 	// 0xF5
do_INC_zpgX, 	// 0xF6
do_NOP_impl, 	// 0xF7
do_SED_impl, 	// 0xF8
do_SBC_absY, 	// 0xF9
do_NOP_impl, 	// 0xFA
do_NOP_impl, 	// 0xFB
do_NOP_impl, 	// 0xFC
do_SBC_absX, 	// 0xFD
do_INC_absX, 	// 0xFE
do_NOP_impl  	// 0xFF
};

struct opreturn do_CLD_impl(CPU *cpu)
// Clear Decimal flag
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "CLD   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Clear D
	cpu->SR &= ~D;

	// Swap arithmetic handlers
	execute[0x69] = do_ADC_imm;
	execute[0x6D] = do_ADC_abs;
	execute[0x7D] = do_ADC_absX;
	execute[0x79] = do_ADC_absY;
	execute[0x65] = do_ADC_zpg;
	execute[0x75] = do_ADC_zpgX;
	execute[0x71] = do_ADC_indY;
	execute[0xE9] = do_SBC_imm;
	execute[0xED] = do_SBC_abs;
	execute[0xFD] = do_SBC_absX;
	execute[0xF9] = do_SBC_absY;
	execute[0xE5] = do_SBC_zpg;
	execute[0xF5] = do_SBC_zpgX;
	execute[0xE1] = do_SBC_Xind;
	execute[0xF1] = do_SBC_indY;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}

struct opreturn do_SED_impl(CPU *cpu)
// Set Decimal flag
{
	struct opreturn opr;

	opr.bytes = 1;
	opr.cycles = 2;
	opr.mnemonic = "SED   ";

	// Cycle 0: instruction fetched, increment PC
	cpu->PC++;

	// Cycle 1: Set D
	cpu->SR |= D;

	// Swap arithmetic handlers
	execute[0x69] = do_ADC_imm_BCD;
	execute[0x6D] = do_ADC_abs_BCD;
	execute[0x7D] = do_ADC_absX_BCD;
	execute[0x79] = do_ADC_absY_BCD;
	execute[0x65] = do_ADC_zpg_BCD;
	execute[0x75] = do_ADC_zpgX_BCD;
	execute[0x61] = do_ADC_Xind_BCD;
	execute[0x71] = do_ADC_indY_BCD;
	execute[0xE9] = do_SBC_imm_BCD;
	execute[0xED] = do_SBC_abs_BCD;
	execute[0xFD] = do_SBC_absX_BCD;
	execute[0xF9] = do_SBC_absY_BCD;
	execute[0xE5] = do_SBC_zpg_BCD;
	execute[0xF5] = do_SBC_zpgX_BCD;
	execute[0xE1] = do_SBC_Xind_BCD;
	execute[0xF1] = do_SBC_indY_BCD;

	opr.operand = -1;
	opr.result = cpu->SR;

	return opr;
}
