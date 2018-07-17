/*
* Author: Jack Martin (15009787)
* Created: 19/10/2015
* Last Revision: 26/11/2015
* Description: Emulates the Intel 8080 processor
*/

#include "stdafx.h"
#include <winsock2.h>

#pragma comment(lib, "wsock32.lib")


#define STUDENT_NUMBER    "15009787"

#define IP_ADDRESS_SERVER "127.0.0.1"

#define PORT_SERVER 0x1984 // We define a port that we are going to use.
#define PORT_CLIENT 0x1985 // We define a port that we are going to use.

#define WORD  unsigned short
#define DWORD unsigned long
#define BYTE  unsigned char

#define MAX_FILENAME_SIZE 500
#define MAX_BUFFER_SIZE   500

SOCKADDR_IN server_addr;
SOCKADDR_IN client_addr;

SOCKET sock;  // This is our socket, it is the handle to the IO address to read/write packets

WSADATA data;

char InputBuffer [MAX_BUFFER_SIZE];

char hex_file [MAX_BUFFER_SIZE];
char trc_file [MAX_BUFFER_SIZE];

//////////////////////////
//   Registers          //
//////////////////////////

#define FLAG_I  0x40
#define FLAG_Z  0x20
#define FLAG_P  0x10
#define FLAG_V  0x08
#define FLAG_N  0x02
#define FLAG_C  0x01
#define REGISTER_A	3
#define REGISTER_B	2
#define REGISTER_D	1
#define REGISTER_C	0
WORD BaseRegister;
BYTE PageRegister;

BYTE Registers[4];
BYTE Flags;
WORD ProgramCounter;
WORD StackPointer;


////////////
// Memory //
////////////

#define MEMORY_SIZE	65536

BYTE Memory[MEMORY_SIZE];

#define TEST_ADDRESS_1  0x01FA
#define TEST_ADDRESS_2  0x01FB
#define TEST_ADDRESS_3  0x01FC
#define TEST_ADDRESS_4  0x01FD
#define TEST_ADDRESS_5  0x01FE
#define TEST_ADDRESS_6  0x01FF
#define TEST_ADDRESS_7  0x0200
#define TEST_ADDRESS_8  0x0201
#define TEST_ADDRESS_9  0x0202
#define TEST_ADDRESS_10  0x0203
#define TEST_ADDRESS_11  0x0204
#define TEST_ADDRESS_12  0x0205


///////////////////////
// Control variables //
///////////////////////

bool memory_in_range = true;
bool halt = false;


///////////////////////
// Disassembly table //
///////////////////////

char opcode_mneumonics[][14] =
{
"NOP impl     ", 
"LODS  #      ", 
"LODS abs     ", 
"LODS zpg     ", 
"LODS (ind)   ", 
"LODS pag     ", 
"LODS bas     ", 
"ILLEGAL     ", 
"DEP impl     ", 
"ILLEGAL     ", 
"LDAA  #      ", 
"LDAA abs     ", 
"LDAA zpg     ", 
"LDAA (ind)   ", 
"LDAA pag     ", 
"LDAA bas     ", 

"HALT impl    ", 
"LDZ  #       ", 
"LDZ abs      ", 
"LDZ zpg      ", 
"LDZ (ind)    ", 
"LDZ pag      ", 
"LDZ bas      ", 
"ILLEGAL     ", 
"INP impl     ", 
"ILLEGAL     ", 
"LDAB  #      ", 
"LDAB abs     ", 
"LDAB zpg     ", 
"LDAB (ind)   ", 
"LDAB pag     ", 
"LDAB bas     ", 

"ILLEGAL     ", 
"CAS impl     ", 
"CLC impl     ", 
"STS abs      ", 
"STS zpg      ", 
"STS (ind)    ", 
"STS pag      ", 
"STS bas      ", 
"DEZ impl     ", 
"JPA abs      ", 
"JPA zpg      ", 
"JPA (ind)    ", 
"JPA pag      ", 
"INC abs      ", 
"INCA A,A     ", 
"INCB B,B     ", 

"ADIA  #      ", 
"TSA impl     ", 
"SEC impl     ", 
"STZ abs      ", 
"STZ zpg      ", 
"STZ (ind)    ", 
"STZ pag      ", 
"STZ bas      ", 
"INZ impl     ", 
"JCC abs      ", 
"JCC zpg      ", 
"JCC (ind)    ", 
"JCC pag      ", 
"DEC abs      ", 
"DECA A,A     ", 
"DECB B,B     ", 

"ADIB  #      ", 
"ABA impl     ", 
"CLI impl     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"JPR abs      ", 
"JPR zpg      ", 
"JPR (ind)    ", 
"JPR pag      ", 
"JCS abs      ", 
"JCS zpg      ", 
"JCS (ind)    ", 
"JCS pag      ", 
"RRC abs      ", 
"RRCA A,A     ", 
"RRCB B,B     ", 

"SBIA  #      ", 
"SBA impl     ", 
"STI impl     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ADC A,C      ", 
"ADC A,D      ", 
"ADC B,C      ", 
"ADC B,D      ", 
"JNE abs      ", 
"JNE zpg      ", 
"JNE (ind)    ", 
"JNE pag      ", 
"RL abs       ", 
"RLA A,A      ", 
"RLB B,B      ", 

"SBIB  #      ", 
"AAB impl     ", 
"STV impl     ", 
"ILLEGAL     ", 
"RET impl     ", 
"SBC A,C      ", 
"SBC A,D      ", 
"SBC B,C      ", 
"SBC B,D      ", 
"JEQ abs      ", 
"JEQ zpg      ", 
"JEQ (ind)    ", 
"JEQ pag      ", 
"SHL abs      ", 
"SHLA A,A     ", 
"SHLB B,B     ", 

"CPIA  #      ", 
"SAB impl     ", 
"CLV impl     ", 
"SWI impl     ", 
"ILLEGAL     ", 
"ADD A,C      ", 
"ADD A,D      ", 
"ADD B,C      ", 
"ADD B,D      ", 
"JVC abs      ", 
"JVC zpg      ", 
"JVC (ind)    ", 
"JVC pag      ", 
"ASR abs      ", 
"ASRA A,A     ", 
"ASRB B,B     ", 

"CPIB  #      ", 
"TAP impl     ", 
"CMC impl     ", 
"RTI impl     ", 
"ILLEGAL     ", 
"SUB A,C      ", 
"SUB A,D      ", 
"SUB B,C      ", 
"SUB B,D      ", 
"JVS abs      ", 
"JVS zpg      ", 
"JVS (ind)    ", 
"JVS pag      ", 
"SHR abs      ", 
"SHRA A,A     ", 
"SHRB B,B     ", 

"ORIA  #      ", 
"TPA impl     ", 
"CMV impl     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"CMP A,C      ", 
"CMP A,D      ", 
"CMP B,C      ", 
"CMP B,D      ", 
"JMI abs      ", 
"JMI zpg      ", 
"JMI (ind)    ", 
"JMI pag      ", 
"NOT abs      ", 
"NOTA A,A     ", 
"NOTB B,B     ", 

"ORIB  #      ", 
"MV A,A       ", 
"MV A,B       ", 
"MV A,C       ", 
"MV A,D       ", 
"ORA A,C      ", 
"ORA A,D      ", 
"ORA B,C      ", 
"ORA B,D      ", 
"JPL abs      ", 
"JPL zpg      ", 
"JPL (ind)    ", 
"JPL pag      ", 
"NEG abs      ", 
"NEGA A,0     ", 
"NEGB B,0     ", 

"ANIA  #      ", 
"MV B,A       ", 
"MV B,B       ", 
"MV B,C       ", 
"MV B,D       ", 
"AND A,C      ", 
"AND A,D      ", 
"AND B,C      ", 
"AND B,D      ", 
"JPE abs      ", 
"JPE zpg      ", 
"JPE (ind)    ", 
"JPE pag      ", 
"ROL abs      ", 
"ROLA A,A     ", 
"ROLB B,B     ", 

"ANIB  #      ", 
"MV C,A       ", 
"MV C,B       ", 
"MV C,C       ", 
"MV C,D       ", 
"EOR A,C      ", 
"EOR A,D      ", 
"EOR B,C      ", 
"EOR B,D      ", 
"JPO abs      ", 
"JPO zpg      ", 
"JPO (ind)    ", 
"JPO pag      ", 
"RR abs       ", 
"RRA A,A      ", 
"RRB B,B      ", 

"ILLEGAL     ", 
"MV D,A       ", 
"MV D,B       ", 
"MV D,C       ", 
"MV D,D       ", 
"BT A,C       ", 
"BT A,D       ", 
"BT B,C       ", 
"BT B,D       ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"CLR abs      ", 
"CLRA A,0     ", 
"CLRB B,0     ", 

"LD  #,C      ", 
"LD abs,C     ", 
"LD zpg,C     ", 
"LD (ind),C   ", 
"LD pag,C     ", 
"LD bas,C     ", 
"STA abs      ", 
"STA zpg      ", 
"STA (ind)    ", 
"STA pag      ", 
"STA bas      ", 
"PUSH  ,A     ", 
"PUSH  ,B     ", 
"PUSH  ,s     ", 
"PUSH  ,C     ", 
"PUSH  ,D     ", 

"LD  #,D      ", 
"LD abs,D     ", 
"LD zpg,D     ", 
"LD (ind),D   ", 
"LD pag,D     ", 
"LD bas,D     ", 
"STB abs      ", 
"STB zpg      ", 
"STB (ind)    ", 
"STB pag      ", 
"STB bas      ", 
"POP A,       ", 
"POP B,       ", 
"POP s,       ", 
"POP C,       ", 
"POP D,       ", 

}; 

