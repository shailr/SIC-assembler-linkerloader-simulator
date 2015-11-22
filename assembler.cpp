
#include "common.h"

int start_add = 0, prog_length = 0;

OpcodeTable optab;
SymTable symtab,estab;

void pass1(string in_file)
{
    cout<<"Running pass 1..."<<endl;
	ifstream in((in_file + ".txt").c_str());
	ofstream inter((in_file + ".i").c_str());
	char line[100];
    char* tokens[6];
	int line_no = 1;
	int locctr = 0, prev_locctr = 0;
	int no_tokens = 0, sc = 0, temp = 0;
	string label, operation, operand, comment;
	string prog_name, loc;
	try{
		while(in && operation!="END")
		{
			label.clear(),operation.clear(),operand.clear(),comment.clear();
			in.getline(line,100);
			if(line[0]==';')
			{
				comment = line;
				//Neglect complete comment...
			}
			else
			{
				no_tokens = extractTokens(line,tokens,'\t');
				label.assign(tokens[0]);
				if(optab.search(label)!=-1)
					throw 1;
					
				operation.assign(tokens[1]);
					
				if(no_tokens==3)
				{
					if(tokens[2][0]==';')
						comment.assign(tokens[2]);
					else
						operand.assign(tokens[2]);
				}
				else if(no_tokens==4)
				{
					operand.assign(tokens[2]);
					comment.assign(tokens[3]);
				}
				
				if(line_no==1)
				{
					if(operation=="START")
					{
						prog_name = label;
						if(prog_name.length()>10)
							throw 8;
						operation = "START";
						if(operand[0]=='x')
						{
							stringstream ss(operand.substr(1,operand.length()-1));
							ss>>hex>>locctr;
						}
						else if(operand[0]=='#')
						{
							stringstream ss(operand.substr(1,operand.length()-1));
							ss>>dec>>locctr;
						}
						start_add = prev_locctr = locctr;
					}
					else
						cout<<"No Prog Name and Start Address specified. Default assumed."<<endl;
				}
				else if(optab.search(operation)!=-1)
				{
					prev_locctr = locctr;
					locctr += 3;
				}
				else if(operation=="RESW")
				{
					stringstream ss(operand);
					ss>>dec>>temp;
					prev_locctr = locctr;
					locctr += 3*temp;
				}
				else if(operation=="RESB")
				{
					stringstream ss(operand);
					ss>>dec>>temp;
					prev_locctr = locctr;
					locctr += temp;
				}
				else if(operation=="WORD")
				{
					prev_locctr = locctr;
					locctr += 3;
				}
				else if(operation=="BYTE")
				{
					int bytecount=0;
					if(operand[0]=='C')
	                {
	                    temp=1;
	                    if(operand[temp]=='\'')
	                    {
	                        temp++;
	                        while(operand[temp]!='\'')
	                        {
	                            bytecount++;
	                            temp++;
	                        }
	                    }
	                    else
	                    {
	                        throw 4;
	                    }
	                }    
                    else if(operand[0]=='X')
                    {
                        bytecount=1;
                    }
                    else
                    {
                        throw 5;
                    }
					prev_locctr = locctr;
                    locctr+=(bytecount);
				}
				else if(operation=="EXTDEF")
				{
					prev_locctr = locctr;
					if(estab.search(operand)==-1)
					{
						if(symtab.search(operand)==-1)
						{
							estab.addSym(operand,"na");
						}
						else
						{
							estab.addSym(operand, symtab.sym_tab[symtab.search(operand)].address);
						}
					}
				}
				else if(operation=="EXTREF")
				{
					prev_locctr = locctr;
					if(estab.search(operand)==-1)
					{
						estab.addSym(operand,"na");
					}
				}
				else if(operation=="END")
				{
				}
				else
				{
					throw 6;
				}
				
				if(!label.empty())
				{
					stringstream ss;
					ss<<uppercase<<hex<<prev_locctr;
					ss>>loc;
					if(symtab.addSym(label, loc)!=1)
						throw 3;
					if(estab.search(label)!=-1 && estab.sym_tab[estab.search(label)].address=="na")
					{
						estab.sym_tab[estab.search(label)].address = loc;
					}
				}
				
				//cout<<uppercase<<hex<<prev_locctr<<"\t"<<dec<<line_no<<"\t"<<label<<"\t"<<operation<<"\t"<<operand<<endl;
				inter<<setw(4)<<setfill('0')<<uppercase<<hex<<prev_locctr<<"\t"<<setw(2)<<setfill('0')<<dec<<line_no<<"\t"<<label<<"\t"<<operation<<"\t"<<operand<<endl;
				line_no++;
			}
		}
		prog_length = locctr - start_add;
	}
	catch(int i)
	{
		cout<<endl<<"ERROR::"<<i<<endl;
	}
	
	in.close();
	inter.close();
    cout<<"Pass 1 over..."<<endl;
}

