#include<iostream>
#include"execute.hpp"
#include"main_control.hpp"
#include"alu.hpp"
#include"myARMSim.h"
using namespace std;
extern inter_stage_buffer_EX_MEM B3;

extern inter_stage_buffer_ID_EX B2;


void execute()
{
	string opcode=B2.opcode;
	//cout<<"helllllllllllllllooooooooooin execute"<<endl;
	//cout<<"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"<<opcode<<endl;
	//cout<<instruction_word<<endl;
	if(opcode=="0010111"||opcode=="0110111"||opcode=="1101111")
	{
		if(opcode=="1101111")
		{
			PCSRC=1;
			cout<<"No operation in ALU"<<endl;
		}
		//return;
	}		
  	B3.RZ=ALU(B2.alu_cntrl_sgnl);
	  //cout<<"B3.RZ"<<B3.RZ<<endl;
	B3.opcode=B2.opcode;
	B3.func3=B2.func3;
	B3.instruction_wrd=B2.inst_wrd;
	B3.RB=B2.RB;
	//cout<<"after control unit"<<B3.reg_write<<endl;
	B3.reg_write=B2.reg_write;
	B3.mem_Read=B2.mem_Read;
	B3.mem_write=B2.mem_write;
	B3.Rd_address=B2.iRd;
	B3.PC=B2.PC;
}	