////////////////////////////////////////////////////////////////////////////////
//                           Simulator/Emulator (Start)                       //
////////////////////////////////////////////////////////////////////////////////
BYTE fetch() {
	BYTE byte = 0;
	if ((ProgramCounter >= 0) && (ProgramCounter <= MEMORY_SIZE)) {
		memory_in_range = true;
		byte = Memory[ProgramCounter];
		ProgramCounter++;
	}
	else {
		memory_in_range = false;
	}
	return byte;
}

void set_flag_z(BYTE inReg) {
	BYTE data; 
	data = inReg;
	if (data == 0x00){ 
		Flags = Flags | FLAG_Z;
	}
	else { 
		Flags = Flags & (~FLAG_Z);
	}
}

void set_flag_n(BYTE inReg) {
	BYTE data;
	data = inReg;
	if ((data & 0x80) == 0x80){ 
		Flags = Flags | FLAG_N;
	}
	else { 
		Flags = Flags & (~FLAG_N);
	}
}

void set_flag_v(BYTE in1, BYTE in2, BYTE out1) {
	BYTE reg1in;
	BYTE reg2in;
	BYTE regOut;
	reg1in = in1;
	reg2in = in2;
	regOut = out1;
	if ((((reg1in & 0x80) == 0x80) && ((reg2in & 0x80) == 0x80) && 
	(((BYTE)regOut & 0x80) != 0x80)) || (((reg1in & 0x80) != 0x80) && 
	((reg2in & 0x80) != 0x80) && (((BYTE)regOut & 0x80) == 0x80))) { 
		Flags = Flags | FLAG_V;
	}
	else {
		Flags = Flags & (~FLAG_V);
	}
}

void set_flag_p(BYTE inReg) {
	BYTE reg; 
	reg = inReg;
	if ((reg & 0x01) != 0) {
		Flags = Flags | FLAG_P;
	}
	else {
		Flags = Flags & (0xFF - FLAG_P);
	}
}

