#include<bits/stdc++.h>
#include<string>
using namespace std;
#ifndef BUFFERS_HPP
#define BUFFERS_HPP

//#include<string>


class inter_stage_buffer_ID_EX          //B2
{
public:
	int RA;//from IF/ID
	int RB;//from IF/ID
	string iRA;//from regfile
	string iRB;//from regfile
    int iRM;
    int reg_write;
    string iRd;  //address of rd
	int MeM_to_reg;
	int branch;
	int mem_Read;
	int mem_write;
    string alu_cntrl_sgnl;
    string opcode;
    unsigned int PC;
    string func3;
    int hazard_found;
    unsigned int  inst_wrd;

};

class inter_stage_buffer_MEM_WB   //B4
{
public:
	int RY;
    string opcode;
    string func3;
    unsigned int instruction_wrd;
    int RB;
    int hazard_found;
    int reg_write;
    string iRd;
};

class inter_stage_buffer_EX_MEM    //B3
{
public:
	unsigned int RZ;
    unsigned int PC;
    string Rd_address;
    string opcode;
    int hazard_found;
    string func3;
    unsigned int  instruction_wrd;
    int RB;
    int reg_write;
    int mem_Read;
	int mem_write;
};

class inter_stage_buffer_IF_ID  //B1
{
public:
	unsigned int instruc_from_mem;
	unsigned int PC;
	int flush;
	int control_hazard;
    int hazard_found;
};

#endif