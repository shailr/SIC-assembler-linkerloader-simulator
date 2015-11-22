

#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "common.h"

char memory[MEMORYLEN][2];

class Processor
{
	public:
		string A, X, L, PC, SW;
		Processor()
		{
			A=X=L=PC=SW="000000";
		}
		void printStatus()
		{
			cout<<endl<<"-----------------------------"<<endl;
			cout<<endl<<"Processor Status:"<<endl<<endl;
			cout<<"A --->"<<A<<endl;
			cout<<"X --->"<<X<<endl;
			cout<<"L --->"<<L<<endl;
			cout<<"PC --->"<<PC<<endl;
			cout<<"SW --->"<<SW<<endl<<endl;
			cout<<endl<<"-----------------------------"<<endl;
		}
		void show_sta()
		{
			cout<<endl<<"-----------------------------"<<endl;
			cout<<"A --->"<<A<<"\t";
			cout<<"X --->"<<X<<"\t";
			cout<<"L --->"<<L<<"\t";
			cout<<"PC --->"<<PC<<"\t";
			cout<<"SW --->"<<SW;
			cout<<endl<<"-----------------------------"<<endl;
		}
		int increamentPC()
		{
			int addr;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			addr+=3;
			s<<setw(6)<<setfill('0')<<uppercase<<std::hex<<addr;
			s>>PC;
			return addr;
		}
		void lda()
		{
			int addr;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>A;
			//cout<<endl<<"A:"<<A<<endl;
			cout<<endl<<"Load Accumulator"<<endl;
		}
		void ldx()
		{
			int addr;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>X;
			//cout<<endl<<"X:"<<X<<endl;
			cout<<endl<<"Load X"<<endl;
		}
		void sta()
		{
			int addr;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get operand
			
			memory[addr][0]=A[0], memory[addr][1]=A[1], memory[addr+1][0]=A[2], memory[addr+1][1]=A[3];
			memory[addr+2][0]=A[4], memory[addr+2][1]=A[5];
			cout<<endl<<"Store Accumulator"<<endl;
		}
		void stx()
		{
			int addr;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get operand
			
			memory[addr][0]=X[0], memory[addr][1]=X[1], memory[addr+1][0]=X[2], memory[addr+1][1]=X[3];
			memory[addr+2][0]=X[4], memory[addr+2][1]=X[5];
			cout<<endl<<"Store x"<<endl;
		}
		void add()
		{
			int addr,op,acc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get location of operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>op;			//Get value of operand
			s.str(""); s.clear();
			
			s.str(A);
			s>>std::hex>>acc;
			s.str(""); s.clear();
			
			acc=acc+op;
			s<<setw(6)<<setfill('0')<<uppercase<<std::hex<<acc;
			s>>A;
			cout<<endl<<"Adding "<<setw(6)<<setfill('0')<<std::hex<<op<<" to acc, result= "<<setw(6)<<setfill('0')<<std::hex<<acc<<endl;
		}
		void sub()
		{
			int addr,op,acc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get location of operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>op;			//Get value of operand
			s.str(""); s.clear();
			
			s.str(A);
			s>>std::hex>>acc;
			s.str(""); s.clear();
			
			acc=acc-op;
			s<<setw(6)<<setfill('0')<<uppercase<<std::hex<<acc;
			s>>A;
			cout<<endl<<"Subtracting "<<setw(6)<<setfill('0')<<std::hex<<op<<" from acc, result= "<<setw(6)<<setfill('0')<<std::hex<<acc<<endl;
		}
		void mul()
		{
			int addr,op,acc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get location of operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>op;			//Get value of operand
			s.str(""); s.clear();
			
			s.str(A);
			s>>std::hex>>acc;
			s.str(""); s.clear();
			
			acc=acc*op;
			s<<setw(6)<<setfill('0')<<uppercase<<std::hex<<acc;
			s>>A;
			cout<<endl<<"Multiplying "<<setw(6)<<setfill('0')<<std::hex<<op<<" with acc, result= "<<setw(6)<<setfill('0')<<std::hex<<acc<<endl;
		}
		void div()
		{
			int addr,op,acc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get location of operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>op;			//Get value of operand
			s.str(""); s.clear();
			
			s.str(A);
			s>>std::hex>>acc;
			s.str(""); s.clear();
			
			acc=acc/op;
			s<<setw(6)<<setfill('0')<<uppercase<<std::hex<<acc;
			s>>A;
			cout<<endl<<"Dividing "<<setw(6)<<setfill('0')<<std::hex<<op<<" & acc, result= "<<setw(6)<<setfill('0')<<std::hex<<acc<<endl;
		}
		void comp()
		{
			int addr, op, acc, res = -1, sc = -1;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get location of operand
			s.str(""); s.clear();
			
			s.put(memory[addr][0]), s.put(memory[addr][1]), s.put(memory[addr+1][0]), s.put(memory[addr+1][1]);
			s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>op;			//Get value of operand
			s.str(""); s.clear();
			
			s.str(A);
			s>>std::hex>>acc;
			s.str(""); s.clear();
			
			res = acc - op;
		    if(res==0)
		    {
		        SW="00";
		        sc=0;
		    }
		    else if(res<0)
		    {
		        SW="01";
		        sc=1;
		    }
		    else
		    {
		        SW="02";
		        sc=2;
		    }

		    if(sc==1)
		        cout<<endl<<"Register A is Less than operand"<<endl;
		    else if(sc==0)
		        cout<<endl<<"Register A is  Equal to operand"<<endl;
		    else
		        cout<<endl<<"Register A is Greater than operand"<<endl;
		}
		void jeq()
		{
			cout<<endl<<"Jump on Equal..."<<endl;
			int addr,sc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.str(SW);
			s>>std::hex>>sc;
			s.str(""); s.clear();
			if(sc==0)
			{
				s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
				s>>std::hex>>addr;		//Get jump address
				s.str(""); s.clear();
				addr-=3;
				
				s<<setw(6)<<setfill('0')<<std::hex<<addr;
				s>>PC;
				//cout<<"here in jeq:"<<std::hex<<addr<<endl;
			}
		}
		void jgt()
		{
			cout<<endl<<"Jump on Greater than..."<<endl;
			int addr,sc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.str(SW);
			s>>std::hex>>sc;
			s.str(""); s.clear();
			if(sc==2)
			{
				s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
				s>>std::hex>>addr;		//Get jump address
				s.str(""); s.clear();
				addr-=3;
				
				s<<setw(6)<<setfill('0')<<std::hex<<addr;
				s>>PC;
				//cout<<"here in jgt:"<<std::hex<<addr<<endl;
			}
		}
		void jlt()
		{
			cout<<endl<<"Jump on Less than..."<<endl;
			int addr,sc;
			stringstream s(PC);
			s>>std::hex>>addr;	//Get PC
			s.str(""); s.clear();
			
			s.str(SW);
			s>>std::hex>>sc;
			s.str(""); s.clear();
			if(sc==1)
			{
				s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
				s>>std::hex>>addr;		//Get jump address
				s.str(""); s.clear();
				addr-=3;
				
				s<<setw(6)<<setfill('0')<<std::hex<<addr;
				s>>PC;
				//cout<<"here in jlt:"<<std::hex<<addr<<endl;
			}
		}
		void jsub()
		{
			cout<<endl<<"Jump to subroutine..."<<endl;
			int addr;
			stringstream s(PC);
			s>>std::hex>>addr;		//Get PC
			s.str(""); s.clear();
			
			L==PC;				//Copy current location to linkage register
			
			s.put(memory[addr+1][0]), s.put(memory[addr+1][1]), s.put(memory[addr+2][0]), s.put(memory[addr+2][1]);
			s>>std::hex>>addr;		//Get jump address
			s.str(""); s.clear();
			addr-=3;
			
			s<<setw(6)<<setfill('0')<<std::hex<<addr;
			s>>PC;
		}
		void rsub()
		{
			int addr;
			cout<<endl<<"Returning to main routine..."<<endl;
			PC=L;				//Get return address
			stringstream s(PC);
			s>>std::hex>>addr;		//Get PC
			s.str(""); s.clear();
			addr-=3;
			s<<setw(6)<<setfill('0')<<std::hex<<addr;
			s>>PC;
		}
		void execute(int i)
		{
		    switch(i)
		    {
			    case 0:
			        lda();
			        break;
			    case 1:
			        ldx();
			        break;
			    case 2:
			        sta();
			        break;
			    case 3:
			        stx();
			        break;
			    case 4:
			        add();
			        break;
			    case 5:
			        sub();
			        break;
			    case 6:
			        mul();
			        break;
			    case 7:
			        div();
			        break;
			    case 8:
			        comp();
			        break;
			    case 9:
			        jeq();
			        break;
			    case 10:
			        jgt();
			        break;
			    case 11:
			        jlt();
			        break;
			    case 12:
			        jsub();
			        break;
			    case 13:
			        rsub();
			        break;
			    default:
			        //cout<<"Execution problem...no such opcode found"<<endl;
			        break;
		    }
		}
};

#endif