void Group_1(BYTE opcode) {

	BYTE LB = 0;
	BYTE HB = 0;
	WORD address = 0;
	WORD data = 0;
	WORD data_16 = 0;
	WORD temp_word = 0;

	switch(opcode) {

		case 0x00: //NOP impl
			// No operation
			halt = true;
			break;

		case 0x01: //LODS \#
			// Contents of memory are copied directly into stack pointer register
			HB = fetch();
			LB = fetch();
			data_16 += (WORD)((WORD)HB << 8) + LB;
			StackPointer = data_16;
			break;

		case 0x02: //LODS abs
			// Contents of memory are copied into stack pointer register using absolute addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			StackPointer = data_16;
			break;

		case 0x03: //LODS zpg
			// Contents of memoery are copied into stack pointer using zero page addressing
			address = 0x0000 | (WORD)fetch();
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			StackPointer = data_16;
			break;

		case 0x04: //LODS ind
			// Contents of memory are copied into stack pointer register using indirect addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			StackPointer = data_16;
			break;

		case 0x05: //LODS pag
			// Contents of memory are copied into stack pointer register using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			StackPointer = data_16;
			break;

		case 0x06: //LODS bas
			// Contents of memory are copied into stack pointer register using base offset addressing
			if((LB = fetch()) >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else {
				address = (BaseRegister + LB);
			}
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			StackPointer = data_16;
			break;

		case 0x08: // DEP
			// Contents of page register (P register) are decremented
			PageRegister--;
			set_flag_z(PageRegister);
			break;

		case 0x0A: //LDAA \#
			// Contents of memory are copied directly into register A (accumulator)
			data = fetch();
			Registers[REGISTER_A] = data;
			break;

		case 0x0B: //LDAA abs
			// Contents of memory are copied into register A (accumulator) using absolute addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_A] = Memory[address];
			}
			break;

		case 0x0C: //LDAA zpg
			// Contents of memory are copied into register A (accumulator) using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_A] = Memory[address];
			}
			break;

		case 0x0D: //LDAA ind
			// Contents of memory are copied into register A (accumulator) using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_A] = Memory[address];
			}
			break;

		case 0x0E: //LDAA pag
			// Contents of memory are copied into register A (accumulator) using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_A] = Memory[address];
			}
			break;

		case 0x0F: //LDAA bas
			// Contents of memory are copied into register A (accumulator) using base offset addressing
			if ((LB = fetch()) >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_A] = Memory[address];
			}
			break;

		case 0x10: //HALT impl
			// Waits for interrupt
			halt = true;
			break;

		case 0x11: //LDZ \#
			// Contents of memory are copied directly into zero page flag (Z flag)
			HB = fetch();
			LB = fetch();
			data_16 = (WORD)((WORD)HB << 8) + LB;
			BaseRegister = data_16;
			break;

		case 0x12: //LDZ abs
			// Contents of memory are copied into zero page flag (Z flag) using absolute addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			data_16 = (WORD)Memory[address] << 8;
			data_16 = (WORD)Memory[address + 1];
			BaseRegister = data_16;
			break;

		case 0x13: //LDZ zpg
			// Contents of memory are copied into zero page flag (Z flag) using zero page addressing
			address = 0x0000 | (WORD)fetch();
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			BaseRegister = data_16;
			break;

		case 0x14: //LDZ ind
			// Contents of memory are copied into zero page flag (Z flag) using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			BaseRegister = data_16;
			break;

		case 0x15: //LDZ pag
			// Contents of memory are copied into zero page flag (Z flag) using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			BaseRegister = data_16;
			break;

		case 0x16: //LDZ bas
			// Contents of memory are copied into zero page flag (Z flag) using base offset addressing
			if((LB = fetch()) >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else {
				address = (BaseRegister + LB);
			}
			data_16 += (WORD)Memory[address] << 8;
			data_16 += (WORD)Memory[address + 1];
			PageRegister = data_16;
			break;

		case 0x18: // INCP
			// Contents of page register (P register) are incremented
			++PageRegister;
			set_flag_z(PageRegister);
			break;

		case 0x1A: //LDAB \#
			// Contents of memory are copied directly into register B (accumulator)
			data = fetch();
			Registers[REGISTER_B] = data;
			break;

		case 0x1B: //LDAB abs
			// Contents of memory are copied into register B (accumulator) using absolute addressing
			HB = fetch();
			LB = fetch();
				address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_B] = Memory[address];
			}
			break;

		case 0x1C: //LDAB zpg
			// Contents of memory are copied into register B (accumulator) using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_B] = Memory[address];
			}
			break;

		case 0x1D: //LDAB ind
			// Contents of memory are copied into register B (accumulator) using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_B] = Memory[address];
			}
			break;

		case 0x1E: //LDAB pag
			// Contents of memory are copied into register B (accumulator) using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_B] = Memory[address];
			}
			break;

		case 0x1F: //LDAB bas
			// Contents of memory are copied into register B (accumulator) using base offset addressing
			if ((LB = fetch()) >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_B] = Memory[address];
			}
			break;
			
		case 0x21: //CAS
			// Contents of register A (accumulator) are copied into status register
			 Flags = Registers[REGISTER_A];
			 break;

		case 0x22: //CLC
			// Contents of carry flag are cleared
			 Flags = Flags & (0xFF - FLAG_C);
			 break;

		case 0x23: //STS abs
			 // Contents of stack pointer are copied into memory using absolute addressing
			 HB = fetch();
			 LB = fetch();
			 address = (WORD)((WORD)HB << 8) + LB;
			 if(address >= 0 && address < MEMORY_SIZE) {
				 Memory[address] = StackPointer >> 8;
				 Memory[address + 1] = StackPointer & 0xFF;
			 }
			 break;

		case 0x24: //STS zpg
			// Contents of stack pointer are copied into memory using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = StackPointer >> 8;
				Memory[address + 1] = StackPointer & 0xFF;
			}
			break;

		case 0x25: //STS ind
			// Contents of stack pointer are copied into memory using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = StackPointer >> 8;
				Memory[address + 1] = StackPointer & 0xFF;
			}
			break;

		case 0x26: //STS pag
			// Contents of stack pointer are copied into memory using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = StackPointer >> 8;
				Memory[address + 1] = StackPointer & 0xFF;
			}
			break;

		case 0x27: //STS bas
			// Contents of stack pointer are copied into memory using base offset addressing
			if ((LB = fetch())  >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = StackPointer >> 8;
				Memory[address + 1] = StackPointer & 0xFF;
			}
			break;

		case 0x28: // DEZ
			// Contents of zero flag (Z flag) are decremented
			BaseRegister--;
			set_flag_z(BaseRegister);
			break;
		
		case 0x29: //JPA abs
			// Content of memory are copied into program counter using absolute addressing
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			ProgramCounter = address;
			break;

		case 0x2A: //JPA zpg
			// Contents of memory are copied into program counter using zero page addressing
			address = 0x0000 | (WORD)fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			ProgramCounter = address;
			break;

		case 0x2B: //JPA ind
			// Contents of memory are copied into program counter using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			ProgramCounter = address;
			break;

		case 0x2C: //JPA pag
			// Contents of memory are copied into program counter using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			ProgramCounter = address;
			break;
		
		case 0x2D: //INC
			// Contents of page register (register P) are incremented
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			++Memory[address];
			set_flag_p(Memory[address]);
			set_flag_n(Memory[address]);
			set_flag_z(Memory[address]);
			break;

		case 0x2E: //INCA
			// Contents of register A (accumulator) are incremented
			++Registers[REGISTER_A];
			set_flag_p(Registers[REGISTER_A]);
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0x2F: //INCB
			// Contents of register B (accumulator) are incremented
			++Registers[REGISTER_B];
			set_flag_p(Registers[REGISTER_B]);
			set_flag_n(Registers[REGISTER_B]);
			set_flag_z(Registers[REGISTER_B]);
			break;

		case 0x30: //ADIA
			// Data added to register A (accumulator) and carry flag is set if appropriate
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_A] + data;
			if ((Flags & FLAG_C) != 0) {
				temp_word++;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], data, (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x31: //TSA impl
			// Contents of status register are copied into register A (accumulator)
			Registers[REGISTER_A] = Flags;
			break;

		case 0x32: //SEC impl
			// Carry flag is set
			Flags |= FLAG_C;
			break;

		case 0x33: //STZ abs
			 // Contents of base register (register Z) are copied into memory using absolute addressing
			 HB = fetch();
			 LB = fetch();
			 address = (WORD)((WORD)HB << 8) + LB;
			 if(address >= 0 && address < MEMORY_SIZE) {
				 Memory[address] = BaseRegister;
			 }
			 break;

		case 0x34: //STZ zpg
			// Contents of base register (register Z) are copied into memory using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = BaseRegister;
			}
			break;

		case 0x35: //STZ ind
			// Contents of base register (register Z) are copied into memory using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = BaseRegister;
			}
			break;

		case 0x36: //STZ pag
			// Contents of base register (register Z) are copied into memory using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = BaseRegister;
			}
			break;

		case 0x37: //STZ bas
			// Contents of base register (register Z) are copied into memory using base offset addressing
			if ((LB = fetch())  >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = BaseRegister;
			}
			break;

		case 0x38: // INZ
			// Contents of base register (register Z) are incremented
			BaseRegister++;
			set_flag_z(BaseRegister);
			break;

		case 0x39: //JCC abs
			// Jumps to subroutine using absolute addressing when carry flag is cleared
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_C) != FLAG_C) {
				ProgramCounter = address;
			}
			break;
			
		case 0x3A: //JCC zpg
			// Jumps to subroutine using zero page addressing when carry flag is cleared
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_C) != FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x3B: //JCC ind
			// Jumps to subroutine using indexed addressing when carry flag is cleared
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if ((Flags & FLAG_C) != FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x3C: //JCC pag
			// Jumps to subroutine using paged addressing when carry flag is cleared
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if ((Flags & FLAG_C) != FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x3D: //DEC
			// Contents of memory are decremented
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			--Memory[address];
			set_flag_p(Memory[address]);
			set_flag_n(Memory[address]);
			set_flag_z(Memory[address]);
			break;

		case 0x3E: //DECA
			// Contents of register A (accumulator) are decremented
			--Registers[REGISTER_A];
			set_flag_p(Registers[REGISTER_A]);
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0x3F: //DECB
			// Contents of register B (accumulator) are decremented
			--Registers[REGISTER_B];
			set_flag_p(Registers[REGISTER_B]);
			set_flag_n(Registers[REGISTER_B]);
			set_flag_z(Registers[REGISTER_B]);
			break;

		case 0x40: //ADIB
			// Data added to contents of register B (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_B] + data;
			if ((Flags & FLAG_C) != 0) {
				temp_word++;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], data, (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x41: //ABA impl
			// Contents of register B added to contents of register A
			temp_word = Registers[REGISTER_A] + Registers[REGISTER_B];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_B], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x42: //CLI impl
			// Contents of interrupt flag (flag I) are cleared
			Flags = Flags & (0xFF - FLAG_I);
			break;

		case 0x45: //JPR abs
			// Jumps to subroutine using absolute addressing
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((StackPointer >= 2) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = (BYTE)(ProgramCounter & 0xFF);
				--StackPointer;
				Memory[StackPointer] = (BYTE)((ProgramCounter >> 8) & 0xFF);
				ProgramCounter = address;
			}
			break;

		case 0x46: //JPR zpg
			// Jumps to subroutine using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if ((StackPointer >= 2) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = (BYTE)(ProgramCounter & 0xFF);
				--StackPointer;
				Memory[StackPointer] = (BYTE)((ProgramCounter >> 8) & 0xFF);
				ProgramCounter = address;
			}
			break;

		case 0x47: //JPR ind
			// Jumps to subroutine using indexed addressing
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((StackPointer >= 2) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = (BYTE)(ProgramCounter & 0xFF);
				--StackPointer;
				Memory[StackPointer] = (BYTE)((ProgramCounter >> 8) & 0xFF);
				ProgramCounter = address;
			}
			break;

		case 0x48: //JPR pag
			// Jumps to subroutine using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((StackPointer >= 2) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = (BYTE)(ProgramCounter & 0xFF);
				--StackPointer;
				Memory[StackPointer] = (BYTE)((ProgramCounter >> 8) & 0xFF);
				ProgramCounter = address;
			}
			break;

		case 0x49: //JCS abs
			// Jumps to subroutine using absolute addressing when carry flag is set
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_C) == FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x4A: //JCS zpg
			// Jumps to subroutine using zero page addressing when carry flag is set
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_C) == FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x4B: //JCS ind
			// Jumps to subroutine using indexed addressing when carry flag is set
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if ((Flags & FLAG_C) == FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x4C: //JCS pag
			// Jumps to subroutine using paged addressing when carry flag is set
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if ((Flags & FLAG_C) == FLAG_C) {
				ProgramCounter = address;
			}
			break;

		case 0x4D: //RRC
			// Rotates data right through carry and memory
            HB = fetch();
            LB = fetch();
            address = ((WORD)HB << 8) + (WORD)LB;
            temp_word = Memory[address];
            if (address >= 0 && address < MEMORY_SIZE) {
				if ((Flags & FLAG_C) == FLAG_C) {
					if ((temp_word & 0x01) == 0x01) {
						temp_word = (temp_word >> 1) + 0x80;
					}
                    else {
						temp_word = (temp_word >> 1) + 0x80;
						Flags &= (0xFF - FLAG_C);
					}
				}
                else {
					if ((temp_word & 0x01) == 0x01) {
						temp_word >>= 1;
                        Flags |= FLAG_C;
                    }
                    else {
						temp_word >>= 1;
					}
                }
			}
            set_flag_n((BYTE)temp_word);
            set_flag_p((BYTE)temp_word);
            set_flag_z((BYTE)temp_word);
            Memory[address] = (BYTE)temp_word;
            break;

		case 0x4E: // RRCA
			// Rotates data right through carry and register A (accumulator)
			temp_word = Registers[REGISTER_A];
			if ((Flags & FLAG_C) == FLAG_C) {
				if ((temp_word & 0x01) == 0x01) {
					temp_word = (temp_word >> 1) + 0x80;
				}
				else {
					temp_word = (temp_word >> 1) + 0x80;
					Flags &= (0xFF - FLAG_C);
				}
			}
			else {
				if ((temp_word & 0x01) == 0x01) {
					temp_word >>= 1;
					Flags |= FLAG_C;
				}
				else temp_word >>= 1;
			}

			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x4F: // RRCB
			// Rotates data right through carry and register B (accumulator)
			temp_word = Registers[REGISTER_B];
			if ((Flags & FLAG_C) == FLAG_C) {
				if ((temp_word & 0x01) == 0x01) {
					temp_word = (temp_word >> 1) + 0x80;
				}
				else {
					temp_word = (temp_word >> 1) + 0x80;
					Flags &= (0xFF - FLAG_C);
				}
			}
			else {
				if ((temp_word & 0x01) == 0x01) {
					temp_word >>= 1;
					Flags |= FLAG_C;
				}
				else temp_word >>= 1;
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x50: //SBIA
			// Data subtracted from register A (accumulator) with carry
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_A] - data;
			if ((Flags & FLAG_C) != 0) {
				temp_word--;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -data, (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;
			
		case 0x51: //SBA impl
			// Contents of register B subtracted from contents of register A
			temp_word = Registers[REGISTER_A] - Registers[REGISTER_B];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_B], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x52: //STI impl
			// Sets interrupt flag (flag I)
			Flags = Flags | FLAG_I;
			break;

		case 0x55: //ADC A,C
			// Contents of register C added to contents of register A with carry
			temp_word = (WORD)Registers[REGISTER_A] + (WORD)Registers[REGISTER_C];
			if ((Flags & FLAG_C) != 0) {
				temp_word++;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x56: //ADC A,D
			// Contents of register D added to contents of register A with carry
			temp_word = (WORD)Registers[REGISTER_A] + (WORD)Registers[REGISTER_D];
			if ((Flags & FLAG_C) != 0) {
				temp_word++;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x57: //ADC B,C
			// Contents of register C added to contents of register B
			temp_word = (WORD)Registers[REGISTER_B] + (WORD)Registers[REGISTER_C];
			if ((Flags & FLAG_C) != 0) {
				temp_word++;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x58: //ADC B,D
			// Contents of register D added to contents of register B
			temp_word = (WORD)Registers[REGISTER_B] + (WORD)Registers[REGISTER_D];
			if ((Flags & FLAG_C) != 0) {
				temp_word++;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x59: //JNE abs
			// Jumps to subroutine using absolute addressing when result isn't zero
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_Z) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x5A: //JNE zpg
			// Jumps to subroutine using zero page addressing when result isn't zero
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_Z) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x5B: //JNE ind
			// Jumps to subroutine using indexed addressing when result isn't zero
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_Z) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x5C: //JNE pag
			// Jumps to subroutine using paged addressing when result isn't zero
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_Z) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x5D:// RL
			// Rotate left through carry and memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			temp_word = Memory[address];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((Flags & FLAG_C) == FLAG_C) {
					if ((temp_word & 0x80) == 0x80) {
						temp_word = (temp_word << 1) + 0x01;
					}
					else {
						temp_word = (temp_word << 1) + 0x01;
						Flags &= (0xFF - FLAG_C);
					}
				}
				else {
					if ((temp_word & 0x80) == 0x80) {
						temp_word = temp_word << 1;
						Flags |= FLAG_C;
					}
					else {
						temp_word = temp_word << 1;
					}
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x5E:// RLA
			// Rotates left through carry and register A (accumulator)
			temp_word = Registers[REGISTER_A];
			if ((Flags & FLAG_C) == FLAG_C) {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = (temp_word << 1) + 0x01;
				}
				else {
					temp_word = (temp_word << 1) + 0x01;
					Flags &= (0xFF - FLAG_C);
				}
			}
			else {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = temp_word << 1;
					Flags |= FLAG_C;
				}
				else {
					temp_word = temp_word << 1;
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x5F:// RLB
			// Rotates left through carry and register B (accumulator)
			temp_word = Registers[REGISTER_B];
			if ((Flags & FLAG_C) == FLAG_C) {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = (temp_word << 1) + 0x01;
				}
				else {
					temp_word = (temp_word << 1) + 0x01;
					Flags &= (0xFF - FLAG_C);
				}
			}
			else {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = temp_word << 1;
					Flags |= FLAG_C;
				}
				else {
					temp_word = temp_word << 1;
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x60: //SBIB
			// Data subtracted from register B (accumulator) with carry
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_B] - data;
			if ((Flags & FLAG_C) != 0) {
				temp_word--;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -data, (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x61: //AAB impl
			// Adds contents of register B to contents of register A
			temp_word = Registers[REGISTER_A] + Registers[REGISTER_B];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_B], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x62: //STV impl
			// Sets overflow flag (flag V)
			Flags = Flags | FLAG_V;
			break;

		case 0x64: //RET impl
			// Returns from subroutine
			if ((StackPointer >= 0) && (StackPointer < MEMORY_SIZE - 2)) {
				HB = Memory[StackPointer];
				StackPointer ++;
				LB = Memory[StackPointer];
				StackPointer ++;
				ProgramCounter = ((WORD)HB << 8) + (WORD)LB;
			}
			break;

		case 0x65: //SBC A,C
			// Subtracts contents of register C from contents of register A
			temp_word = (WORD)Registers[REGISTER_A] - (WORD)Registers[REGISTER_C];
			if ((Flags & FLAG_C) != 0) {
				temp_word--;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x66: //SBC A,D
			// Sbutracts contents of register D from contents of register A
			temp_word = (WORD)Registers[REGISTER_A] - (WORD)Registers[REGISTER_D];
			if ((Flags & FLAG_C) != 0) {
				temp_word--;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x67: //SBC B,C
			// Subtracts contents of register C from contents of register B
			temp_word = (WORD)Registers[REGISTER_B] - (WORD)Registers[REGISTER_C];
			if ((Flags & FLAG_C) != 0) {
				temp_word--;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x68: //SBC B,D
			// Subtracts contents of register D from contents of register B
			temp_word = (WORD)Registers[REGISTER_B] - (WORD)Registers[REGISTER_D];
			if ((Flags & FLAG_C) != 0) {
				temp_word--;
			}
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x69: //JEQ abs
			// Jumps to subroutine using absolute addressing when result is equal to zero
			HB = fetch();
			LB = fetch();
			address	= ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_Z) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x6A: //JEQ zpg
			// Jumps to subroutine using zero page addressing when result is equal to zero
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_Z) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x6B: //JEQ ind
			// Jumps to subroutine using indexed addressing when result is equal to zero
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_Z) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x6C: //JEQ pag
			// Jumps to subroutine using paged addressing when result is equal to zero
			HB = PageRegister;
			LB = fetch();
			address += ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_Z) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x6D: //SHL
			// Arithmetic shift left in memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			temp_word = Memory[address];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x80) != 0) Flags |= FLAG_C;
				else Flags &= (~FLAG_C);
				temp_word = temp_word << 1;
				temp_word = temp_word & 0xFE;
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x6E: // SHLA
			// Arithmetic shift left in register A (accumulator)
			temp_word = Registers[REGISTER_A];				
			if ((temp_word & 0x80) != 0) Flags |= FLAG_C;	
			else Flags &= (0xFF - FLAG_C);					
			temp_word <<= 1;								
			temp_word &= 0xFE;								
			set_flag_z((BYTE)temp_word);					
			set_flag_p((BYTE)temp_word);					
			set_flag_n((BYTE)temp_word);					
			Registers[REGISTER_A] = (BYTE)temp_word;		
			break;

		case 0x6F: // SHLB
			// Arithmetic shift left in register B (accumulator)
			temp_word = Registers[REGISTER_B];				
			if ((temp_word & 0x80) != 0) Flags |= FLAG_C;	
			else Flags &= (0xFF - FLAG_C);					
			temp_word <<= 1;								
			temp_word &= 0xFE;								
			set_flag_z((BYTE)temp_word);					
			set_flag_p((BYTE)temp_word);					
			set_flag_n((BYTE)temp_word);					
			Registers[REGISTER_B] = (BYTE)temp_word;		
			break;

		case 0x70: //CPIA
			// Data compared to register A (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_A] - data; 
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -data, (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;
		
		case 0x71: //SAB impl
			// Subtracts contents of register A from contents of register B
			temp_word = Registers[REGISTER_B] - Registers[REGISTER_A];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_B], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x72: //CLV impl
			// Clear contents of overflow flag (flag V)
			Flags = Flags & (0xFF - FLAG_V);
			break;

		case 0x75: //ADD A,C
			// Adds contents of register C to contents of register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] + (WORD)Registers[REGISTER_C];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x76: //ADD A,D
			// Adds contents of register D to contents of register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] + (WORD)Registers[REGISTER_D];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x77: //ADD B,C
			// Adds contents of register C to contents of register B
			temp_word = (WORD)Registers[REGISTER_B] + (WORD)Registers[REGISTER_C];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x78: //ADD B,D
			// Adds contents of register D to contents of register B
			temp_word = (WORD)Registers[REGISTER_B] + (WORD)Registers[REGISTER_D];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x79: //JVC abs
			// Jumps to subroutine using absolute addressing when overflow flag (flag V) is cleared 
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_V) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x7A: //JVC zpg
			// Jumps to subroutine using zero page addressing when overflow flag (flag V) is cleared
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_V) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x7B: //JVC ind
			// Jumps to subroutine using indexed addressing when overflow flag (flag V) is cleared
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_V) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x7C: //JVC pag
			// Jumps to subroutine using paged addressing when overflow flag (flag V) is cleared
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_V) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0x7D: // ASR abs
			// Arithmetic shift to the right in memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			temp_word = Memory[address];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x01) != 0) {
					Flags |= FLAG_C;
				}
				else {
					Flags &= (0xFF - FLAG_C);
				}
				temp_word >>= 1;
				if ((temp_word & 0x40) != 0) {
					temp_word += 0x80;
				}
			}
			set_flag_z(temp_word);
			set_flag_p(temp_word);
			set_flag_n(temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x7E: // ASRA					
			// Arithmetic shift to the right in register A (accumulator)
			temp_word = Registers[REGISTER_A];
			if ((temp_word & 0x01) != 0) {
				Flags |= FLAG_C;
			}
			else {
				Flags &= (0xFF - FLAG_C);
			}
			temp_word >>= 1;
			if ((temp_word & 0x40) != 0) {
				temp_word += 0x80;
			}
			set_flag_z(temp_word);
			set_flag_p(temp_word);
			set_flag_n(temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x7F: // ASRB	
			// Arithmetic shift to the right in register B (accumulator)
			temp_word = Registers[REGISTER_B];
			if ((temp_word & 0x01) != 0){
				Flags |= FLAG_C;
			}
			else {
				Flags &= (0xFF - FLAG_C);
			}
			temp_word >>= 1;
			if ((temp_word & 0x40) != 0) {
				temp_word += 0x80;
			}
			set_flag_z(temp_word);
			set_flag_p(temp_word);
			set_flag_n(temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x80: //CPIB
			// Data compared to register B (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_B] - data; 
			if (temp_word >= 0x100) {

				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -data, (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x81: //TAP impl
			// Contents of register A (accumulator) copied into page register
			PageRegister = Registers[REGISTER_A];
			break;

		case 0x82: //CMC
			// Compliments carry flag
			Flags ^= FLAG_C;
			break;

		case 0x85: //SUB A,C
			// Subtracts contents of register C from contents of register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] - (WORD)Registers[REGISTER_C];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x86: //SUB A,D
			// Subtracts contents of register D from contents of register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] - (WORD)Registers[REGISTER_D];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x87: //SUB B,C
			// Subtracts contents of register C from contents of register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] - (WORD)Registers[REGISTER_C];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x88: //SUB B,D
			// Contents of register D subtracted from contents of register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] - (WORD)Registers[REGISTER_D];
			if (temp_word >= 0x100) {
				//Set carry flag
				Flags = Flags | FLAG_C;
			}
			else {
				//Clear carry flag
				Flags = Flags & (0xFF - FLAG_C);
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x89: //JVS abs
			// Jumps to subroutine using absolute addressing when overflow flag (flag V) set
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_V) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x8A: //JVS zpg
			// Jumpts to subroutine using zero page addressing when overflow flag (flag V) set
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_V) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x8B: //JVS ind
			// Jumps to subroutine using indexed addressing when overflow flag (flag V) set
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if ((Flags & FLAG_V) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x8C: //JVS pag
			// Jumps to subroutine using paged addressing when overflow flag (flag V) set
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_V) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x8D: //SHR abs
			// Arithmetic shift to the right in memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			temp_word = Memory[address];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x01) != 0) {
					Flags |= FLAG_C;
				}
				else {
					Flags &= (~FLAG_C);
				}
				temp_word = temp_word >> 1;
				temp_word = temp_word & 0x7F;
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x8E: // SHRA
			// Arithmetic shift to the right in register A (accumulator)
			temp_word = Registers[REGISTER_A];
			if ((temp_word & 0x01) != 0){
				Flags |= FLAG_C;
			}
			else {
				Flags &= (0xFF - FLAG_C);
			}
			temp_word >>= 1;
			temp_word &= 0x7F;
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_n((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x8F: // SHRB
			// Arithmetic shift to the right in register B (accumulator)
			temp_word = Registers[REGISTER_B];
			if ((temp_word & 0x01) != 0) {
				Flags |= FLAG_C;
			}
			else {
				Flags &= (0xFF - FLAG_C);
			}
			temp_word >>= 1;
			temp_word &= 0x7F;
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_n((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0x90: //ORIA
			// Data bitwise inclusive OR with register A (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_A] | data; 
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x91: //TPA impl
			// Contents of page register copied to register A (accumulator) 
			Registers[REGISTER_A] = PageRegister;
			break;

		case 0x92: // CMV
			// Compliments overflow flag (flag V)
			Flags ^= FLAG_V;
			break;

		case 0x95: //CMP C,A
			// Compares contents of register A (accumulator) with contents of register C
			temp_word = (WORD)Registers[REGISTER_A] - (WORD)Registers[REGISTER_C];
			if (temp_word >= 0x100) {
				Flags = Flags | FLAG_C; //Set carry flag
			}
			else {
				Flags = Flags & (0xFF - FLAG_C); //Clear carry flag
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x96: //CMP D,A
			// Compares contents of register A (accumulator) with contents of register D
			temp_word = (WORD)Registers[REGISTER_A] - (WORD)Registers[REGISTER_D];
			if (temp_word >= 0x100) {
				Flags = Flags | FLAG_C; //Set carry flag
			}
			else {
				Flags = Flags & (0xFF - FLAG_C); //Clear carry flag
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_A], -Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x97: //CMP C,B
			// Compares contents of register B (accumulator) with contents of register C
			temp_word = (WORD)Registers[REGISTER_B] - (WORD)Registers[REGISTER_C];
			if (temp_word >= 0x100) {
				Flags = Flags | FLAG_C; //Set carry flag
			}
			else {
				Flags = Flags & (0xFF - FLAG_C); //Clear carry flag
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -Registers[REGISTER_C], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x98: //CMP D,B
			// Compares contents of register B (accumulator) with contents of register D
			temp_word = (WORD)Registers[REGISTER_B] - (WORD)Registers[REGISTER_D];
			if (temp_word >= 0x100) {
				Flags = Flags | FLAG_C; //Set carry flag
			}
			else {
				Flags = Flags & (0xFF - FLAG_C); //Clear carry flag
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_v(Registers[REGISTER_B], -Registers[REGISTER_D], (BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0x99: //JMI abs
			// Jumps to subroutine using absolute addressing when result is negative
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_N) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x9A: //JMI zpg
			// Jumps to subroutine using zero page addressing when resut is negative
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_N) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x9B: //JMI ind
			// Jumps to subroutine using indexed addressing when result is negative
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_N) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x9C: //JMI pag
			// Jumps to subroutine using paged addressing when result is negative
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_N) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0x9D: //NOT
			// Inverts contents of memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				temp_word = ~Memory[address];
				if (temp_word >= 0x100) {
					Flags |= FLAG_C;
				}
				else {
					Flags &= (0xFF - FLAG_C);
				}
				set_flag_z((BYTE)temp_word);
				set_flag_p((BYTE)temp_word);
				set_flag_n((BYTE)temp_word);
				Memory[address] = (BYTE)temp_word;
			}
			break;

		case 0x9E: //NOTA
			// Inverts contents of register A (accumulator)
			temp_word = ~Registers[REGISTER_A];
			if (temp_word >= 0x100) {
				Flags = Flags | FLAG_C; //Set carry flag
			}
			else {
				Flags = Flags & (0xFF - FLAG_C); //Clear carry flag
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x9F: //NOTB
			// Inverts contents of register B (accumulator)
			temp_word = ~Registers[REGISTER_B];
			if (temp_word >= 0x100) {
				Flags = Flags | FLAG_C; //Set carry flag
			}
			else {
				Flags = Flags & (0xFF - FLAG_C); //Clear carry flag
			}
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xA0: //ORIB
			// Data bitwise inclusive OR with register B (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_B] | data; 
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xA5: //ORA A,C
			// Register C bitwise inclusive OR with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] | (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xA6: //ORA A,D
			// Register D bitwise inclusive OR with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] | (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xA7: //ORA B,C
			// Register C bitwise inclusive OR with register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] | (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xA8: //ORA B,D
			// Register D bitwise inclusive OR with register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] | (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xA9: //JPL abs
			// Jumps to subroutine using absolute addressing when result is positive
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_N) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xAA: //JPL zpg
			// Jumps to subroutine using zero page addressing when result is positive
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_N) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xAB: //JPL ind
			// Jumps to subroutine using indexed addressing when result is positive
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_N) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xAC: //JPL pag
			// Jumps to subroutine using paged addressing when result is positive
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_N) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xAD: // NEG abs
			// Negates contents of memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				temp_word = 0 - Memory[address];
				set_flag_z((BYTE)temp_word);
				set_flag_p((BYTE)temp_word);
				set_flag_n((BYTE)temp_word);
				Memory[address] = (BYTE)temp_word;
			}
			break;

		case 0xAE: // NEGA
			// Negates contents of register A (accumulator)
			temp_word = 0 - Registers[REGISTER_A];
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_n((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xAF: // NEGB
			// Negates contents of register B (accumulator)
			temp_word = 0 - Registers[REGISTER_B];
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_n((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xB0: //ANIA
			// Data bitwise AND with register A (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_A] & data; 
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xB5: //AND A,C
			// Register C bitwise AND with register A (accumulator) 
			temp_word = (WORD)Registers[REGISTER_A] & (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xB6: //AND A,D
			// Register D bitwise AND with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] & (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xB7: //AND B,C
			// Register C bitwise AND with register B (accumulator) 
			temp_word = (WORD)Registers[REGISTER_B] & (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xB8: //AND B,D
			// Register D bitwise AND with register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] & (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xB9: //JPE abs
			// Jumps to subroutine using absolute addressing when result is even
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_P) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xBA: //JPE zpg
			// Jumps to subroutine using zero page addressing when result is even
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_P) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xBB: //JPE ind
			// Jumps to subroutine using indexed addressing when result is even
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_P) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xBC: //JPE pag
			// Jumps to subroutine using paged addressing when result is even
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_P) == 0) {
				ProgramCounter = address;
			}
			break;

		case 0xBD:	// ROL
			// Rotates memory left with carry
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			temp_word = Memory[address];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = (temp_word << 1) + 0x01;
				}
				else {
					temp_word = temp_word << 1;
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0xBE:	// ROLA
			// Rotates register A (accumulator) left with carry
			temp_word = Registers[REGISTER_A];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = (temp_word << 1) + 0x01;
				}
				else {
					temp_word = temp_word << 1;
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xBF:	// ROLB
			// Rotates register B (accumulator) left with carry
			temp_word = Registers[REGISTER_B];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x80) == 0x80) {
					temp_word = (temp_word << 1) + 0x01;
				}
				else {
					temp_word = temp_word << 1;
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xC0: //ANIB
			// Data bitwise AND with register B (accumulator)
			data = fetch();
			temp_word = (WORD)Registers[REGISTER_B] & data; 
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xC5: //EOR A,C
			// Register C exclusive OR with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] ^ (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xC6: //EOR A,D
			// Register D exclusive OR with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] ^ (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xC7: //EOR B,C
			// Register C exclusive OR with register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] ^ (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xC8: //EOR B,D
			// Register D exclusive OR with register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] ^ (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;

		case 0xC9: //JPO abs
			// Jumps to subroutine using absolute addressing when result odd
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_P) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0xCA: //JPO zpg
			// Jumps to subroutine using zero page addressing when result odd
			address = 0x0000 | (WORD)fetch();
			if ((Flags & FLAG_P) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0xCB: //JP0 ind
			// Jumps to subroutine using indexed addressing when result odd
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_P) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0xCC: //JPO pag
			// Jumps to subroutine using paged addressing when result odd
			HB = PageRegister;
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if ((Flags & FLAG_P) != 0) {
				ProgramCounter = address;
			}
			break;

		case 0xCD:	// RR
			// Rotates memory right without carry
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			temp_word = Memory[address];
			if (address >= 0 && address < MEMORY_SIZE) {
				if ((temp_word & 0x01) == 0x01) {
					temp_word = (temp_word >> 1) + 0x80;
				}
				else {
					temp_word = temp_word >> 1;
				}
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Memory[address] = (BYTE)temp_word;
			break;

		case 0xCE:	// RRA
			// Rotates register A (accumulator) right without carry
			temp_word = Registers[REGISTER_A];
			if ((temp_word & 0x01) == 0x01) {
				temp_word = (temp_word >> 1) + 0x80;
			}
			else {
				temp_word = temp_word >> 1;
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0xCF:	// RRB 
			// Rotates register B (accumulator) right without carry 
			temp_word = Registers[REGISTER_B];
			if ((temp_word & 0x01) == 0x01) {
				temp_word = (temp_word >> 1) + 0x80;
			}
			else {
				temp_word = temp_word >> 1;
			}
			set_flag_n((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_B] = (BYTE)temp_word;
			break;
			
		case 0xD5: //BT A,C
			// Register C bit tested with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] & (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			break;

		case 0xD6: //BT A,D
			// Register D bit tested with register A (accumulator)
			temp_word = (WORD)Registers[REGISTER_A] & (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			break;

		case 0xD7: //BT B,C
			// Register C bit tested with register B (accumulator) 
			temp_word = (WORD)Registers[REGISTER_B] & (WORD)Registers[REGISTER_C];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			break;

		case 0xD8: //BT B,D
			// Register D bit tested with register B (accumulator)
			temp_word = (WORD)Registers[REGISTER_B] & (WORD)Registers[REGISTER_D];
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			set_flag_p((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			break;

		case 0xDD:	// CLR
			// Clears memory
			HB = fetch();
			LB = fetch();
			address = ((WORD)HB << 8) + (WORD)LB;
			if (address >= 0 && address < MEMORY_SIZE){
				Memory[address] = 0;
			}
			Flags |= FLAG_Z;
			Flags &= (0xFF - FLAG_N);
			Flags &= (0xFF - FLAG_C);
			break;

		case 0xDE:	// CLRA
			// Clears register A (accumulator)
			Registers[REGISTER_A] = 0;
			Flags |= FLAG_Z;
			Flags &= (0xFF - FLAG_N);
			Flags &= (0xFF - FLAG_C);
			break;

		case 0xDF:	// CLRB
			// Clears register B (accumulator)
			Registers[REGISTER_B] = 0;
			Flags |= FLAG_Z;
			Flags &= (0xFF - FLAG_N);
			Flags &= (0xFF - FLAG_C);
			break;

		case 0xE0: //LD \# C
			// Data copied into directly into register C
			data = fetch();
			Registers[REGISTER_C] = data;
			break;

		case 0xE1: //LD abs C
			// Data copied into register C using absolute addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_C] = Memory[address];
			}
			break;

		case 0xE2: //LD zpg C
			// Data copied into register C using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_C] = Memory[address];
			}
			break;

		case 0xE3: //LD ind C
			// Data copied into register C using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_C] = Memory[address];
			}
			break;

		case 0xE4: //LD pag C
			// Data copied into register C using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_C] = Memory[address];
			}
			break;

		case 0xE5: //LD bas C
			// Data copied into register C using base offset addressing
			if ((LB = fetch()) >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_C] = Memory[address];
			}
			break;

		case 0xE6: //STA abs
			 // Contents of register A (accumulator) copied into memory using absolute addressing
			 HB = fetch();
			 LB = fetch();
			 address = (WORD)((WORD)HB << 8) + LB;
			 if(address >= 0 && address < MEMORY_SIZE) {
				 Memory[address] = Registers[REGISTER_A];
			 }
			 break;

		case 0xE7: //STA zpg
			// Contents of register A (accumulator) copied into memory using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_A];
			}
			break;

		case 0xE8: //STA ind
			// Contents of register A (accumulator) copied into memory using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_A];
			}
			break;

		case 0xE9: //STA pag
			// Contents of register A (accumulator) copied into memory using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_A];
			}
			break;

		case 0xEA: //STA bas
			// Contents of register A (accumulator) copied into memory using base offset addressing
			if ((LB = fetch())  >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_A];
			}
			break;

		case 0xEB: //PUSH A
			// Pushes register A (accumulator) onto the stack
			if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = Registers[REGISTER_A];
			}
			break;

		case 0xEC: //PUSH B
			// Pushes register B (accumulator) onto the stack
			if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = Registers[REGISTER_B];
			}
			break;

		case 0xED: //PUSH FL
			// Pushes flags onto the stack
			if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = Registers[Flags];
			}
			break;

		case 0xEE: //PUSH C
			// Pushes register C onto the stack
			if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = Registers[REGISTER_C];
			}
			break;

		case 0xEF: //PUSH D
			// Pushes register D onto the stack
			if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE)) {
				--StackPointer;
				Memory[StackPointer] = Registers[REGISTER_D];
			}
			break;

		case 0xF0: //LD \# D
			// Data copied directly into register D
			data = fetch();
			Registers[REGISTER_D] = data;
			break;

		case 0xF1: //LD abs D
			// Data copied into register D using absolute addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_D] = Memory[address];
			}
			break;

		case 0xF2: //LD zpg D
			// Data copied into register D using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_D] = Memory[address];
			}
			break;

		case 0xF3: //LD ind D
			// Data copied into register D using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if(address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_D] = Memory[address];
			}
			break;

		case 0xF4: //LD pag D
			// Data copied into register D using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_D] = Memory[address];
			}
			break;

		case 0xF5: //LD bas D
			// Data copied into register D using base offset
			if ((LB = fetch()) >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Registers[REGISTER_D] = Memory[address];
			}
			break;

		case 0xF6: //STB abs
			 // Contents of register B (accumulator) copied into memory using absolute addressing
			 HB = fetch();
			 LB = fetch();
			 address = (WORD)((WORD)HB << 8) + LB;
			 if(address >= 0 && address < MEMORY_SIZE) {
				 Memory[address] = Registers[REGISTER_B];
			 }
			 break;

		case 0xF7: //STB zpg
			// Contents of register B (accumulator) copied into memory using zero page addressing
			address = 0x0000 | (WORD)fetch();
			if(address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_B];
			}
			break;

		case 0xF8: //STB ind
			// Contents of register B (accumulator) copied into memory using indexed addressing
			HB = fetch();
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			HB = Memory[address];
			LB = Memory[address + 1];
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_B];
			}
			break;

		case 0xF9: //STB pag
			// Contents of register B (accumulator) copied into memory using paged addressing
			HB = PageRegister;
			LB = fetch();
			address = (WORD)((WORD)HB << 8) + LB;
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_B];
			}
			break;

		case 0xFA: //STB bas
			// Contents of register B (accumulator) copied into memory usng base offset addressing
			if ((LB = fetch())  >= 0x80) {
				LB = 0x00 - LB;
				address = (BaseRegister - LB);
			}
			else address = (BaseRegister + LB);
			if (address >= 0 && address < MEMORY_SIZE) {
				Memory[address] = Registers[REGISTER_B];
			}
			break;

		case 0xFB: //POP A
			// Top of stack popped off into register A (accumulator)
			if ((StackPointer >=0) && (StackPointer < MEMORY_SIZE - 1)) {
				Registers[REGISTER_A] = Memory[StackPointer];
				StackPointer++;
			}
			break;

		case 0xFC: //POP B
			// Top of stack popped off into register B (accumulator)
			if ((StackPointer >=0) && (StackPointer < MEMORY_SIZE - 1)) {
				Registers[REGISTER_B] = Memory[StackPointer];
				StackPointer++;
			}
			break;

		case 0xFD: //POP FL
			// Top of stack popped off into flags
			if ((StackPointer >=0) && (StackPointer < MEMORY_SIZE - 1)) {
				Registers[Flags] = Memory[StackPointer];
				StackPointer++;
			}
			break;

		case 0xFE: //POP C
			// Top of stack popped off into register C
			if ((StackPointer >=0) && (StackPointer < MEMORY_SIZE - 1)) {
				Registers[REGISTER_C] = Memory[StackPointer];
				StackPointer++;
			}
			break;

		case 0xFF: //POP D
			// Top of stack popped off into register D
			if ((StackPointer >=0) && (StackPointer < MEMORY_SIZE - 1)) {
				Registers[REGISTER_D] = Memory[StackPointer];
				StackPointer++;
			}
			break;

		default: //unimplemented instruction
			halt = true;
			break;
		}

}

