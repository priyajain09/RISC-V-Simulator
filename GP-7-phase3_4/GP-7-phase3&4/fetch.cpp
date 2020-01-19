
#include<bits/stdc++.h>
#include "myARMSim.h"
#include <cstdlib>
#include <cstdio>
#include "buffers.hpp"
#include<string>
using namespace std;
//--------------------------akansha------------------------------------

extern int flush_inst;

extern 	int num_inst;
extern int c;
extern int stall;
extern int contrl_hazards;
extern  int mis_predictions;
extern inter_stage_buffer_IF_ID B1;
extern unsigned int R[32];
extern int predict;
extern int hazard_cntrl;
extern int comp;
extern  int total_num_exe_inst;
extern unsigned int PC_count;
int binstr1_to_dec(string x)
{
	int sum=0;
	for(int i=3;i>=0;i--)
	{
		int c=x[i];
		sum=sum+(c-48)*pow(2,3-i);
	}
	return sum;
}

string bin1_to_hex(string mac_bin)
{
	string mac_code;
	for(int i=0;i<32;i+=4)
	{
		string x=mac_bin.substr(i,4);
		stringstream ss(x);
		int a=binstr1_to_dec(x);
		if(a<=9)
		{
			char c=48+a;	
			mac_code+=c;
		}
		else
		{
			char c=55+a;
			mac_code+=c;
		}
		x="";		
	}
	return mac_code;
		
}

