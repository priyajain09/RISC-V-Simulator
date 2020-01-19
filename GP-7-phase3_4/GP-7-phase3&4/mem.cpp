
#include<bits/stdc++.h>
#include "myARMSim.h"
#include <cstdlib>
#include <cstdio>
#include<string>
using namespace std;
extern inter_stage_buffer_EX_MEM B3;
extern inter_stage_buffer_MEM_WB B4;

int read_byte(unsigned char *mem, unsigned int address);
int read_hw(unsigned char *mem, unsigned int address);
int read_dw(unsigned char *mem, unsigned int address);
void write_byte(unsigned char *mem, unsigned int address, uint8_t data);
void write_hw(unsigned char *mem, unsigned int address, uint16_t data);
void write_dw(unsigned char *mem, unsigned int address, unsigned long long int data);

int read_word(unsigned char *mem, unsigned int address);
void write_word(unsigned char *mem, unsigned int address, unsigned int data);


int binstr_to_dec3(string x)
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

void mem() 
{	
	cout<<" IN MEM STage----------"<<endl;
	//cout<<"Hey ya i see you"<<endl;
	unsigned int instruction_word=B3.instruction_wrd;
	 bitset<32> instword(instruction_word);
	// //cout<<"++++++instword"<<instword<<endl;
	string s_instword = instword.to_string();
	// //cout<<s_instword<<endl;
	string opcode = B3.opcode;
	// //cout<<"opcode"<<opcode<<endl;
	string func3 = B3.func3;
	unsigned int RZ=B3.RZ;
	B4.iRd=B3.Rd_address;
	int ry;
	// //cout<<"func3"<<func3<<endl;
	int MeM_to_reg;
	int branch;
	int mem_Read;
	int mem_write;
	int alu_result;
	int RB_value;
	int Rd_address;
	
	string imm;
	string imm2;
	string imm1;
	string rs1;
	string rs2;
	string rd;
	ry=RZ;//change
	//cout<<"In mem RY: "<<ry<<endl;
//cout<<"hellllllllllooooooooo1"<<endl;
	if(opcode=="1101111"||(opcode=="1100111"&&func3=="000"))
	{
		cout<<"no operation in memory stage"<<endl;
		ry=B3.PC+4;
		//cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<ry<<endl;
		//return;
	}
	//cout<<"hellllllllllooooooooo2"<<endl;
	
	if(B3.mem_Read==1)     //load
	{
		//ry=RZ;
		 if(opcode=="0000011" && func3=="010")//for lw
		{
		//cout<<"------------------------------"<<endl;
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
		// //int iRs = binstr_to_dec3(rs1);
		// //int iImm = binstr_to_dec3(imm);
		// //int iAccess = R[iRs]+iImm;
		// //iAccess=32;
		//ry = read_word(MEM,RZ);
		ry = cache_mod(RZ, 1,0);
		cout<<"loading  from memory address at :"<<RZ<<"   value  :"<<ry<<endl;
		//cout<<ry<<"ry2"<<endl;
	//cout<<"helloq"<<endl;
	//	cout<<"hellllllllllooooooooo3"<<endl;
		}

		else if(opcode=="0000011" && func3=="000")//lb 
		{
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
		// //int iRs = binstr_to_dec3(rs1);
		// //int iImm = binstr_to_dec3(imm);
		// //int iAccess = R[iRs]+iImm;
		// //iAccess=32;
		ry = cache_mod(RZ,2,0);
		//ry = read_byte(MEM, RZ);
		cout<<"loading  from memory address at :" <<RZ<<endl;

		//cout<<ry<<endl;
	//cout<<"hellop"<<endl;

		}
		else if(opcode=="0000011" && func3=="001")//lhw
		{

		// //cout<<"------------------"<<endl;
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
		// //int iRs = binstr_to_dec3(rs1);
		// //int iImm = binstr_to_dec3(imm);
		// //int iAccess = R[iRs]+iImm;
		// //iAccess=32;
		//ry = read_hw(MEM,RZ);
		ry = cache_mod(RZ,3,0);
		cout<<"loading  from memory address at :"<<RZ<<endl;
		//cout<<ry<<endl;
	//cout<<"hellot"<<endl;

		}
		if(opcode=="0000011" && func3=="011")//ld
		{
		
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
		//int iRs = binstr_to_dec3(rs1);
		//int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs]+iImm;
		ry = cache_mod(RZ,4,0);
		//ry = read_dw(MEM, RZ);
		cout<<"loading  from memory address at :"<<RZ<<endl;
		//cout<<ry<<endl;
	//cout<<"helloe"<<endl;

		}
		else if(opcode=="0000011" && func3=="100")//lbu
		{
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
		//int iRs = binstr_to_dec3(rs1);
		//int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs]+iImm;
		//ry = read_byte(MEM, RZ);
		ry = cache_mod(RZ,2,0);
		cout<<"loading  from memory address at :"<<RZ<<endl;
		//cout<<ry<<endl;
	//cout<<"hellos"<<endl;

		}
		else if(opcode=="0000011" && func3=="101")//lhu
		{
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
	//	int iRs = binstr_to_dec3(rs1);
	//	int iImm = binstr_to_dec3(imm);
	//	int iAccess = R[iRs]+iImm;
		//ry = read_hw(MEM, RZ);
		ry = cache_mod(RZ,3,0);
		cout<<"loading  from memory address at :"<<RZ<<endl;
		//cout<<ry<<endl;
	//cout<<"hellom"<<endl;

		}
		else if(opcode=="0000011" && func3=="110")//for lwu
		{
		// imm = s_instword.substr(0,12);
		// rs1 = s_instword.substr(12,5);
		// rd = s_instword.substr(20,5);
		//int iRs = binstr_to_dec3(rs1);
		//int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs]+iImm;
		//ry = read_word(MEM, RZ);
		ry = cache_mod(RZ,1,0);
		cout<<"loading  from memory address at :"<<RZ<<endl;
		//cout<<ry<<"ry2"<<endl;
	//cout<<"hellon"<<endl;

		}	

	}
	else if(B3.mem_write==1)   ///store
	{//cout<<"hellllllllllooooooooo4"<<endl;
		 if(opcode=="0100011" && func3=="010")//for store operation sw
		{
		imm1 = s_instword.substr(0,7);
		imm2 = s_instword.substr(20,5);
		imm = imm1 + imm2;
		rs1 = s_instword.substr(12,5);
		rs2 = s_instword.substr(7,5);
		int iRs1 = binstr_to_dec3(rs1);
		int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs1]+iImm;
		//iAccess=0;
		int iRs2 = binstr_to_dec3(rs2);
		int daalo = R[iRs2];
		//daalo=6556;
		//MEM[iAccess]=34;
		//iAccess=32;
		//cout<<MEM[iAccess]<<endl;
		if(B4.iRd==rs2 && iRs2!=0)
		{
			daalo=B4.RY;
		}
		//write_word(MEM,RZ,daalo);
		int random = cache_mod(RZ,5,daalo);
		cout<<"storing  from memory address at :"<<RZ<<endl;
		//cout<<"helloending"<<MEM[iAccess]<<"i am back"<<endl;
		//printf("hi%dguru",MEM[iAccess]);
		//cout<<"hhhjjj"<<iAccess<<endl;
		//cout<<MEM[iAccess]<<endl;	
	//cout<<"helloa"<<endl;

		}
		if(opcode=="0100011" && func3=="011")//for store operation sd
		{
		imm = s_instword.substr(0,12);
		rs1 = s_instword.substr(12,5);
		rd = s_instword.substr(20,5);
		//int iRs1 = binstr_to_dec3(rs1);
		//int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs1]+iImm;
		int iRs2 = binstr_to_dec3(rd);
		long long int daalo = R[iRs2];
		//daalo=67;
		//MEM[iAccess]=34;
		//cout<<MEM[iAccess]<<endl;
		if(B4.iRd==rd && iRs2!=0)
		{
			daalo=B4.RY;
		}
		//write_dw(MEM,RZ,daalo);
		int random = cache_mod(RZ, 6, daalo);
		cout<<"storing  from memory address at :"<<RZ<<endl;
		//cout<<MEM[iAccess]<<endl;
	//cout<<"hellow"<<endl;

		}
		else if(opcode=="0100011" && func3=="000")//for store operation sb
		{
		imm = s_instword.substr(0,12);
		rs1 = s_instword.substr(12,5);
		rd = s_instword.substr(20,5);
		int iRs1 = binstr_to_dec3(rs1);
		int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs1]+iImm;
		//cout<<"iAccess"<<iAccess<<endl;
		
		int iRs2 = binstr_to_dec3(rd);
		//cout<<"rrrrrr2"<<iRs2<<endl;
		//cout<<"iw   "<<instruction_word;
		//cout<<"rd   "<<rd<<endl;
		uint8_t daalo = R[iRs2];
		//cout<<"uuuuuudddddddddddddddddddddddddddddddddddd"<<daalo<<endl;
		//daalo = 255;
	//	MEM[iAccess]=45;
		//cout<<MEM[iAccess]=34;
		//cout<<MEM[iAccess]<<endl;
		//cout<<"------------------------------"<<RZ<<endl;
		if(B4.iRd==rd && iRs2!=0)
		{
			daalo=B4.RY;
		}
		//write_byte(MEM,RZ,daalo);
		int random = cache_mod(RZ,5,daalo);
		cout<<"storing  from memory address at :"<<RZ<<endl;
		//cout<<MEM[iAccess]<<endl;
		//printf("ding%dding",MEM[iAccess]);	
	//cout<<"hellos"<<endl;

		}	
		else if(opcode=="0100011" && func3=="001")//for store operation sh
		{
		imm = s_instword.substr(0,12);
		rs1 = s_instword.substr(12,5);
		rd = s_instword.substr(20,5);
		int iRs1 = binstr_to_dec3(rs1);
		int iImm = binstr_to_dec3(imm);
		//int iAccess = R[iRs1]+iImm;
		int iRs2 = binstr_to_dec3(rd);
		uint16_t daalo = R[iRs2];
		//daalo = 258;
		//MEM[iAccess]=78;
		//cout<<MEM[iAccess]<<endl;
		if(B4.iRd==rd && iRs2!=0)
		{
			daalo=B4.RY;
		}
		//write_hw(MEM, RZ,daalo);
		int random = cache_mod(RZ,5,daalo);
		cout<<"storing  from memory address at :"<<RZ<<endl;	
		//cout<<MEM[iAccess]<<endl;
		//cout<<"hellox"<<endl;
		}

	}
	//cout<<"!!!!!!!!! RY: "<<ry<<endl;
	B4.RY=ry;
	B4.RB=B3.RB;
	//cout<<" in mem reg_write: "<<B3.reg_write<<endl;
	B4.reg_write=B3.reg_write;
	B4.opcode=B3.opcode;
	B4.instruction_wrd=B3.instruction_wrd;
	//cout<<"hellllllllllooooooooo5"<<endl;
