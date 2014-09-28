%{

#include <iostream>
using namespace std;
extern int yylex();
extern char *yytext;
extern FILE *yyin;
void yyerror(char *s);
%}

%start program

%token TK_ILLEGAL_ID
%token TK_ILLEGAL_CHAR

%token TK_ID      
%token TK_CHAR_TYPE  
%token TK_INT_TYPE	//int identifier
%token TK_FLOAT_TYPE	//float identifier
%token TK_ELSE		//else
%token TK_IF		//if
%token TK_MAIN          //main
%token TK_RETURN        //return
%token TK_VOID          //void
%token TK_WHILE         //while
%token TK_RECORD        //record
%token TK_INT           //integer
%token TK_FLOAT         //float
%token TK_CHAR          //char

%token TK_LPAR          // left parenthesis " ( "
%token TK_RPAR          // right parenthesis " ) "
%token TK_LBRACE        // left brace  " { "
%token TK_RBRACE        // right brace " } "
%token TK_LBRACKET      // left bracket  " [ "
%token TK_RBRACKET      // right bracket " ] "

%token TK_PLUS          //+33
%token TK_MINUS         //-33
%token TK_ADD           //33+33 to TK_ADD epistrefei o flex otan brei +
%token TK_SUB           //33-33
%token TK_MULT          // "*"
%token TK_DIV           // "/"
%token TK_MOD           // "%"


%token TK_EQ            // ==
%token TK_DIFF          // !=
%token TK_GR            // >
%token TK_LESS          // <
%token TK_GOE           // >=
%token TK_LOE           // <=

%token TK_AND           // &&
%token TK_OR            // ||
%token TK_NOT           // !

%token TK_AMP           // "&"
%token TK_SEMI          // ";"
%token TK_SDOT          // "." ( record.x)

%token TK_MAKE_EQ       // "="
%token TK_COMMA         // ","

%token TK_STRING        // "string"

%left TK_COMMA
%right TK_MAKE_EQ
%left TK_OR
%left TK_AND
%left TK_EQ TK_DIFF
%left TK_GR TK_LESS TK_GOE TK_LOE
%left TK_ADD TK_SUB 
%left TK_MULT TK_DIV TK_MOD
%right TK_NOT TK_PLUS TK_MINUS TK_AMP
%left TK_LPAR TK_RPAR TK_LBRACKET TK_RBRACKET TK_SDOT 
%%

/* to 0 i perissotera lunetai me ton 5o kanona pou ksekinaei kateuthian me program header */

program : function_prototype_plus program_header compound_statement function_definition_star
        | record_definition_plus program_header compound_statement function_definition_star
        | function_definition_plus program_header compound_statement function_definition_star
        | variable_definition_plus program_header compound_statement function_definition_star
        | program_header compound_statement function_definition_star
        ;


function_definition_star : function_definition function_definition_star
			     | /*empty*/
	     		     ;

function_prototype_plus : function_prototype
                        | function_prototype_plus function_prototype
                        ;

record_definition_plus : record_definition
                       | record_definition_plus record_definition
                       ;

function_definition_plus : function_definition
                         | function_definition_plus function_definition
                         ;

variable_definition_plus : variable_definition
                         | variable_definition_plus variable_definition
                         ;

program_header : TK_VOID TK_MAIN TK_LPAR TK_RPAR
               ;

function_prototype : function_header TK_SEMI;

record_definition : composite_type TK_LBRACE primitive_type_def_plus TK_RBRACE TK_SEMI;

function_definition : function_header compound_statement;

variable_definition : primitive_type_def
                    | composite_type_def
                    ;

function_header : data_type TK_ID TK_LPAR TK_RPAR
                | TK_VOID TK_ID TK_LPAR TK_RPAR
                | composite_type TK_ID TK_LPAR TK_RPAR
                | data_type TK_ID TK_LPAR formal_params TK_RPAR
                | TK_VOID TK_ID TK_LPAR formal_params TK_RPAR
                | composite_type TK_ID TK_LPAR formal_params TK_RPAR
                ;

composite_type :TK_RECORD TK_ID;

primitive_type_def_plus : primitive_type_def
                        | primitive_type_def_plus primitive_type_def
                        ;

primitive_type_def : data_type def_onevar_plus TK_SEMI
                   ;

composite_type_def : composite_type def_onevar_plus TK_SEMI
                   ;

def_onevar_plus : def_one_variable
                | def_one_variable TK_COMMA def_onevar_plus
                ;

def_one_variable : TK_ID
                 | TK_ID TK_LBRACKET TK_INT TK_RBRACKET
                 ;


formal_params : formal_parameter
              | formal_parameter TK_COMMA formal_params
              ;

formal_parameter : data_type TK_ID
                 | data_type TK_AMP TK_ID
                 | data_type TK_AMP TK_ID TK_LBRACKET TK_RBRACKET
                 | data_type TK_ID TK_LBRACKET TK_RBRACKET
                 | composite_type TK_ID
                 | composite_type TK_AMP TK_ID
                 | composite_type TK_AMP TK_ID TK_LBRACKET TK_RBRACKET
                 | composite_type TK_ID TK_LBRACKET TK_RBRACKET
                 ;

variable_definition_star : /*empty*/
                         | variable_definition_star variable_definition
                         ;

statement_star : /*empty*/
	       | statement_star statement

statement : open_statement
	  | closed_statement
          ;

open_statement : TK_IF TK_LPAR b_expression TK_RPAR statement 
		| TK_IF TK_LPAR b_expression TK_RPAR closed_statement TK_ELSE open_statement
		| TK_WHILE TK_LPAR b_expression TK_RPAR open_statement
		;

closed_statement : simple_statement
		| TK_IF TK_LPAR b_expression TK_RPAR closed_statement TK_ELSE closed_statement 
		| TK_WHILE TK_LPAR b_expression TK_RPAR closed_statement
		;


simple_statement : assignment
          | return_statement
          | compound_statement
          | TK_SEMI /*empty statement*/
          | void_function_call
	;

void_function_call : function_call TK_SEMI
                   ;

function_call : TK_ID TK_LPAR TK_RPAR
              | TK_ID TK_LPAR actual_params TK_RPAR
              ;

actual_params : actual_parameter
	      | actual_parameter TK_COMMA actual_params
              ;

actual_parameter : expression
                 | TK_STRING
                 ;
/*
if_statement : TK_IF TK_LPAR b_expression after_exp TK_RPAR statement after_then
             | TK_IF TK_LPAR b_expression after_exp TK_RPAR statement after_then TK_ELSE statement
             ;
while_statement : TK_WHILE TK_LPAR b_expression TK_RPAR statement
               ;
*/

return_statement : TK_RETURN TK_SEMI
                 | TK_RETURN expression TK_SEMI
                 ;

assignment :  comp_l_value TK_MAKE_EQ expression TK_SEMI
           ;

comp_l_value : TK_ID TK_SDOT l_value
             | l_value
             ;

l_value : TK_ID
        | TK_ID TK_LBRACKET  expression TK_RBRACKET
        ;

compound_statement : TK_LBRACE variable_definition_star statement_star TK_RBRACE;

data_type : TK_INT_TYPE
          | TK_FLOAT_TYPE
          | TK_CHAR_TYPE

b_expression : b_expression TK_OR b_expression
             | b_expression TK_AND b_expression
             | TK_NOT b_expression
             | expression TK_EQ expression
             | expression TK_DIFF expression
             | expression TK_GR expression
             | expression TK_LESS expression
             | expression TK_GOE expression
             | expression TK_LOE expression
             ;


expression : expression TK_ADD expression
           | expression TK_SUB expression
           | expression TK_MULT expression
           | expression TK_DIV expression
           | expression TK_MOD expression
           | TK_ADD expression %prec TK_PLUS
           | TK_SUB expression %prec TK_MINUS
           | TK_INT
           | TK_CHAR
           | comp_l_value
           | function_call
           ;

%%


void yyerror (char *s)
{
printf ("Error:%s-%s\n", s,yytext);
}

int main(int argc, char *argv[]) {
	if (argc != 1)
	{
		if (argv[1] != NULL)
			yyin = fopen( argv[1], "r" );
	}

	int ret = yyparse();
	if (ret == 0)
		cout<<"Parsing was successful!"<<endl;
	else if (ret == 1)
                cout<<"Parsing has failed,syntax error!"<<endl;
	else if (ret == 2)
                cout<<"Parsing has failed,memory exhaustion!"<<endl;

	return 0;
}
