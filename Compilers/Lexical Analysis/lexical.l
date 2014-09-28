%{

#include <iostream>
#include <string.h>
using namespace std;


#define TK_ILLEGAL_ID 253       // illegal id value
#define TK_ILLEGAL_CHAR 254

#define TK_ID		1	//common identifier
#define TK_CHAR_TYPE	2	//char 	identifier
#define TK_INT_TYPE	3	//int identifier
#define TK_FLOAT_TYPE	4	//float identifier
#define TK_ELSE		5	//else
#define TK_IF		6	//if
#define TK_MAIN		7	//main
#define TK_RETURN	8	//return
#define TK_VOID		9	//void
#define TK_WHILE	10	//while
#define TK_RECORD	11	//record

#define TK_INT		12	//integer
#define TK_FLOAT	13	//float
#define TK_CHAR		14	//char

#define TK_LPAR		15	// left parenthesis " ( "
#define TK_RPAR		16	// right parenthesis " ) "
#define TK_LBRACE	17	// left brace  " { "
#define TK_RBRACE	18	// right brace " } "
#define TK_LBRACKET	19	// left bracket  " [ "
#define TK_RBRACKET	20	// right bracket " ] " 

#define TK_PLUS		21	//+33
#define TK_MINUS	22	//-33
#define TK_ADD		23	//33+33 ta idia ousiastika
#define TK_SUB		24	//33-33
#define TK_MULT		25	// "*"
#define TK_DIV		26	// "/"
#define TK_MOD		27	// "%"


#define TK_EQ 		28 // == 
#define TK_DIFF 	29 // != 
#define TK_GR 		30 // > 
#define TK_LESS 	31 // < 
#define TK_GOE 		32 // >=
#define TK_LOE		33 // <=

#define TK_AND		34 // &&
#define TK_OR		35 // ||
#define TK_NOT		36 // !

#define TK_AMP		37 // "&"
#define TK_SEMI		38 // ";"
#define TK_SDOT		39 // "." ( record.x)

#define TK_MAKE_EQ	40 // "="
#define TK_COMMA	41 // ","

#define TK_STRING	42 // "string"

#define MAX_INCLUDE_DEPTH 1	// mono se ena arxeio mporei na ginetai #include , 
#define MAX_STR_CONST 200				// apo ekeino to arxeio den mporoume na kanoume allo #include
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;
char string_buf[MAX_STR_CONST];


string cBuf,sBuf,yyStr;
%} 

letter	[a-zA-Z] 
digit	[0-9] 
underscore _ 
id	{letter}({letter}|{digit}|{underscore})* 
integer {digit}+
float {digit}+\.{digit}+([eE][+-]?{digit}+)?

