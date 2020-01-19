
#include<bits/stdc++.h>
#include "myARMSim.h"
#include <cstdlib>
#include <cstdio>
#include<math.h>
#include<string>
using namespace std;

int read3_byte(unsigned char *mem, unsigned int address);
int read3_hw(unsigned char *mem, unsigned int address);
int read3_dw(unsigned char *mem, unsigned int address);
void write3_byte(unsigned char *mem, unsigned int address, uint8_t data);
void write3_hw(unsigned char *mem, unsigned int address, uint16_t data);
void write3_dw(unsigned char *mem, unsigned int address, unsigned long long int data);

int read4_word(unsigned char *mem, unsigned int address);
void write4_word(unsigned char *mem, unsigned int address, unsigned int data);


int binstr_to_dec4(string x)
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

unsigned int cache_mod(unsigned int add, int type, int daalo)
{

	if(cachetype==1)
	{
		int store;
		int i;
		int maxLRU=0;
		int jIndexMaxLRU=-1;
		//instructions-------------------------------------------------------------
		if(type==0)
		{      // cout<<"type is zero"<<endl;         
			int total_blocks = cachesizeI/blocksize;
		//	cout<<"total blocks"<<total_blocks<<endl;
			int x = log2(cachesizeI);
			int y = log2(blocksize);
		//	cout<<"x"<<x<<endl;
		//	cout<<"y"<<y<<endl;
			bitset<32> mem_Add_bits(add);
		//	cout<<"mab"<<mem_Add_bits<<endl;
			string smem_Add_bits = mem_Add_bits.to_string();
			
			bitset<32> mab= mem_Add_bits>>y;
			mab= mab<<y;
			string smab = mab.to_string();        //memory address excluding word and byte offset
			
			int itag = 12-y;
			string tag_off = smem_Add_bits.substr(20,itag);
		//	cout<<"tagwaaaaa"<<tag_off<<endl;
			bitset<sizeof(int) * 8> toff(tag_off);
			int itoff = toff.to_ulong();
		//	cout<<"tag offset "<<itoff<<endl;
			int iStartWord=32-y;
			string word_off = smem_Add_bits.substr(iStartWord,y);
			bitset<sizeof(int) * 8> woff(word_off);
			int iwoff = woff.to_ulong();
			int initAdd = mab.to_ulong();
			int storeReturn;
		//	cout<<"word offset"<<iwoff<<endl;
			for(i=0; i<total_blocks; i++)
			{
				//cout<<"AKANSHA"<<tagI[i*blocksize]<<endl;
				//cout<<"ITOFF"<<itoff<<endl;
				if(tagI[i*blocksize]==itoff)
				{
		//			cout<<"tag matched"<<itoff<<endl;
					break;
				}
			}
			if(i<total_blocks)
			{
				hitsI++;
		//		cout<<"incremented hitsI"<<hitsI<<endl;
				store = read1_word(cacheI,i*blocksize+iwoff);
		//		cout<<"returning after hit"<<store<<endl;	
				for(int j=0;j<cachesizeI;j++)
				{
					if(LRUI[j]<LRUI[i*blocksize])
					{	
						LRUI[j]++;
					}
				}
				for(int j=i*blocksize; j<i*blocksize+blocksize; j++)
				{
					LRUI[j]=0;
				}
				
				return store; 
			}
			else                            //if not a hit
			{
				for(i=0; i<cachesizeI; i++)
				{
					if(tagI[i]==-1)
					{
						
						cold_missI++;
						//cout<<"tag i is -1 coldmiss"<<cold_missI<<endl;
						for(int j=0; j<blocksize;j++)
						{
						//	cout<<"initAdd+j"<<initAdd+j<<endl;
							store=read1_word(MEM, initAdd+j); //storing word in required loc in cache	
						//	cout<<"from memory"<<store<<endl;
							write1_word(cacheI,i+j,store);
						//	cout<<"just after storing"<<read1_word(cacheI,i+j)<<endl;
						//	cout<<"I+J"<<i+j<<endl;
							tagI[i+j]=itoff;valid_bitI[i+j]=1;
							tagI[i+1+j]=itoff;valid_bitI[i+1+j]=1;
							tagI[i+2+j]=itoff;valid_bitI[i+j+2]=1;
							tagI[i+3+j]=itoff;valid_bitI[j+i+3]=1;
						//	cout<<"tag"<<tagI[i+j]<<endl;
							j=j+3;
						}
						
						for(int j=0;j<cachesizeI;j++)
						{
							if(LRUI[j]<LRUI[i])
							{	
								LRUI[j]++;
							}
						}	
						for(int j=i; j<i+blocksize; j++)
						{
							LRUI[j]=0;
						}
						//cout<<"III"<<i<<endl;
						store = read1_word(cacheI,i+iwoff);
						//cout<<"printing stored word"<<store<<endl;
						return store;
					}
				}
				if(i>=cachesizeI)               // cache is full need to replace
				{
					
					cold_missI++;capacity_missI++;
					for(int j=0; j<cachesizeI; j++)
					{
						if(LRUI[j]>maxLRU)
						{
							jIndexMaxLRU = j;
							maxLRU = LRUI[j];
						}
					}
					i = jIndexMaxLRU;
					for(int j=0; j<blocksize;j++)
					{
						store=read1_word(MEM, initAdd+j);
						write1_word(cacheI,i+j,store);
						tagI[i+j]=itoff;valid_bitI[i+j]=1;
						tagI[i+1+j]=itoff;valid_bitI[i+1+j]=1;
						tagI[i+2+j]=itoff;valid_bitI[i+j+2]=1;
						tagI[i+3+j]=itoff;valid_bitI[j+i+3]=1;
						j=j+3;
					}
				
					for(int j=0;j<cachesizeI;j++)
					{
						if(LRUI[j]<LRUI[i])
						{	
							LRUI[j]++;
						}
					}
					for(int j=i; j<i+blocksize; j++)
					{
						LRUI[j]=0;
					}
				
					store = read1_word(cacheI,i+iwoff);
					return store;
				}
			}
		}
		if(type==1||type==2||type==3||type==4) //-------lw---------ld-------lhw-------lb--------
		{
			int total_blocks = cachesizeD/blocksize;

			int x = log2(cachesizeD);
			int y = log2(blocksize);
		
			bitset<32> mem_Add_bits(add);
			string smem_Add_bits = mem_Add_bits.to_string();
		
			bitset<32> mab= mem_Add_bits>>y;
			mab= mab<<y;
			string smab = mab.to_string();        //memory address excluding word and byte offset
		
			int itag = 12-y;
			string tag_off = smem_Add_bits.substr(20,itag);
			bitset<sizeof(int) * 8> toff(tag_off);
			int itoff = toff.to_ulong();
		
			int iStartWord=32-y;
			string word_off = smem_Add_bits.substr(iStartWord,y);
			bitset<sizeof(int) * 8> woff(word_off);
			int iwoff = woff.to_ulong();
			int initAdd = mab.to_ulong();
			int storeReturn;
			for(i=0; i<total_blocks; i++)
			{
				if(tagD[i*blocksize]==itoff)
				{
				break;
				}
			}
			if(i<total_blocks)
			{
				hitsD++;
				//store = read1_word(cacheD,i*blocksize+iwoff);
				if(type==1)					//lw
        			{   
            				store=read1_word(cacheD,i*blocksize+iwoff);
        			}
        			else if(type==2)
        			{
           			 	store=read3_byte(cacheD,i*blocksize+iwoff);
        			}
        			else if(type==3)
        			{
            				store=read3_hw(cacheD,i*blocksize+iwoff);
        			}
        			else if(type==4)
        			{
            				store=read3_dw(cacheD,i*blocksize+iwoff);
        			}
				for(int j=0;j<cachesizeD;j++)
				{
					if(LRUD[j]<LRUD[i*blocksize])
					{	
						LRUD[j]++;
					}
				}
				for(int j=i*blocksize; j<i*blocksize+blocksize; j++)
				{
					LRUD[j]=0;
				}
				return store; 
			}
			else                            //if not a hit
			{
				for(i=0; i<cachesizeD; i++)
				{
					if(tagD[i]==-1)
					{
						cold_missD++;
						for(int j=0; j<blocksize;j++)
						{
							store=read1_word(MEM, initAdd+j); //storing word in required loc in cache	
							write1_word(cacheD,i+j,store);
							tagD[i+j]=itoff;valid_bitD[i+j]=1;
							tagD[i+1+j]=itoff;valid_bitD[i+1+j]=1;
							tagD[i+2+j]=itoff;valid_bitD[i+j+2]=1;
							tagD[i+3+j]=itoff;valid_bitD[j+i+3]=1;
							j=j+3;
						}
					
						for(int j=0;j<cachesizeD;j++)
						{
							if(LRUD[j]<LRUD[i])
							{	
								LRUD[j]++;
							}
						}
						for(int j=i; j<i+blocksize; j++)
						{
							LRUD[j]=0;
						}
				
					//store = read1_word(cacheI,i*blocksize+iwoff);
						if(type==1)					//lw
        					{   
            						store=read1_word(cacheD,i+iwoff);
        					}
        					else if(type==2)
        					{
           			 			store=read3_byte(cacheD,i+iwoff);
        					}
        					else if(type==3)
        					{
            						store=read3_hw(cacheD,i+iwoff);
        					}
        					else if(type==4)
        					{
            						store=read3_dw(cacheD,i+iwoff);
        					}
						return store;
					}
				}
				if(i>=cachesizeD)               // cache is full need to replace
				{
						
					cold_missD++;capacity_missD++;
					for(int j=0; j<cachesizeD; j++)
					{
						if(LRUD[j]>maxLRU)
						{
							jIndexMaxLRU = j;
							maxLRU = LRUD[j];
						}
					}
					i = jIndexMaxLRU;
					for(int j=0; j<blocksize;j++)
					{
						store=read1_word(MEM, initAdd+j);
						write1_word(cacheD,i+j,store);
						tagD[i+j]=itoff;valid_bitD[i+j]=1;
						tagD[i+1+j]=itoff;valid_bitD[i+1+j]=1;
						tagD[i+2+j]=itoff;valid_bitD[i+j+2]=1;
						tagD[i+3+j]=itoff;valid_bitD[j+i+3]=1;
						j=j+3;
					}
					
					for(int j=0;j<cachesizeD;j++)
					{
						if(LRUD[j]<LRUD[i])
						{	
							LRUD[j]++;
						}
					}
					for(int j=i;j<i+blocksize; j++)
					{
						LRUD[j]=0;
					}
				
				//store = read1_word(cacheI,i*blocksize+iwoff);
					if(type==1)					//lw
        				{   
            					store=read1_word(cacheD,i+iwoff);
        				}
        				else if(type==2)
        				{
           				 	store=read3_byte(cacheD,i+iwoff);
        				}
        				else if(type==3)
        				{
            					store=read3_hw(cacheD,i+iwoff);
        				}
        				else if(type==4)
        				{
            					store=read3_dw(cacheD,i+iwoff);
        				}
				return store;
				}
			}
		}	

	//-------------------------sw==5;sd==6-----------------------------------------
		else if(type==5||type==6)
		{	
		
			int total_blocks = cachesizeD/blocksize;

			int x = log2(cachesizeD);
			int y = log2(blocksize);
			
			bitset<32> mem_Add_bits(add);
			string smem_Add_bits = mem_Add_bits.to_string();
			
			bitset<32> mab= mem_Add_bits>>y;
			mab= mab<<y;
			string smab = mab.to_string();        //memory address excluding word and byte offset
			
			int itag = 12-y;
			string tag_off = smem_Add_bits.substr(20,itag);
			bitset<sizeof(int) * 8> toff(tag_off);
			int itoff = toff.to_ulong();
		
			int iStartWord=32-y;
			string word_off = smem_Add_bits.substr(iStartWord,y);
			bitset<sizeof(int) * 8> woff(word_off);
			int iwoff = woff.to_ulong();
			int initAdd = mab.to_ulong();
			int storeReturn;
			for(i=0; i<total_blocks; i++)
			{
				if(tagD[i*blocksize]==itoff)
				{
					break;
				}
			}
			if(i<total_blocks)
			{
				hitsD++;
				if(type==5)					//lw
	        		{   
	            			write4_word(cacheD,i*blocksize+iwoff, daalo);
	            			write4_word(MEM, add, daalo);
	        		}
	        		if(type==6)					//lw
	        		{   
	            			write3_dw(cacheD,i*blocksize+iwoff, daalo);
	            			write3_dw(MEM, add, daalo);
	        		}
				for(int j=0;j<cachesizeD;j++)
				{
					if(LRUD[j]<LRUD[i*blocksize])
					{	
						LRUD[j]++;
					}
				}
				for(int j=i*blocksize; j<i*blocksize+blocksize; j++)
				{
					LRUD[j]=0;
				}
				
			return 0; 
			}
			else
			{
				for(i=0; i<cachesizeD; i++)
				{
					if(tagD[i]==-1)
					{
						cold_missD++;
						for(int j=0; j<blocksize;j++)
						{
							store=read1_word(MEM, initAdd+j); //storing word in required loc in cache
							write1_word(cacheD,i+j,store);
							tagD[i+j]=itoff;valid_bitD[i+j]=1;
							tagD[i+1+j]=itoff;valid_bitD[i+1+j]=1;
							tagD[i+2+j]=itoff;valid_bitD[i+j+2]=1;
							tagD[i+3+j]=itoff;valid_bitD[j+i+3]=1;
							
							j=j+3;
						}
					
						for(int j=0;j<cachesizeD;j++)
						{
							if(LRUD[j]<LRUD[i])
							{	
								LRUD[j]++;
							}
						}
						for(int j=i; j<i+blocksize; j++)
						{
							LRUD[j]=0;
						}
				//store = read1_word(cacheI,i*blocksize+iwoff);
						if(type==5)				
        					{   
            						write4_word(cacheD,i+iwoff, daalo);
            						write4_word(MEM, add, daalo);
        					}
        					if(type==6)					
        					{   
            						write3_dw(cacheD,i+iwoff, daalo);
            						write3_dw(MEM, add, daalo);
        					}
						return 0;
					}
				}
				if(i>=cachesizeD)               // cache is full need to replace
				{
					
					cold_missD++;capacity_missD++;
					for(int j=0; j<cachesizeD; j++)
					{
						if(LRUD[j]>maxLRU)
						{
							jIndexMaxLRU = j;
							maxLRU = LRUD[j];
						}
					}
					i = jIndexMaxLRU;
					for(int j=0; j<blocksize;j++)
					{
						store=read1_word(MEM, initAdd+j); //storing word in required loc in cache
						write1_word(cacheD,i+j,store);
						tagD[i+j]=itoff;valid_bitD[i+j]=1;
						tagD[i+1+j]=itoff;valid_bitD[i+1+j]=1;
						tagD[i+2+j]=itoff;valid_bitD[i+j+2]=1;
						tagD[i+3+j]=itoff;valid_bitD[j+i+3]=1;
							
						j=j+3;
					}
					
					for(int j=0;j<cachesizeD;j++)
					{
						if(LRUD[j]<LRUD[i])
						{	
							LRUD[j]++;
						}
					}
					for(int j=i; j<i+blocksize; j++)
					{
						LRUD[j]=0;
					}
				
					//store = read1_word(cacheI,i*blocksize+iwoff);
					if(type==5)				
        				{   
        	    				write4_word(cacheD,i+iwoff, daalo);
        	    				write4_word(MEM, add, daalo);
        				}
        				if(type==6)					
        				{   
       		    				write3_dw(cacheD,i+iwoff, daalo);
        	    				write3_dw(MEM, add, daalo);
        				}
				return 0;
				}	
			}
		}
	}
	

//-------------------------------directmap--------------------------------------------------
if(cachetype==2)
	{
		if(type==0)
	{
		int addDiv = add/blocksize;
		int modTo = cachesizeI/blocksize;
		int blockno = addDiv%modTo;
		int loc = blockno*blocksize;
		int addStore;
		int x = log2(cachesizeI);
		int y = log2(blocksize);
		bitset<32> mem_Add_bits(add);
		bitset<32> mab= mem_Add_bits>>(y);
		mab= mab<<(y);
		string smem_Add_bits = mem_Add_bits.to_string();
		string smab = mab.to_string();
		int itag = 12-x;
		string tag_off = smem_Add_bits.substr(20,itag);
		bitset<sizeof(int) * 8> toff(tag_off);
		int itoff = toff.to_ulong();
		int iStartBlock = 32-x;
		int iBlock = x-y;
		string block_off = smem_Add_bits.substr(iStartBlock,iBlock);
		bitset<sizeof(int) * 8> boff(block_off);
		int iboff = boff.to_ulong();
		int iStartWord=32-y;
		string word_off = smem_Add_bits.substr(iStartWord,y);
		bitset<sizeof(int) * 8> woff(word_off);
		int iwoff = woff.to_ulong();
		int cacheLoc = iboff*blocksize;
		int store;
		int initAdd = mab.to_ulong();
		int storeReturn;
		if(valid_bitI[loc]==1)
		{
			if(tagI[loc]==itoff)
			{
				hitsI++;
			}
			else
			{
				conflict_missI++;
				for(int i=0; i<blocksize;i+=4)
				{
					store=read1_word(MEM, initAdd+i);
					write1_word(cacheI,loc+i,store);
				}
				for(int i=loc; i<loc+blocksize; i++)
				{
					tagI[i]=itoff;
				}	
					
			}
		}
		else if(valid_bitI[loc]==0)
		{
			cold_missI++;
			for(int i=0; i<blocksize;i+=4)
			{
				store=read1_word(MEM, initAdd+i);
				write1_word(cacheI,loc+i,store);
			}
			for(int i=loc; i<loc+blocksize; i++)
			{
				tagI[i]=itoff;
				valid_bitI[i]=1;
			}
		}
		storeReturn=read1_word(cacheI,loc+iwoff);
		return storeReturn;				
	}
	else if(type==1||type==2||type==3||type==4)
	{
		int addDiv = add/blocksize;
		int modTo = cachesizeD/blocksize;
		int blockno = addDiv%modTo;
		int loc = blockno*blocksize;
		int addStore;
		int x = log2(cachesizeD);
		int y = log2(blocksize);
		bitset<32> mem_Add_bits(add);
		bitset<32> mab= mem_Add_bits>>(y);
		mab= mab<<(y);
		string smem_Add_bits = mem_Add_bits.to_string();
		string smab = mab.to_string();
		int itag = 12-x;
		string tag_off = smem_Add_bits.substr(20,itag);
		bitset<sizeof(int) * 8> toff(tag_off);
		int itoff = toff.to_ulong();
		int iStartBlock = 32-x;
		int iBlock = x-y;
		string block_off = smem_Add_bits.substr(iStartBlock,iBlock);
		bitset<sizeof(int) * 8> boff(block_off);
		int iboff = boff.to_ulong();
		int iStartWord=32-y;
		string word_off = smem_Add_bits.substr(iStartWord,y);
		bitset<sizeof(int) * 8> woff(word_off);
		int iwoff = woff.to_ulong();
		int cacheLoc = iboff*blocksize;
		int store;
		int initAdd = mab.to_ulong();
		int storeReturn;
		if(valid_bitD[loc]==1)
		{
			if(tagD[loc]==itoff)
			{
				hitsD++;
			}
			else
			{
				conflict_missD++;
				for(int i=0; i<blocksize;i+=4)
				{
					store=read1_word(MEM, initAdd+i);
					write1_word(cacheD,loc+i,store);
				}
				for(int i=loc; i<loc+blocksize; i++)
				{
					tagD[i]=itoff;
				}	
					
			}
		}
		else if(valid_bitD[loc]==0)
		{
			cold_missD++;
			for(int i=0; i<blocksize;i+=4)
			{
				store=read1_word(MEM, initAdd+i);
				write1_word(cacheD,loc+i,store);
			}
			for(int i=loc; i<loc+blocksize; i++)
			{
				tagD[i]=itoff;
				valid_bitD[i]=1;
			}
		}
		if(type==1)
		{	
			storeReturn=read1_word(cacheD,loc+iwoff);
		}
		else if(type==2)
		{
			storeReturn=read3_byte(cacheD,loc+iwoff);
		}
		else if(type==3)
		{
			storeReturn=read3_hw(cacheD,loc+iwoff);
		}
		else if(type==4)
		{
			storeReturn=read3_dw(cacheD,loc+iwoff);
		}
		return storeReturn;
	}
	else if(type==5||type==6)
	{
		int addDiv = add/blocksize;
		int modTo = cachesizeD/blocksize;
		int blockno = addDiv%modTo;
		int loc = blockno*blocksize;
		int addStore;
		int x = log2(cachesizeD);
		int y = log2(blocksize);
		bitset<32> mem_Add_bits(add);
		bitset<32> mab= mem_Add_bits>>(y);
		mab= mab<<(y);
		string smem_Add_bits = mem_Add_bits.to_string();
		string smab = mab.to_string();
		int itag = 12-x;
		string tag_off = smem_Add_bits.substr(20,itag);
		bitset<sizeof(int) * 8> toff(tag_off);
		int itoff = toff.to_ulong();
		int iStartBlock = 32-x;
		int iBlock = x-y;
		string block_off = smem_Add_bits.substr(iStartBlock,iBlock);
		bitset<sizeof(int) * 8> boff(block_off);
		int iboff = boff.to_ulong();
		int iStartWord=32-y;
		string word_off = smem_Add_bits.substr(iStartWord,y);
		bitset<sizeof(int) * 8> woff(word_off);
		int iwoff = woff.to_ulong();
		int cacheLoc = iboff*blocksize;
		int store;
		int initAdd = mab.to_ulong();
		int storeReturn;
		if(valid_bitD[loc]==1)
		{
			if(tagD[loc]==itoff)
			{
				hitsD++;
			}
			else
			{
				conflict_missD++;
				for(int i=0; i<blocksize;i+=4)
				{
					store=read1_word(MEM, initAdd+i);
					write1_word(cacheD,loc+i,store);
				}
				for(int i=loc; i<loc+blocksize; i++)
				{
					tagD[i]=itoff;
				}	
					
			}
		}
		else if(valid_bitD[loc]==0)
		{
			cold_missD++;
			for(int i=0; i<blocksize;i+=4)
			{
				store=read1_word(MEM, initAdd+i);
				write1_word(cacheD,loc+i,store);
			}
			for(int i=loc; i<blocksize; i++)
			{
				tagD[i]=itoff;
				valid_bitD[i]=1;
			}
		}
		if(type==5)
		{
			write4_word(cacheD,loc+iwoff,daalo);
			write4_word(MEM,add,daalo);		
		}
		else if(type==6)
		{
			write3_dw(cacheD,loc+iwoff,daalo);
			write3_dw(MEM,add,daalo);
		}
		return 0;
	}
	}

//-----------------set associative------------------------------------------------------------
	else if(cachetype==3)
	{
		if(type==0)
	{
		bitset<32> mem_Add_bits(add);
		//cout<<"address hh"<<mem_Add_bits<<endl;
		int x = log2(cachesizeI);
		int y = log2(blocksize);
		//cout<<"YYYYYY"<<y<<endl;
		int k = log2(noSets);
		string smem_Add_bits = mem_Add_bits.to_string();
		int itag = 12-x+k;
		string tag_off = smem_Add_bits.substr(20,itag);
		bitset<sizeof(int) * 8> toff(tag_off);
		int itoff = toff.to_ulong();
		int iStartSet = 32-x+k;
		int iSet = x-y-k;
		string set_off = smem_Add_bits.substr(iStartSet,iSet);
		bitset<sizeof(int) * 8> soff(set_off);
		int isoff = soff.to_ulong();
		
		int iStartWord=32-y;
		string word_off = smem_Add_bits.substr(iStartWord,y);
		bitset<sizeof(int) * 8> woff(word_off);
		int iwoff = woff.to_ulong();
		//cout<<"iwoff"<<iwoff<<endl;
		int searchStartI = isoff*noSets*blocksize;
		int searchUpto = noSets*blocksize;
		int notFound = 0;
		//cout<<"searchStartI"<<searchStartI<<endl;
		//cout<<"searchUpto"<<searchUpto<<endl;
		bitset<32> mab= mem_Add_bits>>(y);
		mab= mab<<(y);
		//cout<<"maab"<<mab<<endl;
		int initAdd = mab.to_ulong();
		
		int storeIndex=0;
		int store;
		int storeReturn;
		
		for(int i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
		{
			if(valid_bitI[i]==1)
			{
			//    cout<<"TAG"<<tagI[i]<<endl;
			  //  cout<<"ITFff"<<itoff<<endl;
				if(tagI[i]==itoff)
				{
				//	cout<<"int it"<<endl;
					hitsI++;
					int ind = i;
					for(i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
					{
						if(valid_bitI[i]==1)
						{
							for(int j=i; j<i+blocksize; j++)
							{	
								if(LRUI[j]<LRUI[ind])
								{	
									LRUI[j]++;
								}
							}
						}
					}
					for(int j=ind; j<ind+blocksize; j++)
					{
						LRUI[j]=0;
					}
					storeIndex=ind;
					break;
				}
				else
				{   notFound++;
				    //cout<<"in else"<<endl;
				}
			
			}
			else if(valid_bitI[i]==0)
			{	
				//cout<<"here"<<endl;
				cold_missI++;
				for(int ii=0; ii<blocksize;ii+=4)
				{
					//cout<<"initAdd"<<initAdd<<endl;
					store=read1_word(MEM, initAdd+ii);
					//cout<<"in for"<<store<<endl;
					write1_word(cacheI,ii+i,store);
				}
				for(int j=searchStartI; j<searchStartI+searchUpto; j++)
				{
					if(valid_bitI[j]==1)
					{
						LRUI[j]++;
					}
				}
				for(int ii=i; ii<i+blocksize; ii++)
				{
					tagI[ii]=itoff;
					valid_bitI[ii]=1;
					LRUI[ii]=0;
				}
				storeIndex = i;
				break;
			}
			
		} 
		if(notFound==noSets)
		{
				conflict_missI++;
				int LRUMaxInd=searchStartI;
				for(int i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
				{
					if(LRUI[i]>LRUI[LRUMaxInd])
					{
						LRUMaxInd = i;
					}
				}
				for(int ii=0; ii<blocksize;ii+=4)
				{
					store=read1_word(MEM, initAdd+ii);
					write1_word(cacheI,ii+LRUMaxInd,store);
				}	
				for(int j=searchStartI; j<searchStartI+searchUpto; j++)
				{
					if(valid_bitI[j]==1 && LRUI[j]<LRUI[LRUMaxInd])
					{
						LRUI[j]++;
					}
				}
				for(int ii=LRUMaxInd; ii<LRUMaxInd+blocksize; ii++)
				{
					tagI[ii]=itoff;
					valid_bitI[ii]=1;
					LRUI[ii]=0;
				}
				storeIndex = LRUMaxInd;
		}
		//cout<<"storeIndex"<<storeIndex<<endl;
		storeReturn = read1_word(cacheI,storeIndex+iwoff);
		return storeReturn;
		
	}
	else if(type==1 || type==2 || type==3 || type==4)
	{
		bitset<32> mem_Add_bits(add);
		int x = log2(cachesizeD);
		int y = log2(blocksize);
		int k = log2(noSets);
		string smem_Add_bits = mem_Add_bits.to_string();
		int itag = 12-x+k;
		string tag_off = smem_Add_bits.substr(20,itag);
		bitset<sizeof(int) * 8> toff(tag_off);
		int itoff = toff.to_ulong();
		int iStartSet = 32-x+k;
		int iSet = x-y-k;
		string set_off = smem_Add_bits.substr(iStartSet,iSet);
		bitset<sizeof(int) * 8> soff(set_off);
		int isoff = soff.to_ulong();
		
		int iStartWord=32-y;
		string word_off = smem_Add_bits.substr(iStartWord,y);
		bitset<sizeof(int) * 8> woff(word_off);
		int iwoff = woff.to_ulong();
		
		int searchStartI = isoff*noSets*blocksize;
		int searchUpto = noSets*blocksize;
		int notFound = 0;
		
		bitset<32> mab= mem_Add_bits>>(y);
		mab= mab<<(y);
		int initAdd = mab.to_ulong();
		
		int storeIndex=0;
		int store;
		int storeReturn;
		
		for(int i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
		{
			if(valid_bitD[i]==1)
			{
				if(tagD[i]==itoff)
				{
					hitsD++;
					int ind = i;
					for(i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
					{
						if(valid_bitD[i]==1)
						{
							for(int j=i; j<i+blocksize; j++)
							{	
								if(LRUD[j]<LRUD[ind])
								{	
									LRUD[j]++;
								}
							}
						}
					}
					for(int j=ind; j<ind+blocksize; j++)
					{
						LRUD[j]=0;
					}
					storeIndex=ind;
					break;
				}
				else notFound++;
			
			}
			else if(valid_bitD[i]==0)
			{	
				cold_missD++;
				for(int ii=0; ii<blocksize;ii+=4)
				{
					store=read1_word(MEM, initAdd+ii);
					write1_word(cacheD,ii+i,store);
				}
				for(int j=searchStartI; j<searchStartI+searchUpto; j++)
				{
					if(valid_bitD[j]==1)
					{
						LRUD[j]++;
					}
				}
				for(int ii=i; ii<i+blocksize; ii++)
				{
					tagD[ii]=itoff;
					valid_bitD[ii]=1;
					LRUD[ii]=0;
				}
				storeIndex = i;
				break;
			}
			
		} 
		if(notFound==noSets)
		{
				conflict_missD++;
				int LRUMaxInd=searchStartI;
				for(int i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
				{
					if(LRUD[i]>LRUD[LRUMaxInd])
					{
						LRUMaxInd = i;
					}
				}
				for(int ii=0; ii<blocksize;ii+=4)
				{
					store=read1_word(MEM, initAdd+ii);
					write1_word(cacheD,ii+LRUMaxInd,store);
				}	
				for(int j=searchStartI; j<searchStartI+searchUpto; j++)
				{
					if(valid_bitD[j]==1 && LRUD[j]<LRUD[LRUMaxInd])
					{
						LRUD[j]++;
					}
				}
				for(int ii=LRUMaxInd; ii<LRUMaxInd+blocksize; ii++)
				{
					tagD[ii]=itoff;
					valid_bitD[ii]=1;
					LRUD[ii]=0;
				}
				storeIndex = LRUMaxInd;
		}
		if(type==1)
		{	
			storeReturn = read1_word(cacheD,storeIndex+iwoff);
		}
		else if(type==2)
		{
			storeReturn = read3_byte(cacheD,storeIndex+iwoff);
		} 
		else if(type==3)
		{
			storeReturn=read3_hw(cacheD,storeIndex+iwoff);
		}
		else if(type==4)
		{
			storeReturn=read3_dw(cacheD,storeIndex+iwoff);
		}
		return storeReturn;
	}
	else if(type==5 || type==6 )
	{
		bitset<32> mem_Add_bits(add);
		int x = log2(cachesizeD);
		int y = log2(blocksize);
		int k = log2(noSets);
		string smem_Add_bits = mem_Add_bits.to_string();
		int itag = 12-x+k;
		string tag_off = smem_Add_bits.substr(20,itag);
		bitset<sizeof(int) * 8> toff(tag_off);
		int itoff = toff.to_ulong();
		int iStartSet = 32-x+k;
		int iSet = x-y-k;
		string set_off = smem_Add_bits.substr(iStartSet,iSet);
		bitset<sizeof(int) * 8> soff(set_off);
		int isoff = soff.to_ulong();
		
		int iStartWord=32-y;
		string word_off = smem_Add_bits.substr(iStartWord,y);
		bitset<sizeof(int) * 8> woff(word_off);
		int iwoff = woff.to_ulong();
		
		int searchStartI = isoff*noSets*blocksize;
		int searchUpto = noSets*blocksize;
		int notFound = 0;
		
		bitset<32> mab= mem_Add_bits>>(y);
		mab= mab<<(y);
		int initAdd = mab.to_ulong();
		
		int storeIndex=0;
		int store;
		int storeReturn;
		
		for(int i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
		{
			if(valid_bitD[i]==1)
			{
				if(tagD[i]==itoff)
				{
					hitsD++;
					int ind = i;
					for(i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
					{
						if(valid_bitD[i]==1)
						{
							for(int j=i; j<i+blocksize; j++)
							{	
								if(LRUD[j]<LRUD[ind])
								{	
									LRUD[j]++;
								}
							}
						}
					}
					for(int j=ind; j<ind+blocksize; j++)
					{
						LRUD[j]=0;
					}
					storeIndex=ind;
					break;
				}
				else notFound++;
			
			}
			else if(valid_bitD[i]==0)
			{	
				cold_missD++;
				for(int ii=0; ii<blocksize;ii+=4)
				{
					store=read1_word(MEM, initAdd+ii);
					write1_word(cacheD,ii+i,store);
				}
				for(int j=searchStartI; j<searchStartI+searchUpto; j++)
				{
					if(valid_bitD[j]==1)
					{
						LRUD[j]++;
					}
				}
				for(int ii=i; ii<i+blocksize; ii++)
				{
					tagD[ii]=itoff;
					valid_bitD[ii]=1;
					LRUD[ii]=0;
				}
				storeIndex = i;
				break;
			}
			
		} 
		if(notFound==noSets)
		{
				conflict_missD++;
				int LRUMaxInd=searchStartI;
				for(int i=searchStartI; i<searchStartI+searchUpto; i+=blocksize)
				{
					if(LRUD[i]>LRUD[LRUMaxInd])
					{
						LRUMaxInd = i;
					}
				}
				for(int ii=0; ii<blocksize;ii+=4)
				{
					store=read1_word(MEM, initAdd+ii);
					write1_word(cacheD,ii+LRUMaxInd,store);
				}	
				for(int j=searchStartI; j<searchStartI+searchUpto; j++)
				{
					if(valid_bitD[j]==1 && LRUD[j]<LRUD[LRUMaxInd])
					{
						LRUD[j]++;
					}
				}
				for(int ii=LRUMaxInd; ii<LRUMaxInd+blocksize; ii++)
				{
					tagD[ii]=itoff;
					valid_bitD[ii]=1;
					LRUD[ii]=0;
				}
				storeIndex = LRUMaxInd;
		}
		storeReturn = 0;
		if(type==5)
		{
			write4_word(cacheD,storeIndex+iwoff,daalo);
			write4_word(MEM,add,daalo);		
		}
		else if(type==6)
		{
			write3_dw(cacheD,storeIndex+iwoff,daalo);
			write3_dw(MEM,add,daalo);
		}
		return storeReturn;
	}

	}
}



int read3_byte(unsigned char *mem, unsigned int address){
	char *data;
	data = (char*) (mem + address);
	int *val;
	val=(int*)data;
	//cout<<"data"<<*data<<endl;
	int x = (*val >> (8*0)) & 0xff;
	
	return x;
}

int read3_hw(unsigned char *mem, unsigned int address){
	char *data;
	data = (char*) (mem + address);
	int *val;
	val=(int*)data;
	//cout<<"data"<<*data<<endl;
	int x = (*val >> (8*0)) & 0xffff;
	
	return x;
}

int read3_dw(unsigned char *mem, unsigned int address) {
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

int read4_word(unsigned char *mem, unsigned int address) {
  int *data;
  data =  (int*) (mem + address);
  int x=*data;
  bitset<32> d(x);
  //cout<<"d:"<<d<<endl;
  //cout<<"data++++++++"<<(*(data))<<endl;
  return *data;
}

void write3_hw(unsigned char *mem, unsigned int address, uint16_t data) {
  uint16_t *data_p;
  data_p = (uint16_t*) (mem + address);
  *data_p = data;
  //printf("okie %d dokie",*data_p);
  return;
}

void write3_byte(unsigned char *mem, unsigned int address, uint8_t data) {
  uint8_t *data_p;
  data_p = (uint8_t*) (mem + address);
  *data_p = data;
  //cout<<"heyy"<<*data_p<<"ding"<<endl;
  //printf("dingchak%ddinchak",*data_p);
  return;
}
//lomng long int
void write3_dw(unsigned char *mem, unsigned int address, unsigned long long int data) {
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

void write4_word(unsigned char *mem, unsigned int address, unsigned int data) {
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

