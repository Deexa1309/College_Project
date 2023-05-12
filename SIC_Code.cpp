//Relocating SIC assembler using Bitmask

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip> 
#include <vector>
#include <cmath>
#include <map> 
#include <stdlib.h>
#define MAX 20

using namespace std;

int s[MAX][12]={0};
int row=0,col=0;

typedef struct{
    string op,address,obcode,loc,label,parameter,x;
    int siz;
}instruction;

vector<instruction> line; 
bool isop(string);
string optab(string);
string ten_to_hex(int);
int hex_to_ten(string);
string binary_to_hex(const string);
string hex_to_binary(const string);
string ASCII(char);
void pass1(char * filen);
void pass2();
void record();
string hex_to_op(string);
void bitmask();
char convert(int,int);
char hex(int);

int main(int argc, char* argv[])
{
    
	char file_name[32];
	if(argc==1)
	{
		cout<<"PLEASE ENTER FILE NAME\n";
		cin>>file_name;
	}
	else
		strcpy(file_name,argv[1]);

    pass1(file_name);
    pass2();
    record();
    bitmask();
    cout<<"\nAssembler executed successfully\n\nThe object program has been generated.";
    cout<<"\n\"record.txt\" contains the object program without bitmask.";
    cout<<"\n\"finalobjectcode.txt\" contains the object program with bitmask.\n";
    system("pause");
    
    return 0;
}

void pass1(char * filen)
{
    ifstream file;
    instruction temp;
    string lip;
    int loc_num=0,end_count=0;
    file.open(filen);

    while(end_count!=1)
    {
        temp.address="";temp.label="";temp.loc="";temp.obcode="";
        temp.op="";temp.parameter="";temp.siz=0;temp.x="0";

        file>>lip;
        if(lip=="END")
        {
            temp.op="END";
            file>>lip;
            temp.parameter=lip;
            end_count++;
            temp.loc=ten_to_hex(loc_num);
            for(int k=temp.loc.length();k<4;k++)
    			temp.loc="0"+temp.loc;
            line.push_back(temp);
        }
        else if(isop(lip))
        {
            temp.op=optab(lip);
            temp.loc=ten_to_hex(loc_num);
            for(int k=temp.loc.length();k<4;k++)
    			temp.loc="0"+temp.loc;
            loc_num+=3;
            if(lip!="RSUB")
            {
                file>>lip;
                temp.parameter=lip;
            }
            line.push_back(temp);
        }
        else
        {
            temp.label=lip;
            file>>lip;
            if(lip=="START")
            {
                temp.op="START";
                file>>lip;
                temp.loc=lip;
                temp.parameter=lip;
                loc_num=hex_to_ten(lip);
                line.push_back(temp);
            }
            else if(lip=="BYTE"||lip=="RESW"||lip=="RESB"||lip=="WORD")
            {
                temp.op=lip;
                if(lip=="WORD")
                {
                    int x;
                    temp.loc=ten_to_hex(loc_num);
                    for(int k=temp.loc.length();k<4;k++)
    					temp.loc="0"+temp.loc;
                    loc_num+=3;
                    temp.op=lip;
                    file>>x;
                    temp.siz=x;
                    line.push_back(temp);
                }
                else if(lip=="BYTE")
                {
                    temp.loc=ten_to_hex(loc_num);
                    for(int k=temp.loc.length();k<4;k++)
    					temp.loc="0"+temp.loc;
                    temp.op=lip;
                    file>>lip;
                    if(lip[0]=='C')
                        loc_num+=3;
                    else if(lip[0]=='X')
                        loc_num+=1;
                    temp.parameter=lip;
                    line.push_back(temp);
                }
                else if(lip=="RESW")
                {
                    temp.loc=ten_to_hex(loc_num);
                    for(int k=temp.loc.length();k<4;k++)
    					temp.loc="0"+temp.loc;
                    temp.op=lip;
                    file>>lip;
                    int conversion=stoi(lip);
                    loc_num+=conversion*3;
                    temp.parameter=lip;
                    line.push_back(temp);
                }
                else if(lip=="RESB")
                {
                    int x;
                    temp.op=lip;
                    file>>x;
                    temp.loc=ten_to_hex(loc_num);
                    for(int k=temp.loc.length();k<4;k++)
    					temp.loc="0"+temp.loc;
                    loc_num+=x;
                    temp.siz=x;
                    line.push_back(temp);
                }
            }
            else if(isop(lip))
            {
                temp.op=optab(lip);
                temp.loc=ten_to_hex(loc_num);
                for(int k=temp.loc.length();k<4;k++)
    				temp.loc="0"+temp.loc;
                loc_num+=3;
                file>>lip;
                temp.parameter=lip;
                line.push_back(temp);
            }
        }
    }
}

