#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>  
#include <sstream>
#include<Windows.h>
#include <stddef.h>
using namespace std;


//Globally Declared File Objects
ifstream inFile; //Input File
ifstream inFilex; //For symbol table
ofstream outFile;//Output File

string table[200][4]; //Implies Symbol
int iteratorr;		//Table & its iteratorr

int RA; //Implies relative Address.

string quadruple[100][4]; //Implies Quadruple
int iteratorr2;		//& its iteratorr


class symbolTable{

	
public:
	//symbolTable();
	void fill(string lex);
	int lookup(string lex);
	void insert(int row,int col,string le);
	void addArray(string nam,string params);
	void printTable();
	string returnType(string tokk);

	string getRA(string sx);//getting Relative Address of the variable
};

void symbolTable::fill(string lex)
{
	table[iteratorr][0]=lex;

	stringstream ss;
	ss << RA;
	string str = ss.str();

	table[iteratorr][1]=str;
	table[iteratorr][3]="INT";
	iteratorr=iteratorr+1;
	RA=RA+4;
}
int symbolTable::lookup(string y)
{
	for(int i=0;i<=iteratorr;i++){
			if(table[i][0]==y)
			{
				return i;
			}
		
	}
}
void symbolTable::insert(int r,int c,string le)
{
	table[r][c]=le;
}
void symbolTable::printTable()
{
	ofstream ii;
	ii.open("symboltable.txt");
	for(int i=0;i<=iteratorr;i++){
		for(int j=0;j<4;j++){
			cout<<table[i][j]<<"  ";
			ii<<table[i][j]<<" ";
		}
		cout<<endl<<endl;
	}
}
string symbolTable::getRA(string sx)
{
	for(int i=0;i<=iteratorr;i++)
	{
		if(table[i][0]==sx)
			{
				return table[i][1];
			}
	}
}

