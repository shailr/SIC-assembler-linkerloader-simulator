
#include "common.h"
#include "processor.h"

typedef enum mode_e {QUIET,TRACE,STEP} mode_e;
mode_e mode;
int start_addr = 0, prog_length = 0, used_mem_length = 0;

OpcodeTable optab;
SymTable estab;

int linkingLoaderPass1(string file, int csaddr)
{
    cout<<endl<<"-----------------"<<endl;
    cout<<"Running pass 1..."<<endl;
	ifstream hex((file+".o").c_str());
	char line[100];
    char* tokens[6];
	int addr = 0, csaddr_as = 0, rel = 0, cslen = 0;
	int no_tokens = 0;
	string cs_name;
	string rec_type, loc, opcode, operand;
	if(hex)
	{
		rec_type.clear(), cs_name.clear(), loc.clear(), opcode.clear(), operand.clear();
		hex.getline(line,100);
		no_tokens = extractTokens(line,tokens,'^');
		rec_type.assign(tokens[0]);
		if(rec_type[0]=='H')
		{
			cs_name.assign(tokens[1]);
			loc.assign(tokens[2]);
			stringstream s(loc.substr(1,loc.length()-1));
			s>>std::hex>>csaddr_as;
			s.str("");s.clear();
			
			addr = csaddr_as;
			rel = addr-csaddr_as;
			operand.assign(tokens[3]);
			s<<operand;
			s>>std::hex>>cslen;
			s.str("");s.clear();
			
            cout<<endl<<"Control Section Name::"<<cs_name<<endl<<"Start Address::"<<setw(4)<<setfill('0')<<uppercase<<std::hex<<(csaddr+rel)<<endl<<"Control Section length::"<<setw(4)<<setfill('0')<<uppercase<<std::hex<<cslen<<endl<<endl;
		}
		else
			cout<<"ERROR: No header record"<<endl;
		
		while(hex && rec_type[0]!='E')
		{
			rec_type.clear(), loc.clear(), operand.clear();
			hex.getline(line,100);
			no_tokens = extractTokens(line,tokens,'^');
			rec_type.assign(tokens[0]);
			if(rec_type[0]=='D')
			{
				string sym;
				sym.assign(tokens[1]);
				operand.assign(tokens[2]);
				stringstream ss(operand);
				ss>>std::hex>>addr;
				ss.str("");ss.clear();
				
				rel = addr - csaddr_as;
				ss<<setw(4)<<setfill('0')<<uppercase<<std::hex<<(csaddr+rel);
				ss>>loc;
				
				if(estab.search(sym)==-1)
                {
                    estab.addSym(sym, loc);  
                }
                else
                {
                	if(estab.sym_tab[estab.search(sym)].address=="na")
                	{
                		estab.sym_tab[estab.search(sym)].address = loc;
                	}
                	else
					{
		                cout<<"Error: Duplicate External Symbol"<<endl;
		                throw 0;
                	}
                }
			}
			else if(rec_type[0]=='R')
			{
				string sym;
				sym.assign(tokens[1]);
				if(estab.search(sym)==-1)
                {
                    estab.addSym(sym, "na");
                }
			}
		}
	}
	else
		cout<<"ERROR: Problem with file handler."<<endl;
		
	hex.close();
    cout<<"Pass 1 over..."<<endl;
    cout<<"-----------------"<<endl;
	return cslen;
}