//cout<<"at 2ndloading  from memory address at :" <<RZ<<"and the value :"<<ry<<endl;

}


int read_byte(unsigned char *mem, unsigned int address){
	char *data;
	data = (char*) (mem + address);
	int *val;
	val=(int*)data;
	//cout<<"data"<<*data<<endl;
	int x = (*val >> (8*0)) & 0xff;
	
	return x;
}

int read_hw(unsigned char *mem, unsigned int address){
	char *data;
	data = (char*) (mem + address);
	int *val;
	val=(int*)data;
	//cout<<"data"<<*data<<endl;
	int x = (*val >> (8*0)) & 0xffff;
	
	return x;
}

int read_dw(unsigned char *mem, unsigned int address) {
  int *data1,*data2,*data;
  data1 =  (int*) (mem + address);
  //cout<<"data"<<(*(data1))<<endl;
  address+=4;
  data2 = (int*) (mem + address);
  long long int mul = 4294967296;
  mul*=(*(data2));
  int first = *data1;
  int finzz = first+mul;
  //cout<<"lmao"<<endl;
  //cout<<"2"<<*(data2)<<endl;
  return finzz;
}

int read_word(unsigned char *mem, unsigned int address) {
  int *data;
  data =  (int*) (mem + address);
  int x=*data;
  bitset<32> d(x);
  //cout<<"d:"<<d<<endl;
  cout<<"data++++++++"<<(*(data))<<"______address:  "<<address<<endl;
  return *data;
}

void write_hw(unsigned char *mem, unsigned int address, uint16_t data) {
  uint16_t *data_p;
  data_p = (uint16_t*) (mem + address);
  *data_p = data;
  //printf("okie %d dokie",*data_p);
  return;
}

void write_byte(unsigned char *mem, unsigned int address, uint8_t data) {
  uint8_t *data_p;
  data_p = (uint8_t*) (mem + address);
  *data_p = data;
  //cout<<"heyy"<<*data_p<<"ding"<<endl;
  //printf("dingchak%ddinchak",*data_p);
  return;
}
//lomng long int
void write_dw(unsigned char *mem, unsigned int address, unsigned long long int data) {
  long long int data2,data1;
  long long int mul = 4294967296;
  data2 = data%mul;
  data1 = data/mul;
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data2;
  address-=4;
  data_p = (int*) (mem + address);
  *data_p = data1;
  return;
}

void write_word(unsigned char *mem, unsigned int address, unsigned int data) {
//cout<<"Meghana reddy"<<endl;
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data;
  //cout<<" in write_back ";
  //cout<<*(data_p)<<endl;
  //printf("%d",*data_p);
  //printf("%d\n",*(data_p));
  return;
}

