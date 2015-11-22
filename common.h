

#ifndef _COMMON_H
#define _COMMON_H

#include<iostream>
#include<iomanip>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include<conio.h>

#define MAX_OPCODES 100
#define MAX_SYMBOLS 100
#define MEMORYLEN 65536

using namespace std;

int extractTokens(char* , char* [] );

struct OpcodeTable
{
	struct Entry
	{
		string mnemonics;
		string opcode;
		int no_operands;
	} codes[MAX_OPCODES];
	int total_op;
	OpcodeTable()
	{
		total_op = 0;
		ifstream f;
		string op;
		string code;
		int n_operands;
		int i=0,j=0,k=0;
		f.open("opcodes.txt");
		while(!f.eof() && total_op < MAX_OPCODES)
		{
			f>>op>>code>>n_operands;
			codes[i].mnemonics = op;
			codes[i].opcode = code;
			codes[i].no_operands = n_operands;
			total_op++;
			i++;
		}
		
		f.close();
	}
	void printInstr()
	{
		int i = 0;
		for(i=0;i<total_op;i++)
		{
			cout<<i<<">\t"<<codes[i].mnemonics<<"\t"<<codes[i].opcode<<"\t"<<codes[i].no_operands<<endl;
		}
	}
	
	int search(string op)
	{
		int i=0, found = -1;
		while(i<total_op)
		{
			if(op == codes[i].mnemonics)
			{
				found = i;
				break;
			}
			i++;
		}
		return found;
	}
	
	int searchCode(int o)
	{
		string op;
		stringstream s;
		s<<setw(2)<<setfill('0')<<uppercase<<std::hex<<o;
		s>>op;
		int i=0, found = -1;
		while(i<total_op)
		{
			if(op == codes[i].opcode)
			{
				found = i;
				break;
			}
			i++;
		}
		return found;
	}
};

struct SymTable
{
	struct Entry
	{
		string symbol;
		string address;
	} sym_tab[MAX_SYMBOLS];
	int total_symbols;
	SymTable()
	{
		total_symbols = 0;
	}
	void printSymTab()
	{
		int i = 0;
		for(i=0;i<total_symbols;i++)
		{
			cout<<i<<">\t"<<sym_tab[i].symbol<<"\t"<<sym_tab[i].address<<endl;
		}
		
	}
	void writeToFile(string file)
	{
		int i = 0;
		ofstream out_symtab(file.c_str());
		for(i=0;i<total_symbols;i++)
		{
			out_symtab<<i<<">\t"<<sym_tab[i].symbol<<"\t"<<sym_tab[i].address<<endl;
		}
	}
	
	//	-1. 			Symbol doesn't Exist.
	//	0-MAX_SYMBOLS. 	Symbol exist at this(returned) location.
	int search(string sym)
	{
		int i=0, found = -1;
		while(i<total_symbols)
		{
			if(sym == sym_tab[i].symbol)
			{
				found = i;
				break;
			}
			i++;
		}
		return found;
	}
	
	//	0. Unknown Error.
	//	1. Added Successfully.
	//	2. Symbol Already Exist.
	//	3. Symbol Table Full.
	int addSym(string sym, string addr)
	{
		int f = search(sym);
		if(f==-1 && total_symbols<MAX_SYMBOLS)
		{
			sym_tab[total_symbols].symbol = sym;
			sym_tab[total_symbols].address = addr;
			total_symbols++;
			return 1;
		}
		else if(f!=-1 && total_symbols<MAX_SYMBOLS)
		{
			return 2;
		}
		else if(f==-1 && total_symbols>=MAX_SYMBOLS)
		{
			return 3;
		}
		else
		{}
		return 0;
	}
};

//Returns no. of tokens extracted...
int extractTokens(char* line, char* tokens[], char sep)
{
	char* buf;
	int no_tokens=0, j=0;
	while(line[j]!='\0')
	{
		if(line[j]!=sep)
		{
			j++;
		}
		else
		{
			line[j]='\0';
			buf=line;
			line=line+j+1;
			j = 0;
			tokens[no_tokens] = buf;
			no_tokens++;
		}
	}
	tokens[no_tokens] = line;
	no_tokens++;
	return no_tokens;
}

#endif