int linkingLoaderPass2(string file, int csaddr)
{
    cout<<endl<<"-----------------"<<endl;
    cout<<"Running pass 2..."<<endl;
	ifstream hex((file+".o").c_str());
	char line[100];
    char* tokens[6];
    unsigned char op;
	int addr_it = 0, addr = 0, csaddr_as = 0, rel = 0, cslen = 0, temp = 0;
	int no_tokens = 0;
	string cs_name;
	string rec_type, loc, opcode, operand;
	if(hex)
	{
		rec_type.clear(), loc.clear(), opcode.clear(), operand.clear();
		hex.getline(line,100);
		no_tokens = extractTokens(line,tokens,'^');
		rec_type.assign(tokens[0]);
		if(rec_type[0]=='H')
		{
			cs_name.assign(tokens[1]);
			loc.assign(tokens[2]);
			stringstream s(loc.substr(1,loc.length()-1));
			s>>std::hex>>csaddr_as;
			s.str("");s.clear();
			
			addr_it = addr = csaddr_as;
			rel = addr-csaddr_as;
			operand.assign(tokens[3]);
			s<<operand;
			s>>std::hex>>cslen;
			s.str("");s.clear();
			
            cout<<endl<<"Control Section Name::"<<cs_name<<endl<<"Start Address::"<<setw(4)<<setfill('0')<<uppercase<<std::hex<<(csaddr+rel)<<endl<<"Control Section length::"<<setw(4)<<setfill('0')<<uppercase<<std::hex<<cslen<<endl<<endl;
		}
		else
			cout<<"ERROR: No header record"<<endl;
		
		while(hex && rec_type[0]!='E')
		{
			rec_type.clear(), loc.clear(), opcode.clear(), operand.clear();
			hex.getline(line,100);
			no_tokens = extractTokens(line,tokens,'^');
			rec_type.assign(tokens[0]);
			
			loc.assign(tokens[1]);
			
			stringstream ss(loc);
			ss>>std::hex>>temp;
			ss.str("");ss.clear();
			rel = temp - csaddr_as;
			temp = csaddr + rel;
			ss<<setw(4)<<setfill('0')<<uppercase<<std::hex<<temp;
			ss>>loc;
			ss.str("");ss.clear();
			
			addr=temp;
			
			if(rec_type[0]=='T')
			{
				if(no_tokens==4)
				{
					opcode.assign(tokens[2]);
					operand.assign(tokens[3]);
						
					if(operand[0]=='@')
					{
						operand.assign(operand.substr(1,operand.length()-1));
						if(estab.search(operand)>=0)
		                {
		                    operand.assign(estab.sym_tab[estab.search(operand)].address);
		                }
					}
					else
					{
						ss<<operand;
						ss>>std::hex>>temp;
						ss.str("");ss.clear();
						rel = temp - csaddr_as;
						temp = csaddr + rel;
						ss<<setw(4)<<setfill('0')<<uppercase<<std::hex<<temp;
						ss>>operand;
					}
					
					addr_it = addr;
					memory[addr_it][0]=opcode[0], memory[addr_it][1]=opcode[1];
					//cout<<std::hex<<memory[addr_it]<<endl;
					
					memory[addr_it+1][0]=operand[0], memory[addr_it+1][1]=operand[1];
					//cout<<std::hex<<memory[addr_it+1]<<endl;
					
					memory[addr_it+2][0]=operand[2], memory[addr_it+2][1]=operand[3];
					//cout<<std::hex<<memory[addr_it+2]<<endl;
					
					//cout<<tokens[0]<<"\t"<<tokens[1]<<"\t"<<tokens[2]<<"\t"<<tokens[3]<<endl;
				}
				else if(no_tokens==3)
				{
					operand.assign(tokens[2]);
					int i = 0;
					char temp[10];
					while(operand[i]!='\0')
					{
						if(i==0)
						{
							addr_it = addr;
						}
						memory[addr_it][0]=operand[i], memory[addr_it][1]=operand[i+1];
						//cout<<std::hex<<memory[addr_it+2]<<endl;
						addr_it++;
						i+=2;
					}
					//cout<<tokens[0]<<"\t"<<tokens[1]<<"\t"<<tokens[2]<<endl;
				}
				else{}
			}
		}
	}
	else
		cout<<"ERROR: Problem with file handler."<<endl;
		
	hex.close();
    cout<<"Pass 2 over..."<<endl;
    cout<<"-----------------"<<endl;
	return cslen;
}

void linkingLoader(string files[], int no_files, int progaddr)
{
	int cslen = 0, i = 0;
	int csaddr = progaddr;
	for(i=0;i<no_files;i++)
	{
		cslen = linkingLoaderPass1(files[i], csaddr);
		if(i==0)
		{
			start_addr = csaddr;
			prog_length = cslen;
		}
		csaddr = csaddr + cslen + 15;
	}
	cslen = 0;
	csaddr = progaddr;
	for(i=0;i<no_files;i++)
	{
		cslen = linkingLoaderPass2(files[i], csaddr);
		if(i==0)
		{
			start_addr = csaddr;
			prog_length = cslen;
		}
		csaddr = csaddr + cslen + 15;
	}
	
	used_mem_length = csaddr - progaddr;
}

void printMemory()
{
	cout<<endl<<"-----------------------------"<<endl;
    cout<<"Memory contents..."<<endl<<endl;
    cout<<"\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"<<endl<<endl;
    for(int i=start_addr; i<start_addr+used_mem_length; )
    {
        cout<<i<<"\t";
        for(int k=0; k<16 && i<start_addr+used_mem_length; i++,k++)
        {
        	cout<<std::hex<<memory[i][0]<<memory[i][1]<<" ";
        }
        cout<<endl;
    }
	cout<<endl<<"-----------------------------"<<endl;
}

void simulate()
{
	Processor proc;
	stringstream s;
	int addr=start_addr, op, i;
	s<<setw(6)<<setfill('0')<<std::hex<<addr;
	s>>proc.PC;
	s.str(""); s.clear();
	int m=0;
    while(m<1 || m>3)
    {
        cout<<endl<<"Choose the mode: "<<endl<<"1.QUIET Mode"<<endl<<"2.Trace Mode"<<endl<<"3.Step Mode"<<endl;
        cin>>m;
        if(m==1)
            mode=QUIET;
        else if(m==2)
            mode=TRACE;
        else if(m==3)
            mode=STEP;
    }

    if(mode==TRACE || mode==STEP)
    {
        proc.printStatus();
        printMemory();
    }
    
    while(addr<(start_addr+prog_length))
    {
    	s.put(memory[addr][0]), s.put(memory[addr][1]);
    	//cout<<memory[addr][0]<<memory[addr][1]<<endl;
		s>>std::hex>>op;		//Get opcode
		s.str(""); s.clear();
		i = optab.searchCode(op);
		
		//cout<<"here op:"<<op<<"\t"<<i<<endl;
		if(i>=0)
			proc.execute(i);
		
		addr = proc.increamentPC();
		
		if(mode==TRACE || mode==STEP)
        {
            if(i>=0)
                proc.show_sta();
            if(mode==STEP && i>=0)
            {
                cout<<"Next"<<endl;
                getch();
            }
            //Sleep(300);
        }
    }
    
    proc.printStatus();
    printMemory();
}

int main()
{
	string in_file[10];
	int m = 1, no_files = 0;
	while(m==1 && no_files<10)
	{
		cout<<endl<<"Enter Input/hex File name:";
		cin>>in_file[no_files];
		no_files++;
		cout<<endl<<"More files?(0/1):";
		cin>>m;
		cout<<endl;
	}
	linkingLoader(in_file, no_files, 0x1000);
	simulate();
	estab.writeToFile("estab.txt");
    return 0;
}