void Group_2_Move(BYTE opcode){
	BYTE destination = opcode >> 4;
	BYTE source = opcode & 0x0F;

	int destReg;
	int sourceReg = 0;

	switch(destination) {
		case 0x0A:
			destReg = REGISTER_A;
			break;
		case 0x0B:
			destReg = REGISTER_B;
			break;
		case 0x0C:
			destReg = REGISTER_C;
			break;
		case 0x0D:
			destReg = REGISTER_D;
			break;
		default: //unimplemented instruction
			halt = true;
			break;
	}

	switch(source) {
		case 0x01:
			sourceReg = REGISTER_A;
			break;
		case 0x02:
			sourceReg = REGISTER_B;
			break;
		case 0x03:
			sourceReg = REGISTER_C;
			break;
		case 0x04:
			sourceReg = REGISTER_D;
			break;
		default: //unimplemented instruction
			halt = true;
			break;
	}

	Registers[destReg] = Registers[sourceReg];
}

void execute(BYTE opcode)
{	

	if(((opcode >= 0xA1) && (opcode <= 0xA4))
	|| ((opcode >= 0xB1) && (opcode <= 0xB4))
	|| ((opcode >= 0xC1) && (opcode <= 0xC4))
	|| ((opcode >= 0xD1) && (opcode <= 0xD4)))
	{
		Group_2_Move(opcode);
	}
	else
	{
		Group_1(opcode);
	}
}

