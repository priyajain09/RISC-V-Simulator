#ifndef MAIN_CONTROL_HPP
#define MAIN_CONTROL_HPP
#include<iostream>
#include<string>
#include<cmath>
using namespace std;

class main_control
{
public:  
    int mem_write=0;
    int mem_read=0;
    int mem_to_reg=0;
    int br=0;
    int reg_write=0;
    string func_3="";
    string func_7="";
    string alu_cntrl_sgnl="";
    string alu_op="";
    string opcode="";
    main_control( string func_3, string func_7,string opcode);
    int Mem_write();
    int Mem_read();
    int regwrite();
    int MemtoReg();
    int branch();
    string ALU_Op();
    string ALU_Control();
};

#endif
