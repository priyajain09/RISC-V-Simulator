#ifndef MYARMSIM_H
#define MYARMSIM_H
/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* myRISCVSim.h
   Purpose of this file: header file for myRISCVSim
*/
//Register file
//#include<bits/stdc++.h>
#include<string>
#include"buffers.hpp"
using namespace std;
extern inter_stage_buffer_EX_MEM B3;
extern inter_stage_buffer_ID_EX B2;
extern inter_stage_buffer_IF_ID B1;

extern inter_stage_buffer_MEM_WB B4;
extern unsigned int R[32];






extern int predict;
extern int flush_inst;
extern int hazard_cntrl;//changed intial values here;
extern int comp;
extern int global_PC;
//flags







extern int N,C,V,Z;
//memory
extern unsigned char MEM[4096];

//intermediate datapath and control path signals
//extern unsigned int instruction_word;
 extern unsigned int operand1;
extern unsigned int operand2;
extern signed int jump_imm_offset;
extern unsigned int PCSRC;
 extern unsigned int PC_count;//here also changed
extern unsigned int PC_add;
extern unsigned int return_add;
extern int  clock_cycle;//here 1 intialyy intialised

////////////////////////////////////
extern int cold_missI;
extern int capacity_missI;
extern int conflict_missI;
extern int hitsI;
extern int cold_missD;
extern int capacity_missD;
extern int conflict_missD;
extern int hitsD;

extern unsigned char cacheI[4096];
extern int tagI[4096];
extern bool valid_bitI[4096];
extern unsigned int LRUI[4096];
extern unsigned char cacheD[4096];
extern int tagD[4096];
extern bool valid_bitD[4096];
extern unsigned int LRUD[4096];
extern int noSets;
unsigned int cache_mod(unsigned int add, int x, int y);
extern int cachesizeI;
extern int blocksize;
extern int cachetype;
extern int cachesizeD;
extern int blocksizeD;
extern int cachetypeD;
 
////////////////////////////////////
extern string operation;
// extern int ry, RZ;
// extern int RA, RB;

void run_RISCVsim();
void reset_proc();
void load_program_memory(char* file_name);
//void write_data_memory();
//void swi_exit();

int PC_generator();
//reads from the instruction memory and updates the instruction register
void fetch();
//reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode();

void mem();
void write_back();

int read1_word(unsigned char *mem, unsigned int address);
void write1_word(unsigned char *mem, unsigned int address, unsigned int data);
#endif
