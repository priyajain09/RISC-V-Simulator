
/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* myRISCVSim.cpp
   Purpose of this file: implementation file for myRISCVSim
*/
#include<bits/stdc++.h>
#include "myARMSim.h"
#include"execute.hpp"
#include <cstdlib>
#include <cstdio>
#include<string>
#include"buffers.hpp"
using namespace std;
/////////////////////////////////////// updated++++++++++++++++++++++++++

int cold_missI;
int capacity_missI;
int conflict_missI;
int hitsI;
int cold_missD;
int capacity_missD;
int conflict_missD;
int hitsD;
int noSets;

//memory
 unsigned char MEM[4096];
 unsigned char cacheI[4096];
 unsigned char cacheD[4096];
  int tagI[4096];
  int tagD[4096];
 bool valid_bitI[4096];
 bool valid_bitD[4096];
 unsigned int LRUI[4096];
 unsigned int LRUD[4096];
 
 //input 
 int cachesizeI;
 int blocksize;
 int cachetype;
 int cachesizeD;
 
//-------------------------------------------Priya meghana
int ni=0;
int c=0;
int stall=0;
int contrl_hazards=0;
int stall_data=0;
int mis_predictions=0;
int num_data_hazards=0;
 int total_num_exe_inst=0;
 int data_knob=0;
//Register file
extern  unsigned int R[32];
int num_instructions=0;
extern int prev_PC;
//flags
int N,C,V,Z;
int ry, RZ;
//memory
 
 unsigned int operand2;


unsigned int R[32];

int predict=1;//intitilaised here;

int flush_inst=0;//''''
int hazard_cntrl=0;
int comp=0;
int global_PC=0;
int data_transfer=0;
int ALU=0;
//int data_transfer=0;
int num_inst_cntrl=0;
int pipelining_knob=0;

//intermediate datapath and control path signals
 //unsigned int instruction_word;
  unsigned int operand1;
 signed int jump_imm_offset=0;
 unsigned int PCSRC;
 //extern unsigned int PC_count=0;
 unsigned int PC_add;
 unsigned int return_add;
 int clock_cycle=1;
 unsigned int PC_count=0;
inter_stage_buffer_ID_EX B2;
 inter_stage_buffer_EX_MEM B3;
 inter_stage_buffer_IF_ID B1;

 inter_stage_buffer_MEM_WB B4;
//  int ifunct3;
//  int ifunct7;
//  int iopcode;
//  string funct3;
//  string funct7;
//  string opcode;
 //int iRA;
 //int iRB;
 //int iRd;
 //int iRM;
 //int RA;
 //int RB;
 string operation;
int read_byte(unsigned char *mem, unsigned int address);
int read_hw(unsigned char *mem, unsigned int address);
int read_dw(unsigned char *mem, unsigned int address);
void write_byte(unsigned char *mem, unsigned int address, uint8_t data);
void write_hw(unsigned char *mem, unsigned int address, uint16_t data);
void write_dw(unsigned char *mem, unsigned int address, unsigned long long int data);
int num_inst=0;

int binstr_to_dec(string x)
{
	int sum=0;
	//cout<<"x:"<<x<<endl;
	for(int i=4;i>=0;i--)
	{
		int c=x[i];
		sum=sum+(c-48)*pow(2,4-i);
	}
	//cout<<"sum:"<<sum<<endl;
	return sum;
}

int binstr_to_decBigEndian(string x)
{
	int sum=0;
	for(int i=0;i<=3;i++)
	{
		int c=x[i];
		sum=sum+(c-48)*pow(2,3-i);
	}
	return sum;
}