string optab(string mnem)
{
    map<string,string> r;
    r["ADD"]="18";r["ADDF"]="58";
    r["ADDR"]="90";r["AND"]="40";
    r["CLEAR"]="B4";r["COMP"]="28";
    r["COMPF"]="88";r["COMPR"]="A0";
    r["DIV"]="24";r["DIVF"]="64";
    r["DIVR"]="9C";r["FIX"]="C4";
    r["FLOAT"]="C0";r["HIO"]="F4";
    r["J"]="3C";r["JEQ"]="30";
    r["JGT"]="34";r["JLT"]="38";
    r["JSUB"]="48";r["LDA"]="00";
    r["LDB"]="68";r["LDCH"]="50";
    r["LDF"]="70";r["LDL"]="08";
    r["LDS"]="6C";r["LDT"]="74";
    r["LDX"]="04";r["LPS"]="D0";
    r["MUL"]="20";r["MULF"]="60";
    r["MULR"]="98";r["NORM"]="C8";
    r["OR"]="44";r["RD"]="D8";
    r["RMO"]="AC";r["RSUB"]="4C";
    r["SHIFTL"]="A4";r["SHIFTR"]="A8";
    r["SIO"]="F0";r["SSK"]="EC";
    r["STA"]="0C";r["STB"]="78";
    r["STCH"]="54";r["STF"]="80";
    r["STI"]="D4";r["STL"]="14";
    r["STS"]="7C";r["STSW"]="E8";
    r["STT"]="84";r["STX"]="10";
    r["SUB"]="1C";r["SUBF"]="5C";
    r["SUBR"]="94";r["SVC"]="B0";
    r["TD"]="E0";r["TIO"]="F8";
    r["TIX"]="2C";r["TIXR"]="B8";
    r["WD"]="DC";
    string opcode;

    opcode=r[mnem];
    return opcode;
}

bool isop(string lib)
{
    map<string,string> r;
    r["ADD"]="18";r["ADDF"]="58";
    r["ADDR"]="90";r["AND"]="40";
    r["CLEAR"]="B4";r["COMP"]="28";
    r["COMPF"]="88";r["COMPR"]="A0";
    r["DIV"]="24";r["DIVF"]="64";
    r["DIVR"]="9C";r["FIX"]="C4";
    r["FLOAT"]="C0";r["HIO"]="F4";
    r["J"]="3C";r["JEQ"]="30";
    r["JGT"]="34";r["JLT"]="38";
    r["JSUB"]="48";r["LDA"]="00";
    r["LDB"]="68";r["LDCH"]="50";
    r["LDF"]="70";r["LDL"]="08";
    r["LDS"]="6C";r["LDT"]="74";
    r["LDX"]="04";r["LPS"]="D0";
    r["MUL"]="20";r["MULF"]="60";
    r["MULR"]="98";r["NORM"]="C8";
    r["OR"]="44";r["RD"]="D8";
    r["RMO"]="AC";r["RSUB"]="4C";
    r["SHIFTL"]="A4";r["SHIFTR"]="A8";
    r["SIO"]="F0";r["SSK"]="EC";
    r["STA"]="0C";r["STB"]="78";
    r["STCH"]="54";r["STF"]="80";
    r["STI"]="D4";r["STL"]="14";
    r["STS"]="7C";r["STSW"]="E8";
    r["STT"]="84";r["STX"]="10";
    r["SUB"]="1C";r["SUBF"]="5C";
    r["SUBR"]="94";r["SVC"]="B0";
    r["TD"]="E0";r["TIO"]="F8";
    r["TIX"]="2C";r["TIXR"]="B8";
    r["WD"]="DC";
    if(r.find(lib)==r.end())
        return false;
    return true;
}