void emulate()
{
	BYTE opcode;

	ProgramCounter = 0;
	halt = false;
	memory_in_range = true;

	printf("                    A  B  C  D  P  Z    SP\n");

	while ((!halt) && (memory_in_range)) {
		printf("%04X ", ProgramCounter);           // Print current address
		opcode = fetch();
		execute(opcode);

		printf("%s  ", opcode_mneumonics[opcode]);  // Print current opcode

		printf("%02X ", Registers[REGISTER_A]);
		printf("%02X ", Registers[REGISTER_B]);
		printf("%02X ", Registers[REGISTER_C]);
		printf("%02X ", Registers[REGISTER_D]);
		printf("%02X ", PageRegister);
		printf("%04X ", BaseRegister);
		printf("%04X ", StackPointer);              // Print Stack Pointer

		if ((Flags & FLAG_I) == FLAG_I)	
		{
			printf("I=1 ");
		}
		else
		{
			printf("I=0 ");
		}
		if ((Flags & FLAG_Z) == FLAG_Z)	
		{
			printf("Z=1 ");
		}
		else
		{
			printf("Z=0 ");
		}
		if ((Flags & FLAG_P) == FLAG_P)	
		{
			printf("P=1 ");
		}
		else
		{
			printf("P=0 ");
		}
		if ((Flags & FLAG_V) == FLAG_V)	
		{
			printf("V=1 ");
		}
		else
		{
			printf("V=0 ");
		}
		if ((Flags & FLAG_N) == FLAG_N)	
		{
			printf("N=1 ");
		}
		else
		{
			printf("N=0 ");
		}
		if ((Flags & FLAG_C) == FLAG_C)	
		{
			printf("C=1 ");
		}
		else
		{
			printf("C=0 ");
		}

		printf("\n");  // New line
	}

	printf("\n");  // New line
}