string bin_to_hex(string mac_bin)
{
	string mac_code;
	for(int i=0;i<32;i+=4)
	{
		string x=mac_bin.substr(i,4);
		stringstream ss(x);
		int a=binstr_to_dec(x);
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

 
 void run_RISCVsim()
{

	cout<<"enter \n1.pipelining implementation\n2.without pipelining<<endl";
	cout<<"\n Pipelining knob: ";
	cin>>pipelining_knob;
	cout<<"\n\n Data Forwarding Knob:";
	
	cin>>data_knob;
	int index=0;
	if(pipelining_knob==1)
	{
		cout<<"DO YOU WANT REGISTER VALUES TO BE DISPALYED  at the end of each cock cycle \n  1.yes\n2.no  ::"<<endl;
			int q;
			cin>>q;
			cout<<"DO YOU WANT TO KNOW THE INFORMATION IN THE PIPELINE REGISTERS    \n 1.YES \n2.NO"<<endl;
			int s;
			cin>>s;
		
		while(1) 
		{
			//cout<<"clock_cycle$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    :"<<clock_cycle<<endl;
			//cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% R[5] : "<<R[5]<<endl;
			//cout<<"B1.RA
			// if(clock_cycle>25)
			// break;
		
		cout<<"values in buffer1"<<endl;
		cout<<"B1.instruc_from_mem:"<<B1.instruc_from_mem;
		cout<<"B1.PC:"<<B1.PC<<endl;;
		cout<<"\n\n"<<endl;
		cout<<"values in buffer2"<<endl;
		cout<<"B2.RA"<<B2.RA;//from IF/ID
	// int RB;//from IF/ID
	// string iRA;//from regfile
	// string iRB;//from regfile
    // int iRM;
    // int reg_write;
    // string iRd;  //address of rd
	// int MeM_to_reg;
	// int branch;
	// int mem_Read;
	// int mem_write;
    // string alu_cntrl_sgnl;
    // string opcode;
    // unsigned int PC;
    // string func3;
    // unsigned int  inst_wrd;

			// for(int i=0; i<32 ; i++)
		//	{
	//	cout<<"R["<<i<<"] = "<<R[i]<<"\t";
		//printf("intitalise hua");
		
	//		}


	//		 for(int j=0; j<4096; j++)
	//		{
	//			if(MEM[j]!=0)
	//				printf("MEM [%d] = %x		",j,MEM[j]);
	//		}
	cout<<endl;
	
		//	for(int i=0;i<)
	
	cout<<endl<<endl;
			if(clock_cycle>=5)
			write_back();
			if(clock_cycle>=4)
			mem();
			if(clock_cycle>=3)
			execute();
			if(clock_cycle>=2)
			decode();
			if(clock_cycle>=1)
    	    fetch();
			cout<<"CLOCK CYCLE              ::::"<<clock_cycle<<endl;
			
			if(q==1)
			{
				for(int i=0; i<32 ; i++)
				{
				cout<<"R["<<i<<"] = "<<R[i]<<"\t";
				//printf("intitalise hua");
				}
				cout<<endl<<endl;
			}
			
			if(s==1)
			{
				cout<<"VALUES FROM B1 REGISTERS     ::"<<endl;
                cout<<"B1.instruc_from_mem         ::"<<B1.instruc_from_mem<<endl;
                cout<<"B1.PC                  ::"<< B1.PC<<endl<<endl<<endl;
               
                cout<<"VALUES FROM B2 REGISTERS     ::"<<endl;
                cout<<" B2.RA                       :: "<<B2.RA<<endl;//from IF/ID
                cout<<"B2.RB                        :: "<<B2.RB<<endl;;//from IF/ID
                cout<<"B2.iRA                      ::"<<B2.iRA<<endl;;//from regfile
                cout<<"B2.iRB          :"<<B2.iRB<<endl;;//from regfile
    cout<<"B2.iRM            :"<<B2.iRM<<endl;
    cout<<"B2.reg_write      :"<<B2.reg_write<<endl;;
    cout<<"B2.iRd             :"<<B2.iRd<<endl;  //address of rd
    cout<<"B2.MeM_to_reg       :"<<B2.MeM_to_reg<<endl;
    //branch;
    cout<<"B2.mem_Read           :"<<B2.mem_Read<<endl;
    cout<<"B2.mem_write          :"<<B2.mem_write<<endl;
    //string alu_cntrl_sgnl;
    //string opcode;
    cout<<"B2.PC                   :"<<B2.PC<<endl;
    //string func3;
    //int hazard_found;
    cout<<"B2.inst_wrd             :"<<B2.inst_wrd<<endl<<endl<<endl;
    cout<<"    VALUES FROM B3 REGISTERS        ::"<<endl;
    cout<<"B3.RZ       :"<<B3.RZ<<endl;;
        cout<<"B3.PC          :"<<B3.PC<<endl;
       cout<<"B3.Rd_address         :"<<B3.Rd_address<<endl;;
   // string opcode;
    //int hazard_found;
    //string func3;
    cout<<"B3.instruction_wrd           :"<<B3.instruction_wrd<<endl;
    cout<<"B3.RB                        :"<<B3.RB<<endl;
    cout<<"B3.reg_write                 :"<<B3.reg_write<<endl;;
    cout<<"B3.mem_Read                  :"<<B3.mem_Read<<endl;
    cout<<"B3.mem_write                :"<< B3.mem_write<<endl;
cout<<"VALUES FROM B4 REGISTER             :"<<endl;
    cout<<"B4.RY        :"<<B4.RY<<endl;
   // string opcode;
    //string func3;
    cout<<"B4.instruction_wrd       :"<<B4.instruction_wrd<<endl;
    cout<<"B4.RB                    :"<<B4.RB<<endl ;
    //int hazard_found;
    cout<<"B4.reg_write              :" <<B4.reg_write<<endl;
   cout<<"B4.iRd                     :"<<B4.iRd<<endl;
				

			}
			clock_cycle++;
   //cout<<"oooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"<<opcode<<endl;
  			if(N==1)
   			{	
				//    cout<<"i am out"<<endl;
	  			// break;
				write_back();
				mem();
				execute();
				if(q==1)
			{
				for(int i=0; i<32 ; i++)
				{
				cout<<"R["<<i<<"] = "<<R[i]<<"\t";
				//printf("intitalise hua");
				}
				cout<<endl<<endl;
			}
				clock_cycle++;
				write_back();
				mem();
				if(q==1)
			{
				for(int i=0; i<32 ; i++)
				{
				cout<<"R["<<i<<"] = "<<R[i]<<"\t";
				//printf("intitalise hua");
				}
				cout<<endl<<endl;
			}
				clock_cycle++;
				write_back();
				if(q==1)
			{
				for(int i=0; i<32 ; i++)
				{
				cout<<"R["<<i<<"] = "<<R[i]<<"\t";
				//printf("intitalise hua");
				}
				cout<<endl<<endl;
			}
				clock_cycle++;
				break;
				
   			}

			
	//ni++;
  		}
  		for(int i=0; i<32 ; i++)
	{
		cout<<"R["<<i<<"] = "<<R[i]<<"\t";
		//printf("intitalise hua");
		
	}
	
	cout<<endl<<endl;
  
  for(int j=0; j<4096; j++)
	{
		if(MEM[j]!=0)
		printf("MEM [%d] = %x		",j,MEM[j]);
		
	}

		
		cout<<"TOTAL NUMBER OF CYCLES       ::"<<clock_cycle<<endl;
		  
		//  num_inst=clock_cycle-4+c;
			num_instructions=num_instructions-1;
		  cout<<"NUMBER OF INSTRUCTIONS       ::"<<(num_instructions)<<endl;
		   cout<<"TOTAL NUMBER OF EXECUTED INSTRUCTIONS        ::"<<total_num_exe_inst<<endl;
			float  x=clock_cycle/total_num_exe_inst;
			cout<<"CLOCK_CYCLE PER INSTRUCTION  ::"<<x<<endl;
			//int y=
			cout<<"NUMBER OF DATA TRANSFER INSTRUCTIONS     ::"<<data_transfer<<endl;
			cout<<"NUMBER OF ALU INSTRUCTIONS               ::"<<ALU<<endl;
			cout<<"NUMBER OF CONTROL INSTRUCTIONS            ::"<<num_inst_cntrl<<endl;
			cout<<"NUMBER OF STALLS              ::"<<contrl_hazards+stall_data<<endl;
			cout<<"NUMBER OF DATA HAZARDS        ::"<<num_data_hazards<<endl;
			cout<<"NUMBER OF CONTROL HAZARDS     ::"<<contrl_hazards<<endl;
			cout<<"NUMBER OF STALLS DUE TO CONTROL HAZARDS    ::"<<contrl_hazards<<endl;
			cout<<"NUMBER OF STALLS DUE TO DATA HAZARDS      ::"<<stall_data<<endl;
			cout<<"NUMBER OF MISPREDICTIONS                   ::"<<mis_predictions<<endl;
   			//PCSRC=0;
			cout<<endl;
			cout<<"CACHE STATS --------------"<<endl;
			cout<<endl;
			cout<<"total number of data hitsD: "<<hitsD<<endl;
			cout<<"total number of instruction hitsI: "<<hitsI<<endl;
			cout<<"total number of data cold missD: "<<cold_missD<<endl;
			cout<<"total number of data capacity missD: "<<capacity_missD<<endl;
			cout<<"total number of data conflict missD: "<<conflict_missD<<endl;
			cout<<"total number of instruction cold missI: "<<cold_missI<<endl;
			cout<<"total number of instruction capacity missI: "<<capacity_missI<<endl;
			cout<<"total number of instruction conflict missI: "<<conflict_missI<<endl;
	}
else{
	while(1)
	{
		fetch();
		if(N==1)
		{
			break;
		}
		decode();
		execute();
		mem();
		write_back();
		clock_cycle+=5;
	}
	for(int i=0; i<32 ; i++)
	{
		cout<<"R["<<i<<"] = "<<R[i]<<"\t";
		//printf("intitalise hua");
		
	}
	
	cout<<endl<<endl;
  
  for(int j=0; j<4096; j++)
	{
		if(MEM[j]!=0)
		printf("MEM [%d] = %x		",j,MEM[j]);
		
	}

	cout<<endl;
	cout<<"CACHE STATS --------------"<<endl;
	cout<<"total number of data hitsD: "<<hitsD<<endl;
	cout<<"total number of instruction hitsI: "<<hitsI<<endl;
	cout<<"total number of data cold missD: "<<cold_missD<<endl;
	cout<<"total number of data capacity missD: "<<capacity_missD<<endl;
	cout<<"total number of data conflict missD: "<<conflict_missD<<endl;
	cout<<"total number of instruction cold missI: "<<cold_missI<<endl;
	cout<<"total number of instruction capacity missI: "<<capacity_missI<<endl;
	cout<<"total number of instruction conflict missI: "<<conflict_missI<<endl;
}
	
  
  for(int i=0; i<32 ; i++)
	{
		cout<<"R["<<i<<"] = "<<R[i]<<"\t";
		//printf("intitalise hua");
		
	}
	
	cout<<endl<<endl;
  
  for(int j=0; j<4096; j++)
	{
		if(MEM[j]!=0)
		printf("MEM [%d] = %x		",j,MEM[j]);
		
	}
	cout<<endl;
	
  
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {
	for(int i=0; i<32 ; i++)
	{
		R[i]=0;
		//printf("intitalise hua");
		
	}
	for(int j=0; j<4096 ; j++)
	{
		MEM[j]='\0';
		
		cacheD[j]='\0';
		tagD[j]=-1;
		LRUD[j]=0;
		valid_bitD[j]=0;
		
		cacheI[j]='\0';
		tagI[j]=-1;
		LRUI[j]=0;
		valid_bitI[j]=0;
	}
		
	
	cold_missD=0;
	capacity_missD=0;
	conflict_missD=0;	
	hitsD=0;
	cold_missI=0;
	capacity_missI=0;
	conflict_missI=0;	
	hitsI=0;
}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {
  FILE *fp;
 
  unsigned int address, instruction;
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
	  num_instructions++;
	  //printf("%x       %x        ",address, instruction);
    write1_word(MEM, address, instruction);
  }
 // cout<<"NUMBER OF INSTRUCTIONS     ::"<<num_instructions<<endl;
  fclose(fp);
  for(int i=0; i<32 ; i++)
	{
		R[i]=0;
		if(i==2)
		{
			R[i]=4092;
		}
		//printf("intitalise hua");
		
	}
	for(int j=0; j<4096; j++)
	{
		//if(MEM[j]!=0)
		//printf("MEM [%d] = %d		",j,MEM[j]);
		
	}
	jump_imm_offset=0;
	PCSRC=0;
	PC_count=0;
	PC_add=0;
	N=0;
	cout<<"enter instruction cache size in bytes : "<<endl;
	cin>>cachesizeI;
	cout<<"enter data cache size in bytes: "<<endl;
	cin>>cachesizeD;
	cout<<"enter cache block size in bytes : "<<endl;
	cin>>blocksize;
	cout<<"enter cache mapping type: "<<endl<<"1. fully associative	    2. direct map      3. set associative"<<endl;
	cin>>cachetype;
	if(cachetype==3)
	{
		cout<<"enter number of sets: "<<endl;
		cin>>noSets;
	}
	//cout<<"$$$$$$$$$$$$$$$$$$$$$$$$  N  "<<N<<endl;
}
/*
//writes the data memory in "data_out.mem" file
void write_data_memory() {
  FILE *fp;
  unsigned int i;
  fp = fopen("data_out.mem", "w");
  if(fp == NULL) {
    printf("Error opening dataout.mem file for writing\n");
    return;
  }
  
  for(i=0; i < 4000; i = i+4){
    fprintf(fp, "%x %x\n", i, read_word(MEM, i));
  }
  fclose(fp);
}

//should be called when instruction is swi_exit
void swi_exit() {
  write_data_memory();
  exit(0);
}
*/

int PC_generator()
{
	//cout<<"in pc generator"<<endl;
	//cout<<"PCSRC "<<PCSRC<<endl;
	//return_add = PC_count+4;
	//cout<<return_add<<"****************************************************************************"<<endl;
	
	if(comp==2)   //jalr
	{
		PC_count = jump_imm_offset;
	}
	else if(comp==1)       //jal
	{
		PC_count = PC_count + jump_imm_offset ;
	}
	// else if(hazard_cntrl==1)   //]
	// {
	// 	cout<<"BROOOOOOOOOOOOOO my PC value:"<<PC_count<<endl;
	// 	PC_count=PC_count;
	// }
	else if(flush_inst==1)
	{
		PC_count=B2.PC+4;
	}
	else if(comp==3)   //branch   //always taken
	{
		PC_count+=jump_imm_offset;
		// if(predict==1)
		// {
		// 	PC_count = PC_count + jump_imm_offset;
		// }
		// if(predict==0)
		// {
		// 	PC_count = RZ;
		// }
	}
	else
	{
		PC_count+=4;
	}

//before change code
	/*if(PCSRC==1)
	{
		//cout<<"jjjjjjjjjjjjjjjjjjjj"<<jump_imm_offset<<endl;
		//cout<<"pppppppppppppppppppp"<<PC_count<<endl;
		
		//cout<<return_add<<"****************************************************************************"<<endl;
		PC_count = PC_count + jump_imm_offset - 4;
	}
	if(opcode=="1100111" && funct3=="000")
	{
		PC_count = RZ;
		//cout<<RZ<<endl;
	}*/
	
			
}


//executes the ALU operation based on ALUop
//void execute() {}
//perform the memory operation
//void mem() {}
//writes the results back to register file
//void write_back() {}


int read1_word(unsigned char *mem, unsigned int address) {
  int *data;
  data =  (int*) (mem + address);
  //cout<<"data++++++++"<<(*(data))<<endl;
  return *data;
}

void write1_word(unsigned char *mem, unsigned int address, unsigned int data) {
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data;
}