//reads from the instruction memory and updates the instruction register
void fetch() {
	
	
	//cout<<endl<<PC_count<<"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
	comp=0;
	unsigned int instruction_word = cache_mod(PC_count,0,0);
	//cout<<"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"<<instruction_word<<endl;
	bitset<32> pc(PC_count);
	//return_add = PC_count;
	//cout<<return_add<<"****************************************************************************"<<endl;
	if(instruction_word==4009754641)
	{
		//cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ N in it"<<N<<endl;
		N=1;
	}
	bitset<32> instword(instruction_word);
	//cout<<"++++++"<<instword<<endl;
	
	
	string istring = instword.to_string();
	string pcstring = pc.to_string();
	//num_inst++;
	cout<<"FETCH:Fetch instruction 0x"<<bin1_to_hex(istring)<<" from address 0x"<<bin1_to_hex(pcstring)<< endl;
	total_num_exe_inst++;
	string s_instword = instword.to_string();
	//cout<<s_instword<<endl;
	
 	string opcode = s_instword.substr(25, 7);
	string s_imm2, s_imm1;
	string s_imm;
	if(opcode=="1100011"){// SB instruction
		s_imm2 = s_instword.substr(0,7);
		s_imm1 = s_instword.substr(20,5);
		string s_imm;
		s_imm="0";
		s_imm += s_imm1[4];
		
		for(int i=1; i<7; i++){
			s_imm +=s_imm2[i];
		}
		for(int i=0;i<4; i++)
		{
			s_imm +=s_imm1[i];
		}
		s_imm += s_imm2[0];
		string s= s_imm.substr(0,1);
		bitset<sizeof(int) * 8> rimm(s_imm.substr(1,12));
		bitset<sizeof(int) * 8> rimm2(s);
		int tempimm = rimm.to_ulong();
		int tempimm2 =rimm2.to_ulong();
		jump_imm_offset = tempimm - tempimm2*pow(2, 12);
		//cout<<"JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"<<jump_imm_offset<<endl;
		comp=3;
		//cout<<"imm2"<<imm2<<endl;
		//cout<<"imm1"<<imm1<<endl;
		//cout<<"rs2"<<rs2<<endl;
		//cout<<"rs1"<<rs1<<endl;
		//cout<<"func3"<<func3<<endl;
	}
	string s_rd="";
	if(opcode=="1101111"){// UJ instruction
		s_imm = s_instword.substr(0,20);
		s_rd = s_instword.substr(20,5);

		bitset<sizeof(int)*8> ra(4);
		//iRA = ra.to_ulong();
		// string simm;
		bitset<21> bimm;
		bimm[0]=0;
		bimm[20]=s_imm[0]-48;
		for(int h=10;h>=1;h--)
		{
			bimm[h]=s_imm[11-h]-48;
		}
		bimm[11]=s_imm[11]-48;
		for(int h=19;h>=12;h--)
		{
			bimm[h]=s_imm[31-h]-48;
		}
		// s_fin[0] = s_imm[0];
		// for(int i=12; i<=19; i++)
		// {
		// 	s_fin[i-11]=s_imm[i];
		// }
		// s_fin[9]=s_imm[11];
		// for(int i=1; i<=10; i++)
		// {
		// 	s_fin[i+9]=s_imm[i];
		// }
		// s_fin[20]=0;
		// cout<<s_fin<<endl;
		// cout<<s_fin.length();

		string s;//= s_fin.substr(0,1);
		//s= s_fin.substr(0,1);
		s=bimm.to_string();
		//cout<<"?????"<<endl;
		bitset<sizeof(int) * 8> rimm(s.substr(1,20));
		bitset<1> rimm2(bimm[20]);
		//cout<<bimm[0]<<"bimm"<<endl;
		//cout<<"hello"<<endl;
		//cout<<rimm2<<"RIMM2"<<endl;
		int tempimm = rimm.to_ulong();
		int tempimm2 =rimm2.to_ulong();
		jump_imm_offset = tempimm - tempimm2*pow(2, 20);
		comp=1;
		//cout<<"jump_offset:"<<jump_imm_offset<<endl;
		//cout<<"iRA"<<iRA<<endl;
		//cout<<"iRB"<<iRB<<endl;
		
		//cout<<"imm"<<imm<<endl;
		//cout<<"rd"<<rd<<endl;
		
	}
	string s_rs1,funct3,iRd;
	int iRA,iRB,RA,RB;
	if(opcode=="1100111")
	{
		//cout<<"yes in it"<<endl;
		string s_imm;
	
		for(int i=20; i<25; i++)
    		{
    			s_rd+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_rd<<endl;
    		for(int i=17; i<20; i++)
    		{
    			s_rd+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_rd<<endl;
    		for(int i=12; i<17; i++)
    		{
    			s_rs1+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_rd<<endl;
    		for(int i=17; i<20; i++)
    		{
    			funct3+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<"funct3"<<funct3<<endl;
    		for(int i=0; i<12; i++)
    		{
    			s_imm += s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_imm<<endl;
    		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_imm<<endl;
	
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rd<<endl;
		//bitset<sizeof(int) * 8> rimm(s_imm);
		//iRB = rimm.to_ulong();
		//cout << "3333333333333333333333333"<<rimm.to_ulong() <<iRB<< '\n';
		bitset<sizeof(int) * 8> rd(s_rd);
		iRd = rd.to_ulong();
	//	cout << "3333333333333333333333333dddddddddddddddddddddddddddddddd"<<rd.to_ulong() <<iRd<< '\n';
		bitset<sizeof(int) * 8> ra(s_rs1);
		iRA = ra.to_ulong();
		//cout << "3333333333333333333333333aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<ra.to_ulong() <<iRA<< '\n';
		string s= s_imm.substr(0,1);
		bitset<sizeof(int) * 8> rimm(s_imm.substr(1,11));
		bitset<sizeof(int) * 8> rimm2(s);
		int tempimm = rimm.to_ulong();
		int tempimm2 =rimm2.to_ulong();
		iRB = tempimm - tempimm2*pow(2, 11);
		//cout << "3333333333333333333333333bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"<<rimm.to_ulong() <<iRB<< '\n';
		RA=R[iRA];
		RB=iRB;
		
		//jump_imm_offset=iRB+R[iRA];
		jump_imm_offset=RA+RB;
		comp=2;
	}
//	B1.PC=PC_count;
	if(flush_inst==1)
	{
		//c--;
		//num_inst--;
		mis_predictions++;
		contrl_hazards++;
		B1.instruc_from_mem=0;
	}
	else
	{
		//
		//cout<<"i am hereeeeeeeeeeeeeeeeeeeeeeeeeeee"<<endl;
		B1.instruc_from_mem=instruction_word;
	}

	
	B1.PC=PC_count;
	PC_generator();

	//B1.flush=from control;
	//B1.control_hazard=from hazard;


}