////////////////////////////////////////////////////////////////////////////////
//                            Simulator/Emulator (End)                        //
////////////////////////////////////////////////////////////////////////////////


void initialise_filenames() {
	int i;

	for (i=0; i<MAX_FILENAME_SIZE; i++) {
		hex_file [i] = '\0';
		trc_file [i] = '\0';
	}
}




int find_dot_position(char *filename) {
	int  dot_position;
	int  i;
	char chr;

	dot_position = 0;
	i = 0;
	chr = filename[i];

	while (chr != '\0') {
		if (chr == '.') {
			dot_position = i;
		}
		i++;
		chr = filename[i];
	}

	return (dot_position);
}


int find_end_position(char *filename) {
	int  end_position;
	int  i;
	char chr;

	end_position = 0;
	i = 0;
	chr = filename[i];

	while (chr != '\0') {
		end_position = i;
		i++;
		chr = filename[i];
	}

	return (end_position);
}


bool file_exists(char *filename) {
	bool exists;
	FILE *ifp;

	exists = false;

	if ( ( ifp = fopen( filename, "r" ) ) != NULL ) {
		exists = true;

		fclose(ifp);
	}

	return (exists);
}



void create_file(char *filename) {
	FILE *ofp;

	if ( ( ofp = fopen( filename, "w" ) ) != NULL ) {
		fclose(ofp);
	}
}