simple_char [^\\\'\"]
special_char \\[n\"\'0t\\]
non_printable_char [^{simple_char}]
char \'({simple_char}|{special_char})\'
invalid_special_char \'\\[^n\"\'0t\\]\'
empty_char	\'\'
non_printable	\'[^{simple_char}]\'
multibyte_char \'(.)+\'
unterminated_char \'([^(\')])*

illid ({integer}|{float})+(\.)*{id}+

%x IN_COMMENTS 
%x INCLUDE
%x IN_STRING
%%


"int" 		{ return TK_INT_TYPE; }
"float"		{ return TK_FLOAT_TYPE; }
"char"		{ return TK_CHAR_TYPE; }
"else"		{ return TK_ELSE;}
"if"		{ return TK_IF; }
"main" 		{ return TK_MAIN; }
"return"	{ return TK_RETURN; }
"void" 		{ return TK_VOID; }
"while"		{ return TK_WHILE; }
"record"	{ return TK_RECORD; }

"("		{ return TK_LPAR; }
")"		{ return TK_RPAR; }
"["		{ return TK_LBRACKET; }
"]"		{ return TK_RBRACKET; }
"{"		{ return TK_LBRACE; }
"}"		{ return TK_RBRACE; }

"+"		{ return TK_PLUS; }
"-"		{ return TK_MINUS; }
"*"		{ return TK_MULT; }
"/"		{ return TK_DIV; }
"%"		{ return TK_MOD; }

"=="		{ return TK_EQ; }
"!="		{ return TK_DIFF; }
">"		{ return TK_GR; }
"<"		{ return TK_LESS; }
">="		{ return TK_GOE; }
"<="		{ return TK_LOE; }

"\|\|"		{ return TK_OR; }
"&&"		{ return TK_AND; }
"!"		{ return TK_NOT; }

"&"		{ return TK_AMP; }
";"		{ return TK_SEMI; }
"\."		{ return TK_SDOT; }
"="		{ return TK_MAKE_EQ;}
","		{ return TK_COMMA; }
"//"([^\n])*	{ /* trwei ta // comments*/ }

{integer}	{ return TK_INT; }
{float} 	{ return TK_FLOAT; }
{id}		{ return TK_ID; }
{illid}		{
		cout<<"Illegal Id found"<<endl; 
		return TK_ILLEGAL_ID; 
}


{char}	{
	yyStr = yytext;	
	cBuf = yyStr.substr(1,yyleng-2); // bgazoume ta ' '
	strcpy(yytext,cBuf.c_str()); 
	return TK_CHAR;
}

{invalid_special_char}	{
	cout<<"Error:This special char is not valid:"<<yytext<<endl;
        return TK_ILLEGAL_CHAR;

}

{non_printable}	{
	cout<<"Error:This  character is not valid(non-printable):"<<yytext<<endl;
        return TK_ILLEGAL_CHAR;
}

{multibyte_char}	{
	cout<<"Error:Multibyte char constand found:"<<yytext<<endl;
	return TK_ILLEGAL_CHAR;
}

{empty_char}	{
	cout<<"Error:Empty char constand found:"<<yytext<<endl;
        return TK_ILLEGAL_CHAR;
}

{unterminated_char}     {
        // tha mpei edw mono ean exoyme ' mpla mpla mexri to eof
        cout<<"Unterminated or not valid char constant found, exiting"<<endl;
        yyterminate();
}



<IN_STRING>
{
	
	([^\"])*\" { //telos tou string
		yyStr = yytext;
		sBuf = yyStr.substr(0,yyleng-1);
		strcpy(yytext,sBuf.c_str());
		BEGIN(INITIAL);
		return TK_STRING;
	}
}

<IN_STRING>\n {
	cout<<"Multiline string error, exiting"<<endl;
	BEGIN(INITIAL);
	yyterminate();
}

<IN_STRING><<EOF>> {
	cout<<"Unterminated string, exiting"<<endl;
	BEGIN(INITIAL);
        yyterminate();
} 

[ \t\n] { /*ignores spaces,tabs and newlines */ }

<INITIAL>
{
	"/*"		BEGIN(IN_COMMENTS);
	"\""		BEGIN(IN_STRING);
}

<IN_COMMENTS>{
	"*"+"/"      BEGIN(INITIAL); // an brethei * akolouthoumeno apo / 
	[^*]+	     // otidipote brethei ektos apo * to trwei
	"*"+[^*/]*   // an brethei * to opoio den akoloutheitai apo / i (*/) to trwei
}

<IN_COMMENTS><<EOF>> {
	cout<<"Unterminated comments, exiting"<<endl;
	yyterminate();
}



"#include" {BEGIN(INCLUDE);}

<INCLUDE>[ \t]* /* eat the whitespaces & tabs*/

<INCLUDE>
{
	\"(.)*\" {
                yyStr = yytext;
                sBuf = yyStr.substr(1,yyleng-2); // bgazoume ta " "
                strcpy(yytext,sBuf.c_str());
                if ( include_stack_ptr >= MAX_INCLUDE_DEPTH) {
                        cout<<"Includes nested too deeply"<<endl;
                        exit( 1 );
                }
                include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;
                yyin = fopen( yytext, "r" );
                if ( !yyin )
                        cout<<"Error opening file:"<<yytext<<endl;
                YY_BUFFER_STATE new_buffer;
		new_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );
		yy_switch_to_buffer(new_buffer);
                BEGIN(INITIAL);
	}
}
<INCLUDE>
{
	[^\"$] {
	        cout<<"Error in included file name ,propably unterminated file name given"<<endl;
		yyterminate();
	}
}

<<EOF>> {
        if ( --include_stack_ptr < 0 ) {
                yyterminate();  // ean eftase sto telos tou teleutaiou arxeiou
        }
        else {
                yy_delete_buffer( YY_CURRENT_BUFFER );
                yy_switch_to_buffer(include_stack[include_stack_ptr] );
        }
        BEGIN(INITIAL);
}

%%

int main(int argc, char* argv[] ) {
	int token;
	yyin = fopen( argv[1], "r" );

	while ((token = yylex()) != 0) {		
		cout<<"Token "<<token<<" :"<<yytext<<endl;
	}
	return 0;
}