string ten_to_hex(int tennum)
{
    string hexnum="";

    for(;tennum!=0;tennum=(tennum-tennum%16)/16)
    {
        switch(tennum%16)
        {
            case 0:
                hexnum="0"+hexnum;
                break;
            case 1:
                hexnum="1"+hexnum;
                break;
            case 2:
                hexnum="2"+hexnum;
                break;
            case 3:
                hexnum="3"+hexnum;
                break;
            case 4:
                hexnum="4"+hexnum;
                break;
            case 5:
                hexnum="5"+hexnum;
                break;
            case 6:
                hexnum="6"+hexnum;
                break;
            case 7:
                hexnum="7"+hexnum;
                break;
            case 8:
                hexnum="8"+hexnum;
                break;
            case 9:
                hexnum="9"+hexnum;
                break;
            case 10:
                hexnum="A"+hexnum;
                break;
            case 11:
                hexnum="B"+hexnum;
                break;
            case 12:
                hexnum="C"+hexnum;
                break;
            case 13:
                hexnum="D"+hexnum;
                break;
            case 14:
                hexnum="E"+hexnum;
                break;
            case 15:
                hexnum="F"+hexnum;
                break;
        }
    }

    return hexnum;
}

int hex_to_ten(string hexnum)
{
    int tennum=0;

    for(int i=0,pos=hexnum.length(),j=pos-1;i<pos;i++,j--)
    {
        switch(hexnum[i])
        {
            case '0':
                tennum+=0*pow(16.0,j);
                break;
            case '1':
                tennum+=1*pow(16.0,j);
                break;
            case '2':
                tennum+=2*pow(16.0,j);
                break;
            case '3':
                tennum+=3*pow(16.0,j);
                break;
            case '4':
                tennum+=4*pow(16.0,j);
                break;
            case '5':
                tennum+=5*pow(16.0,j);
                break;
            case '6':
                tennum+=6*pow(16.0,j);
                break;
            case '7':
                tennum+=7*pow(16.0,j);
                break;
            case '8':
                tennum+=8*pow(16.0,j);
                break;
            case '9':
                tennum+=9*pow(16.0,j);
                break;
            case 'A':
                tennum+=10*pow(16.0,j);
                break;
            case 'B':
                tennum+=11*pow(16.0,j);
                break;
            case 'C':
                tennum+=12*pow(16.0,j);
                break;
            case 'D':
                tennum+=13*pow(16.0,j);
                break;
            case 'E':
                tennum+=14*pow(16.0,j);
                break;
            case 'F':
                tennum+=15*pow(16.0,j);
                break;
        }
    }

    return tennum;
}

void pass2()
{
    for(int i=0;i<line.size();i++)
    {
        string temp;

        if(line[i].op=="START")
            line[i].obcode="";
        else if(line[i].op=="4C")
            line[i].obcode="4C0000";
        else if(line[i].op=="RESW"||line[i].op=="RESB")
            line[i].obcode="";
        else if(line[i].op=="WORD")
        {
            line[i].obcode=ten_to_hex(line[i].siz);
            for(int k=line[i].obcode.length();k<6;k++)
                line[i].obcode="0"+line[i].obcode;
        }
        else if(line[i].parameter!="")
        {
            line[i].obcode+=line[i].op;
            temp=line[i].parameter.substr(line[i].parameter.length()-2,2);
            if(temp==",X")
            {
                line[i].x="1";
                line[i].parameter=line[i].parameter.substr(0,line[i].parameter.length()-2);
                for(int j=0;j<line.size();j++)
                {
                    if(line[i].parameter==line[j].label)
                        line[i].obcode+=binary_to_hex(line[i].x+hex_to_binary(line[j].loc).substr(1,15));
                }
            }
            else if(line[i].op=="END")
                line[i].obcode="";
            else if(line[i].op=="BYTE")
            {
                if(line[i].parameter[0]=='C')
                    line[i].obcode=ASCII(line[i].parameter[2])+ASCII(line[i].parameter[3])+ASCII(line[i].parameter[4]);
                if(line[i].parameter[0]=='X')
                    line[i].obcode=line[i].parameter.substr(2,2);
            }
            else
            {
                for(int j=0;j<line.size();j++)
                {
                    if(line[i].parameter==line[j].label)
                        line[i].obcode+=binary_to_hex(line[i].x+hex_to_binary(line[j].loc).substr(1,15));
                }
            }
        }
    }
}

string binary_to_hex(const string binnum)
{
    string hexnum="",temp="",fixbinnum=binnum;
    int cou;

    if(4-(binnum.length()%4)==4)
        cou=0;
    else
        cou=4-(binnum.length()%4);
    for(int i=0;i<cou;i++)
        fixbinnum="0"+fixbinnum;
    for(int j=0;j+3<fixbinnum.length();j+=4)
    {
        temp=fixbinnum.substr(j,4);
        if(temp=="0000")
            hexnum+="0";
        else if(temp=="0001")
            hexnum+="1";
        else if(temp=="0010")
            hexnum+="2";
        else if(temp=="0011")
            hexnum+="3";
        else if(temp=="0100")
            hexnum+="4";
        else if(temp=="0101")
            hexnum+="5";
        else if(temp=="0110")
            hexnum+="6";
        else if(temp=="0111")
            hexnum+="7";
        else if(temp=="1000")
            hexnum+="8";
        else if(temp=="1001")
            hexnum+="9";
        else if(temp=="1010")
            hexnum+="A";
        else if(temp=="1011")
            hexnum+="B";
        else if(temp=="1100")
            hexnum+="C";
        else if(temp=="1101")
            hexnum+="D";
        else if(temp=="1110")
            hexnum+="E";
        else if(temp=="1111")
            hexnum+="F";
    }
    return hexnum;
}

string hex_to_binary(const string hexnum)
{
    string binnum="";

    for(int i=0;hexnum[i]!='\0';i++)
    {
        switch(hexnum[i])
        {
            case '0':
                binnum+="0000";
                break;
            case '1':
                binnum+="0001";
                break;
            case '2':
                binnum+="0010";
                break;
            case '3':
                binnum+="0011";
                break;
            case '4':
                binnum+="0100";
                break;
            case '5':
                binnum+="0101";
                break;
            case '6':
                binnum+="0110";
                break;
            case '7':
                binnum+="0111";
                break;
            case '8':
                binnum+="1000";
                break;
            case '9':
                binnum+="1001";
                break;
            case 'A':
                binnum+="1010";
                break;
            case 'B':
                binnum+="1011";
                break;
            case 'C':
                binnum+="1100";
                break;
            case 'D':
                binnum+="1101";
                break;
            case 'E':
                binnum+="1110";
                break;
            case 'F':
                binnum+="1111";
                break;
        }
    }
    return binnum;
}

string ASCII(char li)
{
    map<char,string> r;
    r['A']="41";
    r['B']="42";
    r['C']="43";
    r['D']="44";
    r['E']="45";
    r['F']="46";
    r['G']="47";
    r['H']="48";
    r['I']="49";
    r['J']="4A";
    r['K']="4B";
    r['L']="4C";
    r['M']="4D";
    r['N']="4E";
    r['O']="4F";
    r['P']="50";
    r['Q']="51";
    r['R']="52";
    r['S']="53";
    r['T']="54";
    r['U']="55";
    r['V']="56";
    r['W']="57";
    r['X']="58";
    r['Y']="59";
    r['Z']="5A";
    string as;

    as=r[li];
    return as;
}


void record()
{
    string length,endadd;
    int l[100]={0};
    ofstream out("record.txt");
    out<<"Record\n"<<endl;
    length=ten_to_hex(hex_to_ten(line[line.size()-1].loc)-hex_to_ten(line[0].loc));
    for(int k=length.length();k<6;k++)
    {
    	length="0"+length;
	}
    
    string prog_name=line[0].label;
    for(int k=prog_name.length();k<6;k++)
    {
    	prog_name=prog_name+" ";
	}
    
    out<<"H^"<<prog_name<<"^00"<<line[0].parameter<<"^"<<length<<endl; 
    for(int i=1,insnum=0,j=1;i<=line.size()-1;i++)
    {	
        l[j]+=line[i].obcode.length();
        if(line[i].obcode!="")
        insnum++;
        if(insnum==10)
        {
            l[j]=l[j]/2;
            j++;
            insnum=0;
        }
        else if(line[i].obcode=="" && (i+1) == line.size()-1)
		{
			if(l[j]!=0)
			{
				l[j]=l[j]/2;
				break;
			}
		}
        else if(line[i].obcode=="")
        {
        	if(l[j]!=0)
        	{
        		l[j]=l[j]/2;
        		j++;
            	insnum=0;
			}
		}
    }

    for(int i=1,insnum=0,j=1;i<line.size()-1;i++)
    {
    	if(insnum==0 && line[i].obcode=="")
    	{
    		
		}
        else if(insnum==0)
        {
        	string dup=ten_to_hex(l[j]);
        
            for(int k=dup.length();k<2;k++)
            	dup="0"+dup;
            out<<"T^00"<<line[i].loc<<"^"<<dup<<"^"<<line[i].obcode;
          
            
            if(line[i].op!="WORD" && line[i].op!="BYTE" && line[i].obcode!="4C0000")
            {
            	s[row][col]=1;
            }
            col++;
            j++;
            insnum++;
        }
        else if(insnum==10 || line[i].obcode=="" && line[i-1].obcode!="" )
        {
            out<<endl;
            row++;
            col=0;
            insnum=0;
            i--;
        }
        else
        {
            if(line[i].obcode!=""){
                out<<"^"<<line[i].obcode;
                if(line[i].op!="WORD" && line[i].op!="BYTE" && line[i].op!="4C")
                {
            		s[row][col]=1;
                }
                col++;
            }
            insnum++;
            
			
        }
    }
    out<<endl;
    for(int i=0;i<line.size();i++)
    {
        if(line[line.size()-1].parameter==line[i].label)
            endadd=line[i].loc;
    }
    out<<"E^00"<<endadd<<endl;
    out.close();
    
}


void bitmask()
{
	int r=0;
	FILE *p,*q;
	p=fopen("record.txt","r");
	q=fopen("finalobjectcode.txt","w");
	char* line = (char*)malloc(100) ;
	int len=0;
	fprintf(q,"%s\n\n","Final Object Program");
	while(fgets(line,100,p)!=NULL)
	{
		int k=strlen(line);
		if(line[0]=='H')
		{
			fprintf(q,"%s",line);
		}
		else if(line[0]=='T')
		{
			
			char s1[4];
			
			s1[0]='^';	
			s1[1]=convert(r,0);
			s1[2]=convert(r,4);
			s1[3]=convert(r,8);
		
			for(int j=0,i=0;j<k+4;j++)
			{
				if(j<11)
				{	
					fprintf(q,"%c",line[j]);	
				}
                
				else if (j<15 && j>10)
				{
					fprintf(q,"%c",s1[i]);
					i++;
				}
                
				else
				{	
					fprintf(q,"%c",line[j-4]);	
				}
              
			}
			r++;
		
		}
		else if(line[0]=='E')
		{
			fprintf(q,"%s",line);
		}
		
	}

	fclose(q);
	fclose(p);
	//finalobjectcode.txt FILE IS READY!!	

}


char convert(int r,int i)
{
	int value=0,j=0;
	char a;
	value=(s[r][i]*2*2*2)+(s[r][i+1]*2*2)+(s[r][i+2]*2)+(s[r][i+3]);
	a=hex(value);
	return a; 
}

char hex(int value)
{
	char a;
	switch(value)
	{
		case 0:
			a='0';
			break;
		case 1:
			a='1';
			break;
		case 2:
			a='2';
			break;
		case 3:
			a='3';
			break;
		case 4:
			a='4';
			break;
		case 5:
			a='5';
			break;
		case 6:
			a='6';
			break;
		case 7:
			a='7';
			break;
		case 8:
			a='8';
			break;
		case 9:
			a='9';
			break;
		case 10:
			a='A';
			break;
		case 11:
			a='B';
			break;
		case 12:
			a='C';
			break;
		case 13:
			a='D';
			break;
		case 14:
			a='E';
			break;
		case 15:
			a='F';
			break;		
	}
	return a;
}