bool getline(FILE *fp, char *buffer) {
	bool rc;
	bool collect;
	char c;
	int  i;

	rc = false;
	collect = true;

	i = 0;
	while (collect) {
		c = getc(fp);

		switch (c) {
		case EOF:
			if (i > 0) {
				rc = true;
			}
			collect = false;
			break;

		case '\n':
			if (i > 0) {
				rc = true;
				collect = false;
				buffer[i] = '\0';
			}
			break;

		default:
			buffer[i] = c;
			i++;
			break;
		}
	}

	return (rc);
}






void load_and_run(int args,_TCHAR** argv) {
	char chr;
	int  ln;
	int  dot_position;
	int  end_position;
	long i;
	FILE *ifp;
	long address;
	long load_at;
	int  code;

	// Prompt for the .hex file

	printf("\n");
	printf("Enter the hex filename (.hex): ");

	if(args == 2){
		ln = 0;
		chr = argv[1][ln];
		while (chr != '\0')
		{
			if (ln < MAX_FILENAME_SIZE)
			{
				hex_file [ln] = chr;
				trc_file [ln] = chr;
				ln++;
			}
			chr = argv[1][ln];
		}
	} else {
		ln = 0;
		chr = '\0';
		while (chr != '\n') {
			chr = getchar();

			switch(chr) {
			case '\n':
				break;
			default:
				if (ln < MAX_FILENAME_SIZE)	{
					hex_file [ln] = chr;
					trc_file [ln] = chr;
					ln++;
				}
				break;
			}
		}

	}
	// Tidy up the file names

	dot_position = find_dot_position(hex_file);
	if (dot_position == 0) {
		end_position = find_end_position(hex_file);

		hex_file[end_position + 1] = '.';
		hex_file[end_position + 2] = 'h';
		hex_file[end_position + 3] = 'e';
		hex_file[end_position + 4] = 'x';
		hex_file[end_position + 5] = '\0';
	} else {
		hex_file[dot_position + 0] = '.';
		hex_file[dot_position + 1] = 'h';
		hex_file[dot_position + 2] = 'e';
		hex_file[dot_position + 3] = 'x';
		hex_file[dot_position + 4] = '\0';
	}

	dot_position = find_dot_position(trc_file);
	if (dot_position == 0) {
		end_position = find_end_position(trc_file);

		trc_file[end_position + 1] = '.';
		trc_file[end_position + 2] = 't';
		trc_file[end_position + 3] = 'r';
		trc_file[end_position + 4] = 'c';
		trc_file[end_position + 5] = '\0';
	} else {
		trc_file[dot_position + 0] = '.';
		trc_file[dot_position + 1] = 't';
		trc_file[dot_position + 2] = 'r';
		trc_file[dot_position + 3] = 'c';
		trc_file[dot_position + 4] = '\0';
	}

	if (file_exists(hex_file)) {
		// Clear Registers and Memory

		Registers[REGISTER_A] = 0;
		Registers[REGISTER_B] = 0;
		Registers[REGISTER_C] = 0;
		Registers[REGISTER_D] = 0;
		PageRegister = 0;
		BaseRegister = 0;
		Flags = 0;
		ProgramCounter = 0;
		StackPointer = 0;

		for (i=0; i<MEMORY_SIZE; i++) {
			Memory[i] = 0x00;
		}

		// Load hex file

		if ( ( ifp = fopen( hex_file, "r" ) ) != NULL ) {
			printf("Loading file...\n\n");

			load_at = 0;

			while (getline(ifp, InputBuffer)) {
				if (sscanf(InputBuffer, "L=%x", &address) == 1) {
					load_at = address;
				} else if (sscanf(InputBuffer, "%x", &code) == 1) {
					if ((load_at >= 0) && (load_at <= MEMORY_SIZE)) {
						Memory[load_at] = (BYTE)code;
					}
					load_at++;
				} else {
					printf("ERROR> Failed to load instruction: %s \n", InputBuffer);
				}
			}

			fclose(ifp);
		}

		// Emulate

		emulate();
	} else {
		printf("\n");
		printf("ERROR> Input file %s does not exist!\n", hex_file);
		printf("\n");
	}
}

