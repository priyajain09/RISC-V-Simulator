
#include<bits/stdc++.h>
#include "myARMSim.h"
#include <cstdlib>
#include <cstdio>
#include<string>
#include"buffers.hpp"
#include "main_control.hpp"
using namespace std;

extern inter_stage_buffer_ID_EX B2;
extern inter_stage_buffer_IF_ID B1;
extern inter_stage_buffer_EX_MEM B3;
extern inter_stage_buffer_MEM_WB B4;
extern  int predict;
extern  int flush_inst;
extern int num_inst_cntrl;
extern int hazard_cntrl;
extern int comp;
extern int ALU;
extern int data_transfer;
extern int global_PC;
extern int clock_cycle;
extern int num_data_hazards;
extern int stall_data;
extern int total_num_exe_inst;
extern int data_knob;
int Hazard_detection_unit();
extern int pipelining_knob;

//reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode() 
{
int flg=0;
	string  s_rs2="", s_rs1="", s_rd="";
	flush_inst=0;
	hazard_cntrl=0;
	int iRA=0;//change
	int iRM=0;
	int iRd=0;
	int iRB=0;//change
	int RA=0;//
	int RB=0;//change
	string funct3="";
	string funct7="";
	string opcode="";
	unsigned int instruction_word=B1.instruc_from_mem;
	bitset<32> instword(instruction_word);
	//cout<<"++++++"<<instword<<endl;
	int valid_rs1=0,valid_rs2=0;
	
	string s_instword = instword.to_string();
	//cout<<s_instword<<endl;
	
 	opcode = s_instword.substr(25, 7);
	//cout<<"00000000000000000000000000000000000000000000000000000opcode"<<opcode<<endl;

	//bitset<sizeof(int) * 8> b(opcode);
	//iopcode = b.to_ulong();
	//cout << b.to_ulong() <<iopcode<< '\n';

	//string funct3, funct7;
	
	if(opcode=="0110011" || opcode=="0111011")          //Decoding R-format instruction
	{
		//cout<<"yes in it"<<endl;
		ALU++;
		
		for(int i=20; i<25; i++)
    		{
    			s_rd+=s_instword[i];
    		
    		}
    		//cout<<s_rd<<endl;
    		for(int i=17; i<20; i++)
    		{
    			funct3+=s_instword[i];
    		}
    		//cout<<"funct3"<<funct3<<endl;
    		for(int i=12; i<17; i++)
    		{
    			s_rs1+=s_instword[i];
    		}
    		//cout<<s_rs1<<endl;
    		for(int i=7; i<12; i++)
    		{
    			s_rs2+=s_instword[i];
    		}
    		//cout<<s_rs2<<endl;
    		for(int i=0; i<7; i++)
    		{
    			funct7+=s_instword[i];
    		}
    		//cout<<funct7<<endl;
    		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rs1<<endl;
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rs2<<endl;
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rd<<endl;
		bitset<sizeof(int) * 8> ra(s_rs1);
		iRA = ra.to_ulong();
		RA=R[iRA];
		//cout << "3333333333333333333333333"<<ra.to_ulong() <<iRA<< '\n';
		bitset<sizeof(int) * 8> rb(s_rs2);
		iRB = rb.to_ulong();
		RB=R[iRB];
		//cout << "3333333333333333333333333"<<rb.to_ulong() <<iRB<< '\n';
		bitset<sizeof(int) * 8> rd(s_rd);
		iRd = rd.to_ulong();
		//cout << "3333333333333333333333333"<<rd.to_ulong() <<iRd<< '\n';
		valid_rs1=1;
		valid_rs2=1;
		
	}
	
	
	
	if(opcode=="0010111" || opcode=="0110111") //auipc lui not checked dont use it
	{
		//cout<<"yes in it"<<endl;
		string s_imm;
	
		for(int i=20; i<25; i++)
    		{
    			s_rd+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_rd<<endl;
    		for(int i=0; i<20; i++)
    		{
    			s_imm += s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_imm<<endl;
    		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_imm<<endl;
	
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rd<<endl;
		bitset<sizeof(int) * 8> rimm(s_imm);
		iRB = rimm.to_ulong();
		//cout << "3333333333333333333333333bbbbbbbbbbbbb"<<rimm.to_ulong() <<iRB<< '\n';
		
		bitset<sizeof(int) * 8> rd(s_rd);
		iRd = rd.to_ulong();
		//cout << "3333333333333333333333333"<<rd.to_ulong() <<iRd<< '\n';
		iRA=0;
		RB=iRB;
	}
	if(opcode=="0000011"||opcode=="0100011"||opcode=="0010011"||opcode=="0011011") //I-format
	{
		if(opcode=="0100011")     // sd 
		{
			ALU++;
			data_transfer++;
			for(int i=17; i<20; i++)
    			{
    				funct3+=s_instword[i];
    			
    			}
    			//cout<<funct3<<endl;
    			if(funct3=="011")
    			{
    				//mac_bin = s_imm.substr(5,11) + s_rs2  + s_rs1 + funct3 +imm_str + opcode;
				string s_imm5_11, imm_str;
				for(int i=20; i<25; i++)
    				{
    					imm_str+=s_instword[i];
    			
    				}
    				//cout<<imm_str<<endl;
    			
    				for(int i=12; i<17; i++)
    				{
    					s_rs1+=s_instword[i];
    		
    				}
    				//cout<<s_rs1<<endl;
    				for(int i=7; i<12; i++)
    				{
    					s_rs2+=s_instword[i];
    				
    				}
    				//cout<<s_rs2<<endl;
				for(int i=0; i<7; i++)
    				{
    					s_imm5_11+=s_instword[i];
    				
    				}	
    				//cout<<s_imm5_11<<endl;	
    				string s_imm = 	s_imm5_11 + imm_str;
    				
    				//cout<<s_imm<<endl;
    		                //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_imm<<endl;
	
		                //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rs1<<endl;
		                //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rs2<<endl;
		                cout<<"############B2.RA:"<<B2.RA<<endl;
		                bitset<sizeof(int) * 8> ra(s_rs1);
				iRA = ra.to_ulong();
				RA=R[iRA];
				//cout << "3333333333333333333333333aaaaaaaaaaaaaaaaaaaaaaa"<<ra.to_ulong() <<iRA<< '\n';
				bitset<sizeof(int) * 8> rm(s_rs2);
				iRM = rm.to_ulong();
			//	cout << "3333333333333333333333333mmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<rm.to_ulong() <<iRM<< '\n';
				string s= s_imm.substr(0,1);
				bitset<sizeof(int) * 8> rimm(s_imm.substr(1,11));
				bitset<sizeof(int) * 8> rimm2(s);
				int tempimm = rimm.to_ulong();
			//	cout<<"ttttttttttttttttttttttttttt111111111111111111111111"<<tempimm<<endl;
				int tempimm2 =rimm2.to_ulong();
			//	cout<<"tttttttttttttttttttttttttttt22222222222222222"<<tempimm2<<endl;
				iRB = tempimm - tempimm2*pow(2, 11);
				//cout << "3333333333333333333333333bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"<<rimm.to_ulong() <<iRB<< '\n';				
				RB=iRB;
    				
			}
		}
		if(opcode=="0010011"||opcode=="0011011")//andi addiw
		{
			string  s_imm;
			ALU++;
	
			for(int i=20; i<25; i++)
    			{
    				s_rd+=s_instword[i];
    			
    			}
    			//cout<<s_rd<<endl;
    			for(int i=17; i<20; i++)
    			{
    				funct3+=s_instword[i];
    			
    			}
    			//cout<<funct3<<endl;
    			for(int i=12; i<17; i++)
    			{
    				s_rs1+=s_instword[i];
    		
    			}
    			//cout<<s_rs1<<endl;
    			int flag=0;
			for(int i=0; i<7; i++)
    			{
    				funct7+=s_instword[i];
    				if(funct7=="0000000"||funct7=="0100000")
    				{
    					flag=1;	
    				}
    			}	
    			//cout<<funct7<<endl;
    			for(int i=7; i<12; i++)
    			{
    				s_imm+=s_instword[i];
    				
    			}
    			if(flag==0)
    			{
    				s_imm = funct7+s_imm;
    				
    			}
    			//cout<<s_imm<<endl;	
    			
    			bitset<sizeof(int) * 8> ra(s_rs1);
			iRA = ra.to_ulong();
			RA=R[iRA];
			//cout << "3333333333333333333333333aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<ra.to_ulong() <<iRA<< '\n';
			bitset<sizeof(int) * 8> rd(s_rd);
			iRd = rd.to_ulong();
			//cout << "3333333333333333333333333dddddddddddddddddddddddddddddddddddddddddd"<<rd.to_ulong() <<iRd<< '\n';
			string s= s_imm.substr(0,1);
			bitset<sizeof(int) * 8> rimm(s_imm.substr(1,11));
			bitset<sizeof(int) * 8> rimm2(s);
			int tempimm = rimm.to_ulong();
			int tempimm2 =rimm2.to_ulong();
			iRB = tempimm - tempimm2*pow(2, 11);
			//cout << "3333333333333333333333333mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<rimm.to_ulong() <<iRB<< '\n';		
				RB=iRB;
			valid_rs1=1;
			//cout<<"rrrraaaaaaaaaaaaaaaaaaaaaa"<<RA<<endl;
			//cout<<"rrrrbbbbbbbbbbbbbbbbbbbbbbb"<<RB<<endl;
			//bitset<sizeof(int) * 8> rimm(s_imm);
			//iRB = rimm.to_ulong();
			//cout << "3333333333333333333333333"<<rimm.to_ulong() <<iRB<< '\n';
		}
		
		
		
	}
	
	
	if(opcode=="0000011")  //load
	{
		//cout<<"yes in it"<<endl;
		string  s_imm;
		ALU++;
		data_transfer++;
		for(int i=20; i<25; i++)
    		{
    			s_rd+=s_instword[i];
    		
    		}
    		//cout<<s_rd<<endl;
    		for(int i=17; i<20; i++)
    		{
    			funct3+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<"funct3"<<funct3<<endl;
    		for(int i=12; i<17; i++)
    		{
    			s_rs1+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_rs1<<endl;
    		for(int i=0; i<12; i++)
    		{
    			s_imm+=s_instword[i];
    			//cout<<funct3[i-8]<<inst[i]<<endl;
    		}
    		//cout<<s_imm<<endl;
    		
    		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rs1<<endl;
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_imm<<endl;
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<s_rd<<endl;
		bitset<sizeof(int) * 8> ra(s_rs1);
		iRA = ra.to_ulong();
		//cout << "3333333333333333333333333aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<ra.to_ulong() <<iRA<< '\n';
		
		bitset<sizeof(int) * 8> rd(s_rd);
		iRd = rd.to_ulong();
		//cout << "3333333333333333333333333ddddddddddddddddddddddddddddddddddddddd"<<rd.to_ulong() <<iRd<< '\n';
		
		string s= s_imm.substr(0,1);
		bitset<sizeof(int) * 8> rimm(s_imm.substr(1,11));
		bitset<sizeof(int) * 8> rimm2(s);
		int tempimm = rimm.to_ulong();
		int tempimm2 =rimm2.to_ulong();
		iRB = tempimm - tempimm2*pow(2, 11);
		//cout << "3333333333333333333333333mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<rimm.to_ulong() <<iRB<< '\n';
    		RB=iRB;
    		RA=R[iRA];
			valid_rs1=1;
	}
	
	if(opcode=="1100111")  // jalr
	{
		ALU++;
		num_inst_cntrl++;
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
		valid_rs1=1;
		//jump_imm_offset=iRB+R[iRA];
		
	}
	
	string s_imm2, s_imm1;
	string s_imm;
	//cout<<"OKAY OKAY"<<endl;
	//opcode="1101111";
	//s_instword="11111111110111111111000011101111";
	if(opcode=="0100011"){
		ALU++;
		data_transfer++;
		 // S instruction  store inst
		s_imm2 = s_instword.substr(0,7);
		s_imm1 = s_instword.substr(20,5);
		s_rs2 = s_instword.substr(7,5);
		s_rs1 = s_instword.substr(12,5);
		funct3 = s_instword.substr(17,3);
		bitset<sizeof(int)*8> ra(s_rs1);
		iRA = ra.to_ulong();
		s_imm = s_imm2 + s_imm1;
		string s= s_imm.substr(0,1);
		bitset<sizeof(int) * 8> rimm(s_imm.substr(1,11));
		bitset<sizeof(int) * 8> rimm2(s);
		int tempimm = rimm.to_ulong();
		int tempimm2 =rimm2.to_ulong();
		iRB = tempimm - tempimm2*pow(2, 11);
		//cout<<"iRA"<<iRA<<endl;
	//	cout<<"iRB"<<iRB<<endl;
		RA= R[iRA];
		RB=iRB; 
		valid_rs1=1;
		//cout<<"imm2"<<imm2<<endl;
		//cout<<"imm1"<<imm1<<endl;
		//cout<<"rs2"<<rs2<<endl;
		//cout<<"rs1"<<rs1<<endl;
		//cout<<"func3"<<func3<<endl;
	}
	if(opcode=="1100011"){// SB instruction  branch inst
	ALU++;
	data_transfer++;
	num_inst_cntrl++;
		s_imm2 = s_instword.substr(0,7);
		s_imm1 = s_instword.substr(20,5);
		s_rs2 = s_instword.substr(7,5);
		s_rs1 = s_instword.substr(12,5);
		funct3 = s_instword.substr(17,3);
		bitset<sizeof(int)*8> ra(s_rs1);
		iRA = ra.to_ulong();
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
		//cout<<s_imm<<"YE HAI"<<endl;
		
		bitset<sizeof(int)*8> rb(s_rs2);
		iRB = rb.to_ulong();
		//cout<<"iRA"<<iRA<<endl;
		//cout<<"iRB"<<iRB<<endl;
		RA = R[iRA];
		RB = R[iRB];
		//cout<<"ra"<<RA<<endl;
		//cout<<"rb"<<RB<<endl;
		
		string s= s_imm.substr(0,1);
		bitset<sizeof(int) * 8> rimm(s_imm.substr(1,12));
		bitset<sizeof(int) * 8> rimm2(s);
		int tempimm = rimm.to_ulong();
		int tempimm2 =rimm2.to_ulong();
		jump_imm_offset = tempimm - tempimm2*pow(2, 12);
		//cout<<"JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"<<jump_imm_offset<<endl;
		valid_rs1=1;
		valid_rs2=1;
		//cout<<"imm2"<<imm2<<endl;
		//cout<<"imm1"<<imm1<<endl;
		//cout<<"rs2"<<rs2<<endl;
		//cout<<"rs1"<<rs1<<endl;
		//cout<<"func3"<<func3<<endl;
	}
	if(opcode=="1101111"){// UJ instruction   jal inst
		s_imm = s_instword.substr(0,20);
		s_rd = s_instword.substr(20,5);
	num_inst_cntrl++;

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
		//cout<<"jump_offset:"<<jump_imm_offset<<endl;
		//cout<<"iRA"<<iRA<<endl;
		//cout<<"iRB"<<iRB<<endl;
		
		//cout<<"imm"<<imm<<endl;
		//cout<<"rd"<<rd<<endl;
		
	}
	
	
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<funct3<<endl;
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<funct7<<endl;
	// bitset<sizeof(int) * 8> f3(funct3);
	// t3 = f3.to_ulong();
	// //cout <ifunc< "3333333333333333333333333"<<f3.to_ulong() <<ifunct3<< '\n';
	// bitset<sizeof(int) * 8> f7(funct7);
	// ifunct7 = f7.to_ulong();
	// //cout << "77777777777777777777777777777"<<f7.to_ulong() <<"77"<<ifunct7<< '\n';
	
	
	
	
	if(opcode=="0110011")
	{
		if(funct3=="000"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is ADD, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		
		if(funct3=="000"&& funct7=="0100000")
		{
			cout<<"DECODE: Operation is SUB, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		
		if(funct3=="001"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is SLL, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
    		
		if(funct3=="010"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is SLT, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="011"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is SLTU, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="100"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is XOR, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="101"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is SRL, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="101"&& funct7=="0100000")
		{
			cout<<"DECODE: Operation is SRA, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="110"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is OR, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="111"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is AND, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="000"&& funct7=="0000001")
		{
			cout<<"DECODE: Operation is MUL, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
	}
	
	if(opcode=="0111011")
	{
		if(funct3=="000"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is ANDW, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="000"&& funct7=="0100000")
		{
			cout<<"DECODE: Operation is SUBW, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="001"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is SLLW, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="101"&& funct7=="0000000")
		{
			cout<<"DECODE: Operation is SRLW, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="101"&& funct7=="0100000")
		{
			cout<<"DECODE: Operation is SRAW, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
	}
	
	if(opcode=="0000011")
	{
		if(funct3=="000")
		{
			cout<<"DECODE: Operation is lb, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="001")
		{
			cout<<"DECODE: Operation is lh, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="010")
		{
			cout<<"DECODE: Operation is lw, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="011")
		{
			cout<<"DECODE: Operation is ld, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="100")
		{
			cout<<"DECODE: Operation is lbu, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="101")
		{
			cout<<"DECODE: Operation is lhu, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		if(funct3=="110")
		{
			cout<<"DECODE: Operation is lwu, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   R"<<iRB<<"="<<R[iRB]<<endl;
		}
		
	}
	if(opcode=="0010011")//addi
	{
		//cout<<"7"<<funct3<<"7"<<endl;
		//cout<<"hhhhhhhhhhhhhhhhhhhhhhlllllllllllllllllllllllllllllllllllllll---------"<<endl;
		if(funct3=="000")
		{
			//cout<<"fffffffffffffffffffffffffffffffffffffffffff---------------"<<endl;
			cout<<"DECODE: Operation is addi, first operand R"<<RA<<" Second operand R"<<RB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="001"&&funct7=="0000000")
		{
			cout<<"DECODE: Operation is slli, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="010")
		{
			cout<<"DECODE: Operation is slti, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="011")
		{
			cout<<"DECODE: Operation is sltiu, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="100")
		{
			cout<<"DECODE: Operation is xori, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="101"&&funct7=="0000000")
		{
			cout<<"DECODE: Operation is srli, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="101"&&funct7=="0100000")
		{
			cout<<"DECODE: Operation is srai, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="110")
		{
			cout<<"DECODE: Operation is ori, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
		if(funct3=="111")
		{
			cout<<"DECODE: Operation is andi, first operand R"<<iRA<<" Second operand "<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<RB<<endl;
		}
	}
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
	if(opcode=="0011011")
	{
		if(funct3=="000")
		{
			cout<<"DECODE: Operation is addiw, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		if(funct3=="001"&&funct7=="0000000")
		{
			cout<<"DECODE: Operation is slliw, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		if(funct3=="101"&&funct7=="0000000")
		{
			cout<<"DECODE: Operation is srliw, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		if(funct3=="101"&&funct7=="0100000")
		{
			cout<<"DECODE: Operation is sraiw, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		if(funct3=="011")
		{
			cout<<"DECODE: Operation is sd, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		
	}
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
	if(opcode=="0100011")
	{
		if(funct3=="011")
		{
			cout<<"DECODE: Operation is sd, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
	}
	if(opcode=="1100111")
	{
		if(funct3=="000")
		{
			cout<<"DECODE: Operation is jalr, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		
	}
	if(opcode=="0010111")
	{
			cout<<"DECODE: Operation is auipc, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
	}
	if(opcode=="0110111")
	{
			cout<<"DECODE: Operation is lui, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
	}
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
	if(opcode=="0100011")
	{
		if(funct3=="000")
		{
			cout<<"DECODE: Operation is sb, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		if(funct3=="001")
		{
			cout<<"DECODE: Operation is sh, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
		if(funct3=="010")
		{
			cout<<"DECODE: Operation is sw, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
		}
	}
	if(opcode=="1100011")
	{
		if(funct3=="000")
		{
			cout<<"DECODE: Operation is beq, first operand R"<<iRA<<" Second operand R"<<iRB<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<RA<<"   R"<<iRB<<"="<<RB<<endl;
		}
		if(funct3=="001")
		{
			cout<<"DECODE: Operation is bne, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		//cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
			cout<<"Read registers R"<<iRA<<"="<<RA<<"   R"<<iRB<<"="<<RB<<endl;
		}
		if(funct3=="100")
		{
			cout<<"DECODE: Operation is blt, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		//cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
			cout<<"Read registers R"<<iRA<<"="<<RA<<"   R"<<iRB<<"="<<RB<<endl;
		}
		if(funct3=="101")
		{
			cout<<"DECODE: Operation is bge, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		//cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
			cout<<"Read registers R"<<iRA<<"="<<RA<<"   R"<<iRB<<"="<<RB<<endl;
		}
		if(funct3=="110")
		{
			cout<<"DECODE: Operation is bltu, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		//cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
			cout<<"Read registers R"<<iRA<<"="<<RA<<"   R"<<iRB<<"="<<RB<<endl;
		}
		if(funct3=="111")
		{
			cout<<"DECODE: Operation is bgeu, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		//cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<iRB<<endl;
			cout<<"Read registers R"<<iRA<<"="<<RA<<"   R"<<iRB<<"="<<RB<<endl;
		}
	}
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
	if(opcode=="1101111")
	{
		cout<<"DECODE: Operation is jal, first operand R"<<iRA<<" Second operand R"<<iRB<< " destination register R"<<iRd<<endl;
        		cout<<"Read registers R"<<iRA<<"="<<R[iRA]<<"   immediate= "<<jump_imm_offset<<endl;
	}	
	//hazard_cntrl= Hazard_detection_unit();

	if(opcode=="0010111"||opcode=="0110111"||opcode=="1101111")
	{
		if(opcode=="1101111")
		{
			PCSRC=1;
			cout<<"No operation in ALU"<<endl;
		}
	
	}	
	int hazard=0;
	main_control mc( funct3, funct7,opcode);
//cout<<"hell$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
	//--------------forwarding unit------------------------------------------------
//	cout<<"B3.Rd_address:  "<<B3.Rd_address<<"^^^^^^"<<" s_rs1 "<<s_rs1<<"  s_rs2 "<<s_rs2<<endl;
	string test_s_rd=B3.Rd_address;
	int test_iRd;
	bitset<sizeof(int) * 8> r_d(test_s_rd);
	test_iRd = r_d.to_ulong();
	string test_s_rd2=B4.iRd;
	int test_iRd2;
	bitset<sizeof(int) * 8> r_d2(test_s_rd2);
	test_iRd2 = r_d2.to_ulong();
	if(pipelining_knob==1)
	{
	if(test_iRd!=0 && B3.mem_Read!=1 && ((valid_rs1==1 && s_rs1==test_s_rd)||(valid_rs2==1 && s_rs2==test_s_rd)) && clock_cycle>=3 && data_knob)
	{
		num_data_hazards++;
		//cout<<"*******************"<<B2.mem_write<<endl;
		if(B2.mem_write==1)
		{
			//cout<<"In mem_write"<<endl;
			if(s_rs1==test_s_rd)
				RA=B3.RZ;	
		}
		else if(B2.mem_write!=1)
		{
			if(s_rs1==test_s_rd)
				RA=B3.RZ;
			if(s_rs2==test_s_rd)
				RB=B3.RZ;	
		}
	}
	else if (test_iRd!=0 && B3.mem_Read==1 && ((valid_rs1==1 && s_rs1==test_s_rd)||(valid_rs2==1 && s_rs2==test_s_rd)) && clock_cycle>=3 && data_knob)
	{
		num_data_hazards++;
		if(B2.mem_write!=1)
		{
			stall_data++;
		//	cout<<"In Hazard detection unit"<<endl;
			hazard=Hazard_detection_unit();
		}
	}
	else if (test_iRd2!=0 && ((valid_rs1==1 && s_rs1==test_s_rd2)||(valid_rs2==1 && s_rs2==test_s_rd2)) && clock_cycle>=4 &&data_knob)
	{
		num_data_hazards++;
		if(B2.mem_write!=1)
		{
			if(s_rs1==test_s_rd2)
				RA=B4.RY;
			if(s_rs2==test_s_rd2)
				RB=B4.RY;

		}
		else if(B2.mem_write==1)
		{
			if(s_rs1==test_s_rd2)
				RA=B4.RY;
		}
	}
	
	if(data_knob==0)
	{
		
		if(test_iRd!=0  && ((valid_rs1==1 && s_rs1==test_s_rd)||(valid_rs2==1 && s_rs2==test_s_rd)) && clock_cycle>=3)
		{
			num_data_hazards++;
			stall_data++;
			flg=1;
			hazard=Hazard_detection_unit();
		}
		else if(test_iRd2!=0 && ((valid_rs1==1 && s_rs1==test_s_rd2)||(valid_rs2==1 && s_rs2==test_s_rd2))&& clock_cycle>=4)
		{
			num_data_hazards++;
			stall_data++;
			flg=1;
			hazard=Hazard_detection_unit();
		}
	}
	}





//cout<<"dude$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
	if(flg==0)
	{
		if(opcode=="1100111")//&&funct3=="000")//jalr
		{
			//comp=2;
			flush_inst=0;
		}
		if(opcode=="1101111")//jal
		{
			//comp=1;
			flush_inst=0;
		}
		if(opcode=="1100011"&&funct3=="000")
    	{  
            cout<<"BEQ"<<"  RA:"<<RA<<" RB: "<<RB<<endl;
            if((RA-RB)==0)
            {
				//taking always taken;
				flush_inst=0;
            	// PCSRC=1;
                // ZERO=1;
            }
			else
			flush_inst=1;    
		}  
      if(opcode=="1100011"&&funct3=="001")
        {
            cout<<"BNE"<<"  RA:"<<RA<<" RB: "<<RB<<endl;
            if(RA!=RB)
            {
				flush_inst=0;	
            }
			else
			flush_inst=1;	
        }
       
       if(opcode=="1100011"&&funct3=="100")
        {
	    cout<<"BLT"<<"  RA:"<<RA<<" RB: "<<RB<<endl;
            if(RA<RB)
            {
            if(predict==1)
				flush_inst=0;
            }
			else
			flush_inst=1;
            
        }
       
       if(opcode=="1100011"&&funct3=="101")
        {
	    cout<<"BGE"<<"  RA:"<<RA<<" RB: "<<RB<<endl;
            if(RA>=RB)
            {
				flush_inst=0;
            }
			else
			flush_inst=1;
        } 
        }  
		//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<"RA:"<<RA<<"RB:"<<RB<<endl;
		if(hazard!=1)
		{
			if(opcode=="1100111")//jalr
			{
				unsigned int new_offset=RA+RB;
				PC_count=new_offset;
			}
			B2.iRA=s_rs1;
			B2.iRM=iRM;
			B2.iRd=s_rd;
			B2.iRB=s_rs2;
			B2.RA=RA;
			B2.RB=RB;
			//cout<<"Decode######################   B2.RA ########"<<B2.RA<<" B2.RB     "<<B2.RB<<endl;
			//cout<<"@@@@@  RA: "<<RA<<"   @@@@@@@@@   RB: "<<RB<<endl;
			B2.opcode=opcode;
			B2.func3=funct3;
			B2.inst_wrd=instruction_word;	
			B2.PC=B1.PC;					
		}  
		if(pipelining_knob==0)
		{
			if(flush_inst==1)
			{
				PC_count=B1.PC+4;
				flush_inst=0;
			}
		}
}
int  Hazard_detection_unit()
{
	{
		//cout<<"BRO I aM in hazard detection unit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
		B2.iRd="";
		B2.reg_write=0;
		B2.mem_Read=0;
		B2.mem_write=0;
		B2.inst_wrd=0;
		B2.PC=B1.PC;
		PC_count=B1.PC;
		return 1;
	}
	return 0;
}