void pass2(string in_file)
{
    cout<<"Running Pass 2..."<<endl;
	ifstream inter((in_file + ".i").c_str());
	ofstream out((in_file + ".o").c_str());
	char line[100];
    char* tokens[6];
	int line_no = 1;
	int locctr = 0, prev_locctr = 0, operand_addr = 0;
	int no_tokens = 0, sc = 0, temp = 0;
	string loc, no, label, operation, operand;
	string prog_name;
	if(inter)
	{
		loc.clear(), no.clear(), label.clear(), operation.clear(), operand.clear();
		inter.getline(line,100);
		no_tokens = extractTokens(line,tokens,'\t');
		if(no_tokens==5)
		{
			loc.assign(tokens[0]);
			no.assign(tokens[1]);
			label.assign(tokens[2]);
			operation.assign(tokens[3]);
			operand.assign(tokens[4]);
			if(operation=="START")
			{
				prog_name = label;
				//cout<<"H"<<"^"<<prog_name<<"^"<<setw(4)<<setfill('0')<<uppercase<<hex<<operand<<"^"<<setw(4)<<setfill('0')<<uppercase<<hex<<prog_length<<endl;
				out<<"H"<<"^"<<prog_name<<"^"<<setw(4)<<setfill('0')<<uppercase<<hex<<operand<<"^"<<setw(4)<<setfill('0')<<hex<<prog_length<<endl;
			}
		}
		while(inter && operation!="END")
		{
			loc.clear(), no.clear(), label.clear(), operation.clear(), operand.clear();
			inter.getline(line,100);
			no_tokens = extractTokens(line,tokens,'\t');
			loc.assign(tokens[0]);
			no.assign(tokens[1]);
			label.assign(tokens[2]);
			operation.assign(tokens[3]);
			operand.assign(tokens[4]);
			
			if(optab.search(operation)>=0)
			{
                string opcode = optab.codes[optab.search(operation)].opcode;
				if(!operand.empty())
				{
					if(symtab.search(operand)>=0)
                    {
                    	stringstream ss(symtab.sym_tab[symtab.search(operand)].address);
                        ss>>hex>>operand_addr;
		                //cout<<"T"<<"^"<<loc<<"^"<<opcode<<"^"<<operand_addr<<endl;
		                out<<"T"<<"^"<<loc<<"^"<<opcode<<"^"<<operand_addr<<endl;
                    }
                    else if(estab.search(operand)>=0)
                    {
		                //cout<<"T"<<"^"<<loc<<"^"<<opcode<<"^"<<('@'+operand)<<endl;
		                out<<"T"<<"^"<<loc<<"^"<<opcode<<"^"<<('@'+operand)<<endl;
                    }
                    else
                    {
                        operand_addr =0;
		                cout<<"Error: Symbol not found"<<endl;
		                out<<"Error: Symbol not found"<<endl;
		                throw 0;
                    }
				}
                else
                {
                    operand_addr =0;
	                cout<<"Error: No operand"<<endl;
	                out<<"Error: No operand"<<endl;
		            throw 1;
                }
			}
            else if(operation=="BYTE")
            {
                //cout<<"in byte"<<endl;
                if(operand[0]=='C')
                {
					vector<char> chars(operand.c_str(), operand.c_str() + operand.length() + 1u);
                    char *temp = &chars[0];
                    temp+=2;
	                //cout<<"T"<<"^"<<loc<<"^";
	                out<<"T"<<"^"<<loc<<"^";
                    while( (*temp)!='\'')
                    {
		                //cout<<uppercase<<hex<<(int)(*temp);
		                out<<uppercase<<hex<<(int)(*temp);
                        temp+=1;
                    }
	                //cout<<endl;
	                out<<endl;
                }
                else if(operand[0]=='X')
                {
					vector<char> chars(operand.c_str(), operand.c_str() + operand.length() + 1u);
                    char *temp = &chars[0];
                    temp+=2;
	                //cout<<"T"<<"^"<<loc<<"^";
	                out<<"T"<<"^"<<loc<<"^";
                    while( (*temp)!='\'')
                    {
		                //cout<<uppercase<<hex<<(int)(*temp);
		                out<<uppercase<<hex<<(int)(*temp);
                        temp+=1;
                    }
	                //cout<<endl;
	                out<<endl;
                }
				else{}
            }
            else if(operation=="WORD")
            {
                //cout<<"in word"<<endl;
                int v;
                stringstream ss(operand);
                ss>>v;
                //cout<<"T"<<"^"<<loc<<"^"<<setw(6)<<setfill('0')<<uppercase<<hex<<v<<endl;
                out<<"T"<<"^"<<loc<<"^"<<setw(6)<<setfill('0')<<uppercase<<hex<<v<<endl;
            }
            else if(operation=="EXTDEF")
            {
				if(!operand.empty())
				{
					if(estab.search(operand)>=0)
                    {
                    	stringstream ss(estab.sym_tab[estab.search(operand)].address);
                        ss>>hex>>operand_addr;
		                //cout<<"D"<<"^"<<operand<<"^"<<operand_addr<<endl;
		                out<<"D"<<"^"<<operand<<"^"<<operand_addr<<endl;
                    }
                    else
                    {
                        operand_addr =0;
		                cout<<"Error: Symbol not found"<<endl;
		                out<<"Error: Symbol not found"<<endl;
		                throw 0;
                    }
				}
                else
                {
                    operand_addr =0;
	                cout<<"Error: No operand"<<endl;
	                out<<"Error: No operand"<<endl;
		            throw 1;
                }
            }
            else if(operation=="EXTREF")
            {
            	if(!operand.empty())
				{
					if(estab.search(operand)>=0)
                    {
		                //cout<<"R"<<"^"<<operand<<endl;
		                out<<"R"<<"^"<<operand<<endl;
                    }
                    else
                    {
                        operand_addr =0;
		                cout<<"Error: Symbol not found"<<endl;
		                out<<"Error: Symbol not found"<<endl;
		                throw 0;
                    }
				}
                else
                {
                    operand_addr =0;
	                cout<<"Error: No operand"<<endl;
	                out<<"Error: No operand"<<endl;
		            throw 1;
                }
            }
            else{}
		}
		
        //cout<<"E"<<"^"<<start_add<<endl;
        out<<"E"<<"^"<<start_add<<endl;
	}
	
	inter.close();
	out.close();
    cout<<"Pass 2 over..."<<endl;
}

int main()
{
	string in_file;
	cout<<endl<<"Enter Input/Source File name:";
	cin>>in_file;
	cout<<endl;
	pass1(in_file);
	cout<<endl;
    pass2(in_file);
    
    symtab.writeToFile("symtab.txt");
    //estab.writeToFile("estab_assembler.txt");
    return 0;
}