void building(int args,_TCHAR** argv){
	char buffer[1024];
	load_and_run(args,argv);
	sprintf(buffer, "0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X", 
		Memory[TEST_ADDRESS_1],
		Memory[TEST_ADDRESS_2],
		Memory[TEST_ADDRESS_3],
		Memory[TEST_ADDRESS_4], 
		Memory[TEST_ADDRESS_5],
		Memory[TEST_ADDRESS_6], 
		Memory[TEST_ADDRESS_7],
		Memory[TEST_ADDRESS_8], 
		Memory[TEST_ADDRESS_9], 
		Memory[TEST_ADDRESS_10],
		Memory[TEST_ADDRESS_11],
		Memory[TEST_ADDRESS_12]
		);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));
}



void test_and_mark() {
	char buffer[1024];
	bool testing_complete;
	int  len = sizeof(SOCKADDR);
	char chr;
	int  i;
	int  j;
	bool end_of_program;
	long address;
	long load_at;
	int  code;
	int  mark;
	int  passed;

	printf("\n");
	printf("Automatic Testing and Marking\n");
	printf("\n");

	testing_complete = false;

	sprintf(buffer, "Test Student %s", STUDENT_NUMBER);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));

	while (!testing_complete) {
		memset(buffer, '\0', sizeof(buffer));

		if (recvfrom(sock, buffer, sizeof(buffer)-1, 0, (SOCKADDR *)&client_addr, &len) != SOCKET_ERROR) {
			printf("Incoming Data: %s \n", buffer);

			//if (strcmp(buffer, "Testing complete") == 1)
			if (sscanf(buffer, "Testing complete %d", &mark) == 1) {
				testing_complete = true;
				printf("Current mark = %d\n", mark);

			}else if (sscanf(buffer, "Tests passed %d", &passed) == 1) {
				//testing_complete = true;
				printf("Passed = %d\n", passed);

			} else if (strcmp(buffer, "Error") == 0) {
				printf("ERROR> Testing abnormally terminated\n");
				testing_complete = true;
			} else {
				// Clear Registers and Memory

		Registers[REGISTER_A] = 0;
		Registers[REGISTER_B] = 0;
		Registers[REGISTER_C] = 0;
		Registers[REGISTER_D] = 0;
		PageRegister = 0;
		BaseRegister = 0;
				Flags = 0;
				ProgramCounter = 0;
				StackPointer = 0;
				for (i=0; i<MEMORY_SIZE; i++) {
					Memory[i] = 0;
				}

				// Load hex file

				i = 0;
				j = 0;
				load_at = 0;
				end_of_program = false;
				FILE *ofp;
				fopen_s(&ofp ,"branch.txt", "a");

				while (!end_of_program) {
					chr = buffer[i];
					switch (chr) {
					case '\0':
						end_of_program = true;

					case ',':
						if (sscanf(InputBuffer, "L=%x", &address) == 1) {
							load_at = address;
						} else if (sscanf(InputBuffer, "%x", &code) == 1) {
							if ((load_at >= 0) && (load_at <= MEMORY_SIZE)) {
								Memory[load_at] = (BYTE)code;
								fprintf(ofp, "%02X\n", (BYTE)code);
							}
							load_at++;
						} else {
							printf("ERROR> Failed to load instruction: %s \n", InputBuffer);
						}
						j = 0;
						break;

					default:
						InputBuffer[j] = chr;
						j++;
						break;
					}
					i++;
				}
				fclose(ofp);
				// Emulate

				if (load_at > 1) {
					emulate();
					// Send and store results
					sprintf(buffer, "%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X", 
						Memory[TEST_ADDRESS_1],
						Memory[TEST_ADDRESS_2],
						Memory[TEST_ADDRESS_3],
						Memory[TEST_ADDRESS_4], 
						Memory[TEST_ADDRESS_5],
						Memory[TEST_ADDRESS_6], 
						Memory[TEST_ADDRESS_7],
						Memory[TEST_ADDRESS_8], 
						Memory[TEST_ADDRESS_9], 
						Memory[TEST_ADDRESS_10],
						Memory[TEST_ADDRESS_11],
						Memory[TEST_ADDRESS_12]
						);
					sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));
				}
			}
		}
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	char chr;
	char dummy;

	printf("\n");
	printf("Microprocessor Emulator\n");
	printf("UWE Computer and Network Systems Assignment 1\n");
	printf("\n");

	initialise_filenames();

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return(0);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	if (!sock) {	
		// Creation failed! 
	}

	memset(&server_addr, 0, sizeof(SOCKADDR_IN));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS_SERVER);
	server_addr.sin_port = htons(PORT_SERVER);

	memset(&client_addr, 0, sizeof(SOCKADDR_IN));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(PORT_CLIENT);

	chr = '\0';
	while ((chr != 'e') && (chr != 'E'))
	{
		printf("\n");
		printf("Please select option\n");
		printf("L - Load and run a hex file\n");
		printf("T - Have the server test and mark your emulator\n");
		printf("E - Exit\n");
		if(argc == 2){ building(argc,argv); exit(0);}
		printf("Enter option: ");
		chr = getchar();
		if (chr != 0x0A)
		{
			dummy = getchar();  // read in the <CR>
		}
		printf("\n");

		switch (chr)
		{
		case 'L':
		case 'l':
			load_and_run(argc,argv);
			break;

		case 'T':
		case 't':
			test_and_mark();
			break;

		default:
			break;
		}
	}

	closesocket(sock);
	WSACleanup();


	return 0;
}