void symbolTable::addArray(string nam,string params)
{
	table[iteratorr][0]=nam;
	stringstream ss;
	ss << RA;
	string str = ss.str();
	table[iteratorr][1]=str;
	table[iteratorr][2]=params;
	table[iteratorr][3]="&";
	iteratorr=iteratorr+1;
	RA=RA+4;
}
string symbolTable::returnType(string tokk)
{
	int tmp=lookup(tokk);
	return table[tmp][3];
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class lex_Analyser{

private:
	char input_Words[100];  //To Store chars From Source Program.
	char temp[100];

	int seq1; //for int pehle ay
	int seq2; //for id bad m ay
	string lexeme;

	symbolTable *tbl;
public:
	lex_Analyser();
	//Main Function
	void file_Reader();
	//Intermediate Functions.
	void check_Pattern();
	void make_Lex();
	//To check Whether its a comment or not.
	void check_Comment();
	bool isOperator(char k);

	string nextToken();
};
lex_Analyser::lex_Analyser()
{
	for(int i=0;i<100;++i){
		input_Words[i]='\0';
		temp[i]='\0';
	}
	
	seq1=seq2=0;
	iteratorr=0;
	RA=0;
}
void lex_Analyser::file_Reader()
{
	inFile.open("Input_File.txt");
	//Checking For File Errors.If Any.
	if(inFile.fail())
	{
		cerr << "Error. No input File Found" <<endl;
		exit(1);
	}

	//Now opening the Output File.
	//Opening The Output File

	outFile.open("Output.txt");

	if(outFile.fail())
	{
		cerr <<"Error. No output File Found"<<endl;
	}


	//Main Program Starts Here.
	while(!inFile.eof())
	{
		inFile.getline(input_Words,100); //Assuming Max. 100 chars can be on 1 line.
		check_Comment(); //IF its a comment remove it
		check_Pattern();
	}	
	//Filling Symbol Table
	inFilex.open("Output.txt");
	if(inFilex.fail()){	cerr << "Error. No input File Found" <<endl<<endl;	}
	else{
		string T;//type;
		string ide;//identifier
		string name;
		while(inFilex.eof()==false)
		{
			string tm=nextToken();	//pehle id a jae phr int ay then what??so no bool

			if(tm=="int"){
				seq1=1;
			T="int";}
			else if(tm=="id")
			{	
				ide="id";
				name=lexeme;
				if(seq1==1)
				{	seq2=2;	
				}	
				else{
					lexeme="\0";
				}
			}
			else if(tm=="[")
			{
				if(T=="int" && ide=="id")//conformed
				{
					string nuum=nextToken();
					if(nuum=="num")
					{
						string lex=lexeme;  //got the lexeme;
						nextToken();
						nuum=nextToken();
						if(nuum==";")
						{
							tbl->addArray(name,lex);
							T=ide=name="\0";
							seq1=seq2=0;
							lexeme="\0";
						}
					}
				}
			}
			else if(tm==";"){	
				
				if(seq1==1 && seq2==2)
				{
					tbl->fill(lexeme);
					seq1=seq2=0;
					lexeme="\0";
					T="\0";
				}
			}
			else {
				lexeme="\0";
				seq1=seq2=0;
				T="\0";
			}
		}

		tbl->printTable();
	}

}
string lex_Analyser::nextToken()
{
	string tmp;
	char iff[100]={0};
	inFilex.getline(iff,100);

	if(iff[2]=='i' && iff[3]=='d')  //Got the identifier
		{
			int k=7;
			while(iff[k]!=' ') //Get the lexeme
			{
				lexeme=lexeme+iff[k]; //Concatenation
				k++;
			}
			tmp="id";
		}

	else if(iff[2]=='i' && iff[3]=='n' && iff[4]=='t')
	{
		tmp="int";
	}
	else if(iff[2]==';')
	{
		tmp=";";
	}
	else if(iff[2]=='n' && iff[3]=='u' && iff[4]=='m')
	{
		lexeme="\0";
			int k=8;
			while(iff[k]!=' ') //Get the lexeme
			{
				lexeme=lexeme+iff[k]; //Concatenation
				k++;
			}
			tmp="num";
	}
	else if(iff[2]=='[')
	{
		tmp="[";
	}
	return tmp;
}
void lex_Analyser::check_Pattern()
{
	int j=0;
	char temp_back; //to store previous word
	char backer;
	char temp_frwd;
	for(int i=0;input_Words[i]!='\0';)
	{
		if(input_Words[i]!=' ' && (isalpha(input_Words[i]) || (isdigit(input_Words[i])) || ((input_Words[i]=='_')&&(isalpha(temp_back))) || ((input_Words[i]=='-')&&(isdigit(input_Words[i+1]))&&(isOperator(backer))) ))
		{
			temp[j]=input_Words[i];
			temp_back=input_Words[i];
			
			input_Words[i]='\0'; //Null it for getting the Next Line.
			j++;
			i++;
			backer=0;
		
		}
		
		else
		{
			make_Lex();
			j=0;
			if(input_Words[i]==' ') //Previously It was if( input_Words[i]==' ' || (!isalpha(input_Words[i])) ) 
			{
				input_Words[i]='\0';
				i++;
			}
			else if( (!isalpha(input_Words[i])) )
			{
				temp[j]=input_Words[i];

				//Now for checking double Operators.
				if(input_Words[i]=='=' || input_Words[i]=='<' || input_Words[i]=='>' || input_Words[i]=='!')
				{
					if(input_Words[i+1]=='=')
					{
						input_Words[i]='\0';
						j++; i++;
						temp[j]=input_Words[i];
					}
				}

				make_Lex();
				backer=input_Words[i];
				input_Words[i]='\0';
				j=0;
				i++;
			}

		}
	}

	if(j!=0)  //For Ex. If our prog. doesn't end on ; or space.(simple else)
	{
		make_Lex();
	}

}
void lex_Analyser::make_Lex()
{
	int k=0;
	bool flag=false;
	//1.MACHINES TO RECOGNIZE KEYWORDS
	//'int' Keyword
	if(temp[0]=='i' && flag==false)
	{
		k++;
		if(temp[k]=='n')
		{
			k++;
			if(temp[k]=='t')
			{
				k++;
				if(temp[k]=='\0')
				{
					//KeyWord int is detected.
					outFile<<"( int , ^ )"<<endl;
					flag=true;
					while(k>=0)
					{
						//Now null the whole temp array for the next word. 
						temp[k]='\0';
						k--;
					}
				}
			}
		}
	}

	//'return' Keyword.
	if(temp[0]=='r' && flag==false)
	{
		k=0;	
		k++;
		if(temp[k]=='e')
		{
			k++;
			if(temp[k]=='t')
			{
				k++;
				if(temp[k]=='u')
				{
					k++;
					if(temp[k]=='r')
					{
						k++;
						if(temp[k]=='n')
						{
							k++;
							if(temp[k]=='\0')
							{
								//KeyWord int is detected.
								outFile<<"( return , ^ )"<<endl;
								flag=true;
								while(k>=0)
								{
									//Now null the whole temp array for the next word. 
									temp[k]='\0';
									k--;
								}
							}
						}
					}
				}
			}
		}
	}
	
	//'if' Keyword.
	if(temp[0]=='i' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='f')
		{
			k++;
			if(temp[k]=='\0')
			{
				//KeyWord int is detected.
				outFile<<"( if , ^ )"<<endl;
				flag=true;
				while(k>=0)
				{
					//Now null the whole temp array for the next word. 
					temp[k]='\0';
					k--;
				}
			}
		}
	}

	//'else' Keyword
	if(temp[0]=='e' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='l')
		{
			k++;
			if(temp[k]=='s')
			{
				k++;
				if(temp[k]=='e')
				{
					k++;
					if(temp[k]=='\0')
					{
						//KeyWord else is detected.
						outFile<<"( else , ^ )"<<endl;
						flag=true;
						while(k>=0)
						{
							//Now null the whole temp array for the next word. 
							temp[k]='\0';
							k--;
						}
					}
				}
			}
		}
	}

	//'void' Keyword
	if(temp[0]=='v' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='o')
		{
			k++;
			if(temp[k]=='i')
			{
				k++;
				if(temp[k]=='d')
				{
					k++;
					if(temp[k]=='\0')
					{
						//KeyWord else is detected.
						outFile<<"( void , ^ )"<<endl;
						flag=true;
						while(k>=0)
						{
							//Now null the whole temp array for the next word. 
							temp[k]='\0';
							k--;
						}
					}
				}
			}
		}
	}

	//'while' Keyword.
	if(temp[0]=='w' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='h')
		{
			k++;
			if(temp[k]=='i')
			{
				k++;
				if(temp[k]=='l')
				{
					k++;
					if(temp[k]=='e')
					{
						k++;
						if(temp[k]=='\0')
						{
							//KeyWord int is detected.
							outFile<<"( while , ^ )"<<endl;
							flag=true;
							while(k>=0)
							{
								//Now null the whole temp array for the next word. 
								temp[k]='\0';
								k--;
							}
						}
					}
				}
			}
		}
	}

	//2.MACHINES TO RECOGNIZE SPECIAL SYMBOLS
	//plus
	if(temp[0]=='+' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( + , ^ )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
	}
	//minus
	if(temp[0]=='-' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')//////////////
		{
			outFile<<"( - , ^ )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
	
		else if(isdigit(temp[k]))
		{
		outFile<<"( num , ";
		for(k=0;temp[k]!='\0';k++)
		{
			outFile<<temp[k];
			temp[k]='\0';
		}
		outFile<<" )"<<endl;
	}
	}
	//multiply
	if(temp[0]=='*' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( * , ^ )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
	}
	//divide
	if(temp[0]=='/' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( / , ^ )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
	}
	//equality
	if(temp[0]=='=' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( = , ^ )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
		else if(temp[k]=='=')
		{
			outFile<<"( ro , EQ )"<<endl; flag=true; //Implies double eq. to
			temp[k]='\0';
			k--;
			temp[k]='\0';
		}
	}
	//less-than
	if(temp[0]=='<' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( ro , LT )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
		else if(temp[k]=='=')
		{
			outFile<<"( ro , LE )"<<endl; flag=true;
			temp[k]='\0';
			k--;
			temp[k]='\0';
		}
	}
	//greater-than
	if(temp[0]=='>' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( ro , GT )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
		else if(temp[k]=='=')
		{
			outFile<<"( ro , GE )"<<endl; flag=true;
			temp[k]='\0';
			k--;
			temp[k]='\0';
		}
	}
	//not-Equal to
	if(temp[0]=='!' && flag==false)
	{
		k=0;
		k++;
		if(temp[k]=='\0')
		{
			outFile<<"( ! , ^ )"<<endl; flag=true;
			k--;
			temp[k]='\0';
		}
		else
		{
			outFile<<"( ro , NE )"<<endl; flag=true;
			temp[k]='\0';
			k--;
			temp[k]='\0';
		}
	}
	//comma

	if(temp[0]==',' && flag==false)
	{
		k=0;
		outFile<<"( , , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}
	//semicolon

	if(temp[0]==';' && flag==false)
	{
		k=0;
		outFile<<"( ; , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}
	//round braces
	if(temp[0]=='(' && flag==false)
	{
		k=0;
		outFile<<"( ( , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}

	if(temp[0]==')' && flag==false)
	{
		k=0;
		outFile<<"( ) , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}
	//square bracket
	if(temp[0]=='[' && flag==false)
	{
		k=0;
		outFile<<"( [ , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}

	if(temp[0]==']' && flag==false)
	{
		k=0;
		outFile<<"( ] , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}
	//curly bracket
	if(temp[0]=='{' && flag==false)
	{
		k=0;
		outFile<<"( { , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}

	if(temp[0]=='}' && flag==false)
	{
		k=0;
		outFile<<"( } , ^ )"<<endl; flag=true;
		temp[k]='\0';		
	}
	//Now This is a Character Since it Doesn't matches with any of the above machine
	k=0;
	if(flag!=true &&  isalpha(temp[k]) && (!isdigit(temp[k])) )
	{
		outFile<<"( id , ";
		for(k=0;temp[k]!='\0';k++)
		{
			outFile<<temp[k];
			temp[k]='\0';
		}
		outFile<<" )"<<endl;
	}

	//Now for checking number
	else if(isdigit(temp[k]))
	{
		outFile<<"( num , ";
		for(k=0;temp[k]!='\0';k++)
		{
			outFile<<temp[k];
			temp[k]='\0';
		}
		outFile<<" )"<<endl;
	}

}
void lex_Analyser::check_Comment()
{
	bool y=false;
	int x=strlen(input_Words);
	for(int i=0;i<x;++i)
	{
		if(input_Words[i]=='/' && input_Words[i+1]=='/')
		{
			y=true;
		}
		if(y==true)
		{
			input_Words[i]='\0';  //Meaning from Comm. starting to end remove it!
		}
	}
}
bool lex_Analyser::isOperator(char kole)
{
	if(kole=='=' ||kole=='>' ||kole=='<' ||kole=='!')
		return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////3 ADDRESS CODE STARTING\\\\\\\\\\\\\\\\\\\

ifstream inFile2;
class Translator{
public:
//Global Variables
string token;
string lexeme;

string prev_Tok;//To hold previous token which is somehow lost
string prev_Lex;
//Asign.Stmt
bool AS;
char operatr;

long long indexx;
string tmp;
string prev1;
string next1;
//
//Selection
string program[1000]; //Array to hold the final program
int arrIterator; //Will be placed as line no in array
long long n; //Global Variable
bool SS;
long long BE_true;
long long BE_false;

bool lost;

int backStack[10]; //To push BE_false on stack for correct backtracking ( Used for IF/Else Stack )
int stackVar;

int backStack2[10]; // ( Used for While Stack )
int stackVar2;

bool ifStack;
bool whileStack;

int S_x;

//Iteration/Loop of While.
bool IS;
int ins_Counter;
long long S_s[10];
int S_i;
int wn; //implies n for while

Translator();

//Function____Calls
string funName;
void argumentList();
long long argumentCounter; 
//For Return Stmt;
bool machine4();


bool machine2();
void nextToken();
string subMachine1();
bool machine1();
void backpatch(long long t,long long n);
void bool_Expression();
bool machine3();
void check_Machines();
void backStack_Push(int x);//for
int backStack_Pop();		//if else

void backStack2_Push(int x);//for
int backStack2_Pop();		//while


bool fileConn();

symbolTable* tbl;

inline bool isInteger(const std::string & s);

string forArray(string l);

bool ckr;
};

Translator::Translator()
{
	indexx=0;
	tmp="t";
	arrIterator=0;
	n=0; 
	ins_Counter=0;
	stackVar=stackVar2=0;
	S_i=0;
	wn=0;
	tbl=new symbolTable();
	
	for(int i=0;i<1000;++i)
		program[i]="\0";
	operatr='\0';

for(int i=0;i<20;i++){
for(int j=0;j<4;j++){
	quadruple[i][j]="\0";
}
}

	iteratorr2=0;
}

inline bool Translator::isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}
void Translator:: nextToken()
{
	char input_File[1000]={0};
	inFile2.getline(input_File,100);
	if(input_File[2]=='i' && input_File[3]=='d')  //Got the identifier
		{
			token=lexeme="\0";
			token="id";
			int k=7;
			while(input_File[k]!=' ') //Get the lexeme
			{
				lexeme=lexeme+input_File[k]; //Concatenation
				k++;
			}
		}
	else if(input_File[2]=='=')
	{
		token=lexeme="\0";
		token="=";
	}
	else if(input_File[2]=='+')
	{
		token=lexeme="\0";
		token="+";
	}
	else if(input_File[2]=='-')
	{
		token=lexeme="\0";
		token="-";
	}
	else if(input_File[2]=='*')
	{
		token=lexeme="\0";
		token="*";
	}
	else if(input_File[2]=='/')
	{
		token=lexeme="\0";
		token="/";
	}
	else if(input_File[2]==';')
	{
		token=lexeme="\0";
		token=";";
	}
	else if(input_File[2]=='(')
	{
		token=lexeme="\0";
		token="(";
	}
	else if(input_File[2]==')')
	{
		token=lexeme="\0";
		token=")";
	}
	else if(input_File[2]=='[')
	{
		token=lexeme="\0";
		token="[";
	}
	else if(input_File[2]=='{')
	{
		token=lexeme="\0";
		token="{";
	}
	else if(input_File[2]=='}')
	{
		token=lexeme="\0";
		token="}";
	}
	else if(input_File[2]=='i' && input_File[3]=='f')
	{
		token=lexeme="\0";
		token="if";
	}
	else if(input_File[2]=='e' && input_File[3]=='l' && input_File[4]=='s' && input_File[5]=='e')
	{
		token=lexeme="\0";
		token="else";
	}
	else if(input_File[2]=='r' && input_File[3]=='o')
	{
		token=lexeme="\0";
		token="ro";
			int k=7;
			while(input_File[k]!=' ') //Get the lexeme
			{
				lexeme=lexeme+input_File[k]; //Concatenation
				k++;
			}
	}
	else if(input_File[2]=='w' && input_File[3]=='h' && input_File[4]=='i' && input_File[5]=='l' && input_File[6]=='e')
	{
		token=lexeme="\0";
		token="while";
	}
	else if(input_File[2]=='n' && input_File[3]=='u' && input_File[4]=='m')
	{
		token=lexeme="\0";
		token="num";
			int k=8;
			while(input_File[k]!=' ') //Get the lexeme
			{
				lexeme=lexeme+input_File[k]; //Concatenation
				k++;
			}
	}
	else if(input_File[2]=='i' && input_File[3]=='n' && input_File[4]=='t')
	{
		ckr=true;
	}
	else if(input_File[2]=='r' && input_File[3]=='e' && input_File[4]=='t' && input_File[5]=='u' && input_File[6]=='r' && input_File[7]=='n')
	{
		token=lexeme="\0";
		token="return";
	}
	else{
		token="\0";
	}
	
}


string Translator:: subMachine1()
{
	string rett;
	nextToken();
	if(token=="id" || token=="num")
	{

		if(operatr=='\0')
		{
			if(token=="id"){
			string rt=tbl->returnType(lexeme);
			
			if(rt=="&")//conformed   
			{
				string joSend=lexeme;
				nextToken(); //its this --> [
				prev1=forArray(joSend); //Array Name sent to the function. Now var got the returned new tmp.
			string sx=subMachine1();
			}
			else{
			prev1=lexeme;
			string sx=subMachine1();
			}
			}
			else{
				prev1=lexeme;
			string sx=subMachine1();
			}
		}
		else
		{
			//first make quadruple entry
			string tmp1=tbl->getRA(prev1);
			string tmp2;
			string tmp3;
			//First check whether its an Array.
			string rt=tbl->returnType(lexeme);
			if(rt=="&")//conformed   
			{
				string joSend=lexeme;
				nextToken(); //its this --> [
				string var=forArray(joSend); //Array Name sent to the function. Now var got the returned new tmp.
				tmp2=tbl->getRA(next1);
				tmp3=tbl->getRA(var);
				//make quadruple entry
				quadruple[n][0]=operatr;
				quadruple[n][1]=tmp1;
				quadruple[n][2]=tmp3; 
				quadruple[n][3]=tmp2; 
				n++;

				prev1=next1;
			operatr='\0';
			string sx=subMachine1();
			}
			else{
			/////////////////////////////////
			bool isNum=isInteger(lexeme);
			if(isNum==false){
			 tmp2=tbl->getRA(lexeme);
			 tmp3=tbl->getRA(next1);
			}
			else {												//	+|y|NUM->|5||t0				i=i+1;
		//	 tmp2="NUM="+lexeme;   //tmp2=NUM+5    tmp3=12    quad: +|4|NUM->|5||12				t0=i+1; ->i=t0;
				tmp2="NUM->";					//prev1=i; next1=t0; lexeme=5;
				tmp3=lexeme+"||"+tbl->getRA(next1);
			}
			
				quadruple[n][0]=operatr;
				quadruple[n][1]=tmp1;
				quadruple[n][2]=tmp2; 
				quadruple[n][3]=tmp3; 
				//iteratorr2++;

				program[arrIterator]=next1+"="+prev1+operatr+lexeme+";"; //n1=t0, prev1=x ,lex=y
				arrIterator++;
				n++;
			
			
			prev1=next1;
			operatr='\0';
			string sx=subMachine1();
			
		}
	}
	}
	else if(token=="+")
	{
		operatr='+';
		std::string s= to_string(indexx);
		next1=tmp+s;   //new var created now place it into ST

		tbl->fill(next1);
		int r=tbl->lookup(next1);
		tbl->insert(r,2,"0");
		indexx++;
		string sx=subMachine1();
		
	}

	else if(token=="-")
	{
		operatr='-';
		std::string s= to_string(indexx);
		next1=tmp+s;	//new var created now place it into ST

		tbl->fill(next1);
		int r=tbl->lookup(next1);
		tbl->insert(r,2,"0");

		indexx++;
		string sx=subMachine1();
	}
	else if(token=="*")
	{
		operatr='*';
		std::string s= to_string(indexx);
		next1=tmp+s;	//new var created now place it into ST

		tbl->fill(next1);
		int r=tbl->lookup(next1);
		tbl->insert(r,2,"0");

		indexx++;
		string sx=subMachine1();
	}
	else if(token=="/")
	{
		operatr='/';
		std::string s= to_string(indexx);
		next1=tmp+s;	//new var created now place it into ST

		tbl->fill(next1);
		int r=tbl->lookup(next1);
		tbl->insert(r,2,"0");

		indexx++;
		string sx=subMachine1();	
	}
	else if(token==";")
	{
		operatr='\0';
		AS=false;
		rett=prev1;
	}
	//Now working for function CALL
	else if(token=="(")
	{
		funName=prev1; //functionName
		argumentCounter=0;
		argumentList();
		std::string s= to_string(indexx);
		next1=tmp+s;
		indexx++;

		program[arrIterator]=next1+"= call "+funName+" , "+to_string(argumentCounter);
				arrIterator++;
				n++;
		prev1=next1;
	}

	return prev1;
}

string Translator::forArray(string l)
{
	string reet;
		nextToken();
		if(token=="num")
		{
			std::string s= to_string(indexx);
		string 	boogie=tmp+s;   //new var created now place it into ST
			tbl->fill(boogie);
			int r=tbl->lookup(boogie);
			tbl->insert(r,2,lexeme);//set num ki lexeme as value.
			indexx++;
			//Above var creation was for subscript i.e for ptr[3] ==> t1=3
			//now another var should be created to hold whole ptr[t1]
			 s= to_string(indexx);
		string 	boogie2=tmp+s;   //new var created now place it into ST
			tbl->fill(boogie2);
			 r=tbl->lookup(boogie2);
			tbl->insert(r,2,"0");
			indexx++;
			//now put into quadruple
			string t1=tbl->getRA(l);
			string t2=tbl->getRA(boogie);
			string t3=tbl->getRA(boogie2);
				quadruple[n][0]="[";
				quadruple[n][1]=t1;
				quadruple[n][2]=t2; 
				quadruple[n][3]=t3; 
				n++;


			nextToken(); //for ]
			return boogie2;
			
		}
		else if(token=="id")
		{
			string keep=lexeme;
std::string
			s= to_string(indexx);
		string 	boogie2=tmp+s;   //new var created now place it into ST
			tbl->fill(boogie2);
			int r=tbl->lookup(boogie2);
			tbl->insert(r,2,"0");
			indexx++;
			//now put into quadruple
			string t1=tbl->getRA(l);
			string t2=tbl->getRA(keep);
			string t3=tbl->getRA(boogie2);
				quadruple[n][0]="[";
				quadruple[n][1]=t1;
				quadruple[n][2]=t2; 
				quadruple[n][3]=t3; 
				n++;
				nextToken(); //for ]
			return boogie2;
		}

}
void Translator::argumentList()
{
	while(!(token==";"))
	{
		nextToken();
		if(token=="id")
		{
			program[arrIterator]="parm"+lexeme;
				arrIterator++;
				n++;
			argumentCounter++;
		}
	}
}

bool Translator:: machine1() //Implies S from grammer
{
	if(lost==true)//if not IF k bad else ay & dere is a posibility of losing a token
	{
		lost=false;
		prev_Tok=token;   
	}
	else{
		nextToken();
	}
	if(token=="id")
	{
		string v=lexeme;
		nextToken();
		if(token=="=") //Assignment St. conformed
		{
			string res=subMachine1();
			bool isNum=isInteger(res);
			if(isNum==true){ //e.g z=10;
				int value = atoi(res.c_str());

				int row=tbl->lookup(v);
				tbl->insert(row,2,res);

				string tmp2=tbl->getRA(v);
				quadruple[n][0]="=";
				quadruple[n][1]="NUM";
				quadruple[n][2]=res;
				quadruple[n][3]=tmp2;
			}
			else{
				//Not a numb e.g its z=t1;
				string tmp1=tbl->getRA(res); //pehle got t1 ka RA.
				string tmp2=tbl->getRA(v);
				quadruple[n][0]="=";
				quadruple[n][1]=tmp1;
				quadruple[n][2]=tmp2;

				//coz structure of quadrupel is :: = | t1 | z
			}
			program[arrIterator]=v+"="+res+";";
			arrIterator++;
			n++;

			return true;
		}
		else if(token=="[" && ckr==false) //Array Is Conformed
		{
			string 	boogie;
			nextToken(); //
			if(token=="num")
			{
			std::string s= to_string(indexx);
			boogie=tmp+s;   //new var created now place it into ST
			tbl->fill(boogie);
			int r=tbl->lookup(boogie);
			tbl->insert(r,2,lexeme);//set num ki lexeme as value.
			indexx++;
			}
			else if(token=="id")
			{
				boogie=lexeme;
			}
			nextToken();
			nextToken();
			if(token=="=") //Assignment St. conformed
			{
				string res=subMachine1();
				bool isNum=isInteger(res);
				if(isNum==true){ //e.g z[3]=10;
					std::string s= to_string(indexx);
					string boo=tmp+s;   //new var created now place it into ST
					tbl->fill(boo);
					int r=tbl->lookup(boo);
					tbl->insert(r,2,res);//set num ki lexeme as value.
					indexx++;
						string tmp1=tbl->getRA(v);//Array ka addr
						string tmp2=tbl->getRA(boogie);//Subscript addr
						string tmp3=tbl->getRA(boo); //10 ka addr

					quadruple[n][0]="]"; //Format---> ] | z | 3 | 10
					quadruple[n][1]=tmp1;
					quadruple[n][2]=tmp2;
					quadruple[n][3]=tmp3;
					n++;
				}
				else{
					//Not a numb e.g its z[3]=t1;
					string tmp1=tbl->getRA(v);//Array ka addr
						string tmp2=tbl->getRA(boogie);//Subscript addr
						string tmp3=tbl->getRA(res);
						quadruple[n][0]="]"; //Format---> ] | z | 3 | 10
					quadruple[n][1]=tmp1;
					quadruple[n][2]=tmp2;
					quadruple[n][3]=tmp3;
					n++;
				}
				

				return true;

			}
		}
		else if(ckr==true)
		{
			ckr=false;
			return true;
		}
		else{
			prev_Tok=token;  //since it is not an assignment statement
			prev_Lex=lexeme;
			return false;
		}
	}
}
////////////////////////////////////////////
void Translator ::backpatch(long long t,long long n)
{
	std::string dd= to_string(n);
	
	char* char_type = (char*) dd.c_str();
/*
//	char c = (char)n;
//	itoa(n,c,10);
	//char *c=new char;
	//itoa(n,c,10);
	
	//const char *cstr = dd.c_str();
	//params = 0,2 matlab prog k oth index ki goto entry per 2 rkhwa do
	
//	 char *result = strdup(dd.c_str());
*/
	string str (program[t]);

	string::iterator it;
		int index = 0;
		for ( it = str.begin() ; it < str.end(); it++ ,index++)
		 {
        
			 if(*it=='#')
			 {
				// str[index]=*c;
				 str[index]=char_type[0];
				// int b = char_type[1]-48;
				 if(isdigit(char_type[1]))
				 {
					 index++;
					 str[index]=char_type[1];
					 index++;
					 str[index]=';';
						 break;
				 }
				 else
				 {
					 index++;
					 str[index]=str[index+1];
					 str[index+1]='\0';
					 break;
				 }
			 }
		 }
		program[t]=str;
	//	cout<<c<<endl;
}
void Translator::bool_Expression()
{
	string a,b,c;
	nextToken();
	if(token=="id" ||token=="num")
	{		a=lexeme;	}
	nextToken();

	if(token=="ro")
	{		b=b+lexeme;	}
	nextToken();
	if(token=="id" ||token=="num")
	{		c=lexeme;	}
	//now Writing to array
	

	//first make quadruple entry
			string tmp1=tbl->getRA(a); //a
			string tmp2=b;
			string tmp3=tbl->getRA(c);//b implies a>=b
				quadruple[n][0]=b;
				quadruple[n][1]=tmp1;
				quadruple[n][2]=tmp3;
				iteratorr2=n;
				// >=|a|b|goto__ 

	program[arrIterator]="if"+a+b+c+"goto"+"#"+"#"+";";
	n++;
	arrIterator++;

	//need to save quadruple entry as well
	quadruple[n][0]="goto";
	string bx="#";
	program[arrIterator]="goto"+bx+"#"+";";
	n++;
	arrIterator++;
	BE_true=n-2;
   // BE_false=n-1;
	//  --->pushing onto Stack
	if(ifStack==true){
	backStack_Push(n-1);
	}
	else if(whileStack==true){
		backStack2_Push(n-1);
	}
	ifStack=whileStack=false;
}

void Translator::backStack_Push(int x)
{
	backStack[stackVar]=x;
	stackVar++;
}
int Translator::backStack_Pop()
{
	stackVar--;
	return backStack[stackVar];
}

void Translator::backStack2_Push(int x)
{
	backStack2[stackVar2]=x;
	stackVar2++;
}
int Translator::backStack2_Pop()
{
	stackVar2--;
	return backStack2[stackVar2];
}


bool Translator::machine3()//for looping
{
	if(prev_Tok=="while")
	{
		//n=n+ins_Counter; //get the emits of assignment st. b4 loop start
		//arrIterator=arrIterator+ins_Counter;
		S_s[S_i]=n;
		S_i++;
		nextToken();
		if(token=="(") 
		{
			whileStack=true;
			bool_Expression(); //implies BE
			nextToken();
			//here token=)

			std::string dd= to_string(n); //Writing goto line No in quadruple
			quadruple[iteratorr2][3]=dd; 

			//calling Action A2 now
			backpatch(BE_true,n);
			//now if Body need to be executed
			nextToken();
			if(token=="{")
			{
				IS=true;
				//while((!(token=="}")) && !(prev_Tok=="}")) ---> Changed
				while(!(token=="}"))
				{
					if(machine1()){	}
					else if(machine2()){}
					else if(machine3()){}
				//	nextToken();
				}
				S_i--;
				std::string s = std::to_string(S_s[S_i]);
				program[arrIterator]="goto"+s+";";
				//Writing above line in Quadruple
					quadruple[n][0]="goto";
					quadruple[n][1]=s;

				arrIterator++;
				n++;
				//backpatch(BE_false,n);
			//	backpatch(backStack2_Pop(),n);
				int theta=backStack2_Pop();
					backpatch(theta,n);
					//Now making quadruple entry of Above line
					quadruple[theta][1]=to_string(n);
				
				token="\0"; //Changed as in Selection
					return true;
			}
		}
	}
}

bool Translator::machine4()
{
	if(prev_Tok=="return")
	{
		nextToken();
		if(token==";")
		{
			program[arrIterator]="return ;";
				arrIterator++;
				n++;
		}
		else if(token=="id")
		{
			program[arrIterator]="return "+lexeme+" ;";
				arrIterator++;
				n++;
		}
	return true;
	}
}

bool Translator::machine2()
{
	if(prev_Tok=="if")
	{
		nextToken();
		if(token=="(") 
		{
			ifStack=true;
			bool_Expression(); //implies BE
			nextToken();
			//here token=)
			std::string dd= to_string(n); //Writing goto line No in quadruple
			quadruple[iteratorr2][3]=dd; 
			//iteratorr++;

			//calling Action A2 now
			backpatch(BE_true,n);
			//now if Body need to be executed
			nextToken();
			if(token=="{")
			{
				SS=true;
				while(!(token=="}"))
				{
					if(machine1()){	}
					else if(machine2()){}
					else if(machine3()){}
				//	nextToken();
				}
				//ended if statement now run A3 action.
				string bx="#";
				quadruple[n][0]="goto";
				program[arrIterator]="goto"+bx+"#"+";";
				arrIterator++;
				n++;
				S_x=n-1;
				//now executing else
				nextToken();
				if(token=="else")
				{
					nextToken();
					//executing A4
					//backpatch(BE_false,n);
					int theta=backStack_Pop();
					backpatch(theta,n);
					//Now making quadruple entry of Above line
					quadruple[theta][1]=to_string(n);

					//running S2
					while(!(token=="}"))
					{
						if(machine1());
						else if(machine2());
						else if(machine3());
					//	nextToken();
					}
					//now running Action A5.
					backpatch(S_x,n);
					//Applying above action in quadruple
					quadruple[S_x][1]=to_string(n);
					token="\0"; //Changed So that in nested wo else ka } na consider karay
					return true;

				}
				else { //backpatch(S_x,n);
					   //backpatch(BE_false,n);

			//	backpatch(backStack_Pop(),n);
				int theta=backStack_Pop();
					backpatch(theta,n);
					//Now making quadruple entry of Above line
					quadruple[theta][1]=to_string(n);

				lost=true;
				prev_Tok=token;
					return false; 
					} //changed
			}
		}


	}
	
}


void Translator::check_Machines()
{
	inFile2.open("Output.txt");
	if(inFile2.fail())
	{
		cerr << "Error. No input File Found" <<endl<<endl;
	}
	else{
	//check here k file ka eof toh ni ho gya...
	while(inFile2.eof()==false)
	{
		AS=SS=false;
		if(machine1()) //for assign st
		{
			ins_Counter++;
		}
		else if(machine2()) //for if,else
		{

		}
		else if(machine3())
		{
			lost=false;
		}
		else if(machine4())
		{
			lost=false;
		}
	}
//	cout<<"Translator"<<endl;
	tbl->printTable();
//	Sleep(2500);

	//printing
	for(int i=0;program[i]!="\0";i++)
	{
		cout<<i<<")  "<<program[i]<<endl<<endl;
	}
	 cout<<endl<<endl;

	 ofstream ii;
	 ii.open("quadruple.txt");
//	 cout<<"Quadruple"<<endl;
	 for(int i=0;i<=n;i++){
		for(int j=0;j<4;j++){
			ii<<quadruple[i][j]<<"  ";
			cout<<quadruple[i][j]<<"  ";
		}
		cout<<endl<<endl;
	}
	 quadruple[n][0]="HALT";
	 ii<<quadruple[n][0];
}

}



class VirtualMachine{
public:
	int *ds;
	VirtualMachine();
	void populateSet();
	void run();
};
VirtualMachine::VirtualMachine()
{
	ds=new int[RA];
}
void VirtualMachine::populateSet()
{
	int j=0;
	for(int i=0;i<iteratorr;i++)
	{
			

			string rt=table[i][3];
			if(rt=="&")//For Arry pick the RA for it.
			{
				int numb;
				istringstream ( table[i][1] ) >> numb;
				ds[j]=numb;
				j=j+4;
			}
			else{
			int numb;
			istringstream ( table[i][2] ) >> numb;

			ds[j]=numb;
			j=j+4;
			}
	}

	for(int i=0;i<RA;i=i+4)
	{
		cout<<ds[i]<<" ";
	}
	cout<<endl;

}

void VirtualMachine::run()
{
	int pc=0;
	while(quadruple[pc][0]!="HALT")
	{
		
		if(quadruple[pc][0]=="+")
		{
			string lex="\0";
			string add="\0";
			if(quadruple[pc][2]=="NUM->")
			{
				string str (quadruple[pc][3]);

				string::iterator it;
				int index = 0;
				bool strt=false;
				for ( it = str.begin() ; it < str.end(); it++ ,index++)
				{

					if(!(*it=='|') && strt==false)
					{
						lex=lex+str[index];
					}
					else{
						strt=true;
						index=index+2;
						add=add+str[index];
						index=index+1;
						add=add+str[index];
						break;
					}

				}
				int target;
				istringstream ( add ) >> target;
				int firstop;
				istringstream ( quadruple[pc][1] ) >> firstop;
				long long secondop;
				istringstream ( lex ) >> secondop; //number

				ds[target]=ds[firstop]+secondop;
			}
			///////////////////////////////////////\]]]]]]]]'pppppppppppppppppp
		
			else{
			int target;
			istringstream ( quadruple[pc][3] ) >> target;
			int firstop;
			istringstream ( quadruple[pc][1] ) >> firstop;
			int secondop;
			istringstream ( quadruple[pc][2] ) >> secondop;

			ds[target]=ds[firstop]+ds[secondop];
		}
		}
		else if(quadruple[pc][0]=="-")
		{

			int target;
			istringstream ( quadruple[pc][3] ) >> target;
			int firstop;
			istringstream ( quadruple[pc][1] ) >> firstop;
			int secondop;
			istringstream ( quadruple[pc][2] ) >> secondop;

			ds[target]=ds[firstop]-ds[secondop];
		}
		
		else if(quadruple[pc][0]=="*")
		{

			int target;
			istringstream ( quadruple[pc][3] ) >> target;
			int firstop;
			istringstream ( quadruple[pc][1] ) >> firstop;
			int secondop;
			istringstream ( quadruple[pc][2] ) >> secondop;

			ds[target]=ds[firstop]*ds[secondop];
		}

		else if(quadruple[pc][0]=="/")
		{

			int target;
			istringstream ( quadruple[pc][3] ) >> target;
			int firstop;
			istringstream ( quadruple[pc][1] ) >> firstop;
			int secondop;
			istringstream ( quadruple[pc][2] ) >> secondop;

			ds[target]=ds[firstop]/ds[secondop];
		}

		else if(quadruple[pc][0]=="=")//problem.
		{
			if(quadruple[pc][1]=="NUM")
			{
				int target;
				istringstream ( quadruple[pc][3] ) >> target;
				int num;
				istringstream ( quadruple[pc][2] ) >> num;

				ds[target]=num;
			}
			else{
			int target;
			istringstream ( quadruple[pc][2] ) >> target;
			int src;
			istringstream ( quadruple[pc][1] ) >> src;

			ds[target]=ds[src];
			}
		}
		//Assignment Ended.
		else if(quadruple[pc][0]=="GT")
		{
			int left;
			istringstream ( quadruple[pc][1] ) >> left;
			int right;
			istringstream ( quadruple[pc][2] ) >> right;

			if(ds[left] > ds[right])
			{
				int go;
				istringstream ( quadruple[pc][3] ) >> go;
				pc=go;
				continue;
			}
		}

		else if(quadruple[pc][0]=="LT")
		{
			int left;
			istringstream ( quadruple[pc][1] ) >> left;
			int right;
			istringstream ( quadruple[pc][2] ) >> right;

			if(ds[left] < ds[right])
			{
				int go;
				istringstream ( quadruple[pc][3] ) >> go;
				pc=go;
				continue;
			}
		}

		else if(quadruple[pc][0]=="GE")
		{
			int left;
			istringstream ( quadruple[pc][1] ) >> left;
			int right;
			istringstream ( quadruple[pc][2] ) >> right;

			if(ds[left] >= ds[right])
			{
				int go;
				istringstream ( quadruple[pc][3] ) >> go;
				pc=go;
				continue;
			}
		}

		else if(quadruple[pc][0]=="LE")
		{
			int left;
			istringstream ( quadruple[pc][1] ) >> left;
			int right;
			istringstream ( quadruple[pc][2] ) >> right;

			if(ds[left] <= ds[right])
			{
				int go;
				istringstream ( quadruple[pc][3] ) >> go;
				pc=go;
				continue;
			}
		}

		else if(quadruple[pc][0]=="EQ")
		{
			int left;
			istringstream ( quadruple[pc][1] ) >> left;
			int right;
			istringstream ( quadruple[pc][2] ) >> right;

			if(ds[left] == ds[right])
			{
				int go;
				istringstream ( quadruple[pc][3] ) >> go;
				pc=go;
				continue;
			}
		}

		else if(quadruple[pc][0]=="goto")
		{
			int lable;
			istringstream ( quadruple[pc][1] ) >> lable;
			pc=lable;
			continue;
		}
		else if(quadruple[pc][0]=="[")
		{
			int arr,ind,tar;
			istringstream ( quadruple[pc][1] ) >> arr;
			istringstream ( quadruple[pc][2] ) >> ind;
			istringstream ( quadruple[pc][3] ) >> tar;
			//ds[tar]=ds[(arr+ds[ind])*4];//changes *4
			//int xz=ds[ds[ind]];
		//	int yz=ds[xz];
		//	yz*=3;
			ds[tar]=ds[arr+ds[ind]];
			//ds[tar]=ds[arr]+(ds[ds[ind]*4)];
		//	cout<<ds[tar]<<endl;
			//cout<<ds[ds[tar]]<<endl;

		}
		else if(quadruple[pc][0]=="]") //case ptr[3]=10;
		{
			int arr,ind,tar;
			istringstream ( quadruple[pc][1] ) >> arr;//ptr
			istringstream ( quadruple[pc][2] ) >> ind;//3
			istringstream ( quadruple[pc][3] ) >> tar;//10
		//	cout<<ds[arr+ds[ind]]<<"  "<<ds[tar];
			ds[arr+ds[ind]]=ds[tar];

		}

		++pc;
	}
	cout<<"After--------"<<endl;
	for(int i=0;i<RA;i=i+4)
	{
		cout<<ds[i]<<" ";
	}
	cout<<endl;
}
void main()
{
	lex_Analyser * lexer=new lex_Analyser();
//	cout<<"LEXICAL ANALYSER RUNNING"<<endl<<endl;
	lexer->file_Reader();
	//Sleep(2500);
	//system("CLS");
	cout<<endl<<endl;
		Translator *obj=new Translator();
		obj->check_Machines();		
	cout<<endl<<endl<<endl;

	cout<<"DS Before Running Virtual Machine"<<endl;
	VirtualMachine *abc=new VirtualMachine();
	abc->populateSet();

	abc->run();
	system("pause");
}
	