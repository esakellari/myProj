%{

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Object.h"
#include "Node.h"

#define CHAR 0
#define INT 1
#define FLOAT 2
#define STRING 3
#define BOOL 4
#define VOID 5
#define RECORD 6
#define VARIABLE 7
#define ERROR_TYPE -1

#define USE 0
#define DECLARATION 1

using namespace std;


extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;

void yyerror(string s);
string Error;

int num_errors = 0;	/* syntax errors */
int sem_errors = 0;	/* semantic errors */


treeNode *root 	   = new treeNode("GLOBAL",true);
treeNode *curNode  = root;

string OperationError(string s, int type1, int type2,char *val1,char *val2);
string getType(int type);


vector<int>return_types;

bool same;
vector<string>function_protos,function_defs;
vector<string>param_names;
%}

%start program
%error-verbose
%expect 4 /*if -else kai gia ta error-recovery tous */


%union {
	char charValue;
	int intValue;
	float floatValue;
	char *stringValue;

	treeNode *curN;

	vector <Variable *> *tempvars;
	vector <vector <Variable *> *> *primvars;
	vector<Parameter *> *temppars;

	Variable *tempvar;
	Parameter *temppar;

	vector <Record *> *temprecs;
	Record *temprec;


	struct type_and_val {
		char *Val;
		int type;
		bool isArray;
		bool isElement;
	}typeVal;

	vector<struct type_and_val *> *actualparams;

	
	struct lval{
		char *name;		
		char *arrayIndex;
		int indexType;
		bool isArray;
		bool isElement;
	}lval;
	
	struct fh{
		int type;
		char *Val; /*Val = NULL, if type = record Val = recname*/
		char* name;
		vector <Parameter *> *tempvars;
	}fun_head;
}


%type <typeVal> expression return_statement
%type <intValue> b_expression
%type <typeVal> function_call

%type <typeVal> comp_l_value  actual_parameter
%type <actualparams> actual_params
%type <lval> l_value

%type <fun_head> function_header
%type <temppars> formal_params
%type <temppar> formal_parameter


%type <intValue> data_type
%type <typeVal> composite_type 
%type <fun_head> function_definition function_front

%type <tempvar> def_one_variable
%type <tempvars> def_onevar_plus

%type <primvars> primitive_type_def_plus
%type <tempvars> primitive_type_def

%type <curN> close_scope


%token TK_ILLEGAL_ID
%token TK_ILLEGAL_CHAR

%token <stringValue>	TK_ID      
%token <intValue>       TK_INT          //integer
%token <floatValue>     TK_FLOAT        //float
%token <charValue>      TK_CHAR         //char
%token <stringValue>    TK_STRING       //string


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


%token TK_BOGUS

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

program : global_declaration_star program_header compound_statement function_definition_star {
		bool found = true;
		for (int i = 0 ; i < function_protos.size(); i++) {
			found = false;
			for (int j = 0; j < function_defs.size(); j++) {
				if (function_protos[i] == function_defs[j]) {
					found = true;
					break;
				}
			}
			if ( !found ) {
				yyerror("Function "+function_protos[i]+" is declared without being defined");
				yyerrok;
				sem_errors++;
			}
		}
	}
	| global_declaration_star  {
		Error = "Main has not been declared";
		yyerror(Error);
		yyerrok;
		num_errors++;
	}
	;



    
function_definition_star : 
			 | function_definition function_definition_star;
			
global_declaration : function_prototype
	 	   | record_definition
 		   | function_definition
	           | variable_definition
		   ;
global_declaration_star : 
			| global_declaration_star global_declaration
			;

program_header  : TK_VOID TK_MAIN TK_LPAR TK_RPAR
		| TK_VOID TK_MAIN error TK_RPAR  	{yyerror("Error in program header missing '('");num_errors++; yyerrok;}
               	| TK_VOID TK_MAIN TK_LPAR error TK_RPAR {yyerror("Error program header does not takes parameters");num_errors++; yyerrok;}
		;

function_prototype : 
	function_header TK_SEMI{
				
		if ($1.type != ERROR_TYPE) {
			Function *f;
			if((f = root->lookupFunction( $1.name) )== NULL){
				Function *temp;
				if ($1.type == RECORD)
					temp  = new Function($1.type,$1.Val,$1.name,$1.tempvars);
				else
					temp =  new Function($1.type, " ",$1.name,$1.tempvars);

				root->insertFunction($1.name,temp);
				function_protos.push_back($1.name);
			}else{
				string name = $1.name;
				Error = "Function "+name+" has already been defined as "+f->TakePrototypeString();
				yyerror(Error);
				yyerrok;
				sem_errors++;
			}
		}
	}
	| function_header error TK_SEMI  {yyerror("Error in function prototype, function not declared"); num_errors++; yyerrok;}
	;



open_scope : TK_LBRACE  {
		curNode = curNode->insertNode("Empty");
	}
	;

close_scope : TK_RBRACE {
		$$ = curNode; /* to krataei gia periptwsi ( error ) parakatw opou tha leipei to open_scope gia na gurisoume ston swsto */
		curNode = curNode->getFather();
	}
	;

record_definition : 
	composite_type open_scope primitive_type_def_plus close_scope TK_SEMI{

		string recName = $1.Val;
		string name = "NON";
		bool add_memb = false;
		if(root->lookupRecord(DECLARATION, recName) == NULL)
		{
			Record *temp = new Record(name,recName, NULL, false,false , 0);
				
			for(int i =0; i < $3->size(); i++)
			{	
				for(int j = 0; j < (*$3)[i]->size(); j++)
				{
					add_memb = true;
					temp->AddMember((*(*$3)[i])[j]);
				}
			}
			
			if(!add_memb) {
				yyerror("Cannot declare an empty record , due to variable's declaration errors above");
				yyerrok;
				sem_errors++;
				delete temp;
			}
			else	root->insertRecord($1.Val,temp);
		}
		else
		{
			Error = "Record "+recName+" has already been defined";
			yyerror(Error);
	        	sem_errors++;
	        	yyerrok;

			for(int i =0; i < $3->size(); i++)
			{ 
				for(int j = 0; j < (*$3)[i]->size(); j++)
				{
					delete ((*(*$3)[i])[j]);
				}
				delete (*$3)[i];
			}				
		}
		delete $3;
		delete $1.Val;
	}
	| composite_type open_scope error close_scope TK_SEMI {
		yyerror("Error in record's definition variables");
		num_errors++;
		yyerrok;
		delete $1.Val;
	}
	| composite_type open_scope close_scope TK_SEMI {
		yyerror("Cannot declare an empty record");
		num_errors++;
		yyerrok;
		delete $1.Val;
	}
	| composite_type error close_scope TK_SEMI {
		/* has to come back to node */
		curNode = $3;
		yyerror("Error in record definition : missing '{'"); 
		num_errors++; 
		yyerrok;
		delete $1.Val;
	}
	| composite_type open_scope error TK_SEMI {
		curNode = curNode->getFather(); /* close scope */				
		yyerror("Error in record definition : missing '}'"); 
		num_errors++; 
		yyerrok;
		delete $1.Val;
	}
	;

function_definition : function_front function_back{
		string name = $1.name;
		bool alrd = false;
		bool ins = true;
		for (int i = 0; i < function_defs.size(); i++) {
			if ( function_defs[i] == name) {
				yyerror("Function "+name+" has already been defined");
				yyerrok;
				alrd = true;
				sem_errors++;
			}
		}

		if (!alrd) {
			if (return_types.size() == 0 && $1.type != ERROR_TYPE) {
				yyerror("Warning: return statement does not return a value, and header of function "+name+" not void");
				yyerrok;
			}	
			else {
				
				for (int i = 0; i < return_types.size(); i++) {
					if (return_types[i]  != ERROR_TYPE) {
						if (return_types[i] != $1.type) {
							yyerror("Error: conflict between return statement type and function "+name+" type");
							yyerrok;
							sem_errors++;
							ins = false;
						}						
					}
				}	
				return_types.clear();
			}
			if (ins) function_defs.push_back(name);
		}
	}
	;

function_front:
	function_header open_scope {	
		$$.type = $1.type;	
		Function *f;
		string name = $1.name;
		if((f = root->lookupFunction(name)) != NULL){
					
			//psaxnoum an i synartisi einai idiou typou me to prototype
			if( f->getType() != $1.type){
				string tp = getType($1.type);
				Error = "Conflicting return types of function "+name+" , previous declaration as "+f->TakePrototypeString();
				yyerror(Error);
	        		sem_errors++;
	    			yyerrok;
				$$.type = ERROR_TYPE;
			}
			else if (f->getType() == RECORD) {
				string vl = $1.Val;
				if (f->getRecName() != vl) {
			Error = "Conflicting return types of function "+name+" as record "+vl+" , previous declaration as "+f->TakePrototypeString();
					yyerror(Error);
					sem_errors++;
					yyerrok;
					$$.type = ERROR_TYPE;
				}
			}

			if( f->getNumParams()!= $1.tempvars->size()){
				Error = "Wrong number of parameters for function "+name+" , previous declaration as "+f->TakePrototypeString();
				yyerror(Error);
	        		sem_errors++;
	        		yyerrok;
			}
			else{
				vector<Parameter *> *fpar;
				fpar = f->getParams();

				for(int i =0; i < $1.tempvars->size(); i++){
					char cpi[5];	
					memset(cpi,'\0',5);
					sprintf(cpi,"%d",$1.tempvars->size()-i); 
					string pi = cpi;	
					if((*fpar)[i]->getType() != (*($1.tempvars))[i]->getType())
					{
						Error = "Conflicting type for  parameter no:"+pi+" of function "+name;
						Error += " previous declared as "+f->TakeParam(i);
						yyerror(Error);
						sem_errors++;
						yyerrok;
					}

					if((*fpar)[i]->getKind() == RECORD && (*($1.tempvars))[i]->getKind() == RECORD){
						if( (*fpar)[i]->getRecordName() != (*($1.tempvars))[i]->getRecordName()){
							Error = "Conflicting Record name for parameter no:"+pi+" of function ";
							Error += name+" as "+(*($1.tempvars))[i]->getRecordName()+" ,previously declared as ";
							Error += (*fpar)[i]->getRecordName();
							yyerror(Error);
	        					sem_errors++;
	        					yyerrok;
						}
						if((*fpar)[i]->isByref() != (*($1.tempvars))[i]->isByref()){
							Error = "Conflicting types (ref) for parameter no:"+pi+" was declared as "+f->TakeParam(i);
							yyerror(Error);
	     						sem_errors++;
	        					yyerrok;
						}
						if((*fpar)[i]->isArray() != (*($1.tempvars))[i]->isArray()) {
							Error = "Conflicting types (arr) for parameter no:"+pi+" was declared as "+f->TakeParam(i);
							yyerror(Error);
							sem_errors++;
							yyerrok;
						}
						
						Record *rec = (Record *) (*($1.tempvars))[i];
						curNode->insertRecord((*($1.tempvars))[i]->getName(),rec);
					}
					else if((*fpar)[i]->getKind() == VARIABLE  && (*($1.tempvars))[i]->getKind() == VARIABLE ){

						if((*fpar)[i]->isArray() != (*($1.tempvars))[i]->isArray()) {
							Error = "Conflicting types (array) for parameter no:"+pi+" was declared as "+f->TakeParam(i);
							yyerror(Error);
	        					sem_errors++;
	        					yyerrok;
						}
						
						if( (*fpar)[i]->isByref() != (*($1.tempvars))[i]->isByref()) {
							Error = "Conflicting types (ref) for parameter no:"+pi+" was declared as "+f->TakeParam(i);
							yyerror(Error);
	        					sem_errors++;
	        					yyerrok;
						}
						Variable *var =(Variable *) (*($1.tempvars))[i];
						curNode->insertVariable((*($1.tempvars))[i]->getName(), var);
					}
				}
			}
		}
		else{ // ean den brikame prototype autis tis sunartisis
			if($1.type ==RECORD)
				f = new Function($1.type,$1.Val,$1.name,$1.tempvars);
			else
				f = new Function($1.type," ",$1.name,$1.tempvars);
			root->insertFunction($1.name,f);
			// inserting local vars 
			for(int i =0; i < $1.tempvars->size(); i++){
				if( (*($1.tempvars))[i]->getKind() == VARIABLE) {
					Variable *var =(Variable *) (*($1.tempvars))[i];
					curNode->insertVariable((*($1.tempvars))[i]->getName(), var);
				}
				else if ((*($1.tempvars))[i]->getKind() == RECORD) {
					Record *rec = (Record *) (*($1.tempvars))[i];
					curNode->insertRecord((*($1.tempvars))[i]->getName(),rec);
				}
			}			
		}
		$$.name = $1.name;
	}
	;

function_back :
	variable_definition_star statement_star close_scope
	;


variable_definition : primitive_type_def
                    | composite_type_def
                    ;

function_header : 
		data_type TK_ID TK_LPAR TK_RPAR{

			$$.Val = NULL;
			$$.type = $1;
			$$.name = new char[strlen($2)+1];
			strcpy($$.name, $2);
			$$.tempvars = new vector<Parameter *>();
		}
		| TK_VOID TK_ID TK_LPAR TK_RPAR{
			$$.type = VOID;
			$$.name = new char[strlen($2)+1];
			strcpy($$.name, $2);
			$$.Val = NULL;
			$$.tempvars = new vector<Parameter *>();
			
		}
		| composite_type TK_ID TK_LPAR TK_RPAR{
			$$.type = RECORD;
			$$.name = new char[strlen($2)+1];
			strcpy($$.name, $2);
			delete $2;
			$$.Val = new char[strlen($1.Val)+1];
			strcpy($$.Val,$1.Val);
			delete $1.Val;
			$$.tempvars = new vector<Parameter *>();

			if (root->lookupRecord(DECLARATION, $$.Val) == NULL) {
				string rcn = $$.Val;
				Error = "Return type Error: record "+rcn+" has not been defined";
				yyerror(Error);
				yyerrok;
				sem_errors++;
			} 
		}	
		| data_type TK_ID TK_LPAR formal_params TK_RPAR{
			$$.Val = NULL;
			$$.type  = $1;
			$$.name = new char[strlen($2)+1];
			strcpy($$.name, $2);
			delete $2;

			$$.tempvars = new vector<Parameter *>();

			for(int i = 0; i < $4->size(); i++)
				$$.tempvars->push_back((*$4)[i]);

			bool same = false;
			for (int i = 0; i < param_names.size(); i ++ ){
				for (int j = i+1; j < param_names.size(); j++) {
					if (param_names[i] == param_names[j] ) {
						same = true;
						break;
					}
				}
			}
			if (same) {
				yyerror("Error in function header, parameteres with same name");
				yyerrok;sem_errors++;$$.type = ERROR_TYPE;
			}
			param_names.clear();
		}
		| TK_VOID TK_ID TK_LPAR formal_params TK_RPAR{
			$$.type  = VOID;
			
			$$.name = new char[strlen($2)+1];
		 	strcpy($$.name, $2);
			delete $2;
			$$.Val = NULL;
			$$.tempvars = new vector<Parameter *>();

			for(int i = 0; i < $4->size(); i++)
				$$.tempvars->push_back((*$4)[i]);
			for (int i = 0; i < param_names.size(); i ++ ){
				for (int j = i+1; j < param_names.size(); j++) {
					if (param_names[i] == param_names[j] ) {
						same = true;
						break;
					}
				}
			}
			if (same) {
				yyerror("Error in function header, parameteres with same name");
				yyerrok;sem_errors++;$$.type = ERROR_TYPE;
			}
			param_names.clear();
		}
		| composite_type TK_ID TK_LPAR formal_params TK_RPAR{
			$$.type  = RECORD;
			
			$$.name = new char[strlen($2)+1];
		 	strcpy($$.name, $2);
			delete $2;
			$$.Val = new char[strlen($1.Val)+1];
			strcpy($$.Val, $1.Val);
			delete $1.Val;
			$$.tempvars = new vector<Parameter *>();

			for(int i = 0; i < $4->size(); i++)
				$$.tempvars->push_back((*$4)[i]);
			if (root->lookupRecord(DECLARATION, $$.Val) == NULL) {
				string rcn = $$.Val;
				Error = "Return type Error: record "+rcn+" has not been defined";
				yyerror(Error);
				yyerrok;
				sem_errors++;
			}
			for (int i = 0; i < param_names.size(); i ++ ){
				for (int j = i+1; j < param_names.size(); j++) {
					if (param_names[i] == param_names[j] ) {
						same = true;
						break;
					}
				}
			}
			if (same) {
				yyerror("Error in function header, parameteres with same name");
				yyerrok;sem_errors++;$$.type = ERROR_TYPE;
			}
			param_names.clear();
		}
		| data_type TK_LBRACKET TK_RBRACKET TK_ID TK_LPAR TK_RPAR {
			yyerror("Cannot use a function with array return type");
			sem_errors++;
			yyerrok;
			$$.type = ERROR_TYPE;
		}
		| data_type TK_LBRACKET TK_RBRACKET TK_ID TK_LPAR formal_params TK_RPAR {
			yyerror("Cannot use a function with array return type");
			sem_errors++;
			yyerrok;
			$$.type = ERROR_TYPE;
			for (int i = 0; i < param_names.size(); i ++ ){
				for (int j = i+1; j < param_names.size(); j++) {
					if (param_names[i] == param_names[j] ) {
						same = true;
						break;
					}
				}
			}
			if (same) {
				yyerror("Error in function header, parameteres with same name");
				yyerrok;sem_errors++;
			}
			param_names.clear();
		}
		| composite_type TK_LBRACKET TK_RBRACKET TK_ID TK_LPAR TK_RPAR {
			yyerror("Cannot use a function with array return type");
			sem_errors++;
			yyerrok;
			if (root->lookupRecord(DECLARATION, $1.Val) == NULL) {
				string rcn = $1.Val;
				Error = "Return type Error: record "+rcn+" has not been defined";
				yyerror(Error);
				yyerrok;
				sem_errors++;
			}
			$$.type = ERROR_TYPE;
			
		}
		| composite_type TK_LBRACKET TK_RBRACKET TK_ID TK_LPAR formal_params TK_RPAR {
			yyerror("Cannot use a function with array return type");
			sem_errors++;
			yyerrok;
			if (root->lookupRecord(DECLARATION, $1.Val) == NULL) {
				string rcn = $1.Val;
				Error = "Return type Error: record "+rcn+" has not been defined";
				yyerror(Error);
				yyerrok;
				sem_errors++;
			}
			$$.type = ERROR_TYPE;
			for (int i = 0; i < param_names.size(); i ++ ){
				for (int j = i+1; j < param_names.size(); j++) {
					if (param_names[i] == param_names[j] ) {
						same = true;
						break;
					}
				}
			}
			if (same) {
				yyerror("Error in function header, parameteres with same name");
				yyerrok;sem_errors++;
			}
			param_names.clear();
		}
		| data_type TK_ID TK_LPAR error TK_RPAR {
			yyerror("Error in function's header parameters"); 
			num_errors++; 
			yyerrok; 
			$$.type = ERROR_TYPE;
		}
		| TK_VOID TK_ID TK_LPAR error  TK_RPAR  {
			yyerror("error in function's header parameters"); 
			num_errors++; 
			yyerrok;
			$$.type = ERROR_TYPE;
		}
		| composite_type TK_ID TK_LPAR error TK_RPAR {
			yyerror("error in function's header parameters");
			num_errors++; 
			yyerrok;
			if (root->lookupRecord(DECLARATION, $1.Val) == NULL) {
				string rcn = $1.Val;
				Error = "Return type Error: record "+rcn+" has not been defined";
				yyerror(Error);
				yyerrok;
				sem_errors++;
			}
			$$.type = ERROR_TYPE;
		}
		;

composite_type : TK_RECORD TK_ID {
			$$.type = RECORD;
			$$.Val = new char[strlen($2)+1];
			strcpy($$.Val, $2);
			delete $2;
		}
		;


primitive_type_def_plus : 
	
	primitive_type_def {
		$$ = new vector< vector<Variable *> *>();
		if ($1 != NULL) {
			$$->push_back($1);
		}
	} 
        
	| primitive_type_def_plus primitive_type_def {
		$$ = $1; 
		
		if($2 != NULL)
		{
			$$->push_back($2);
		}
	}
	;
         
primitive_type_def :

	 data_type def_onevar_plus TK_SEMI {

		vector<Variable *>::iterator it;
		Variable *ptr;

		for (it = $2->begin(); it != $2->end(); it++) {
			string name = (*it)->getName();
			if ((ptr = curNode->lookupVariable(DECLARATION, name)) == NULL) {

				/* psaxnw na dw an exei dilwthei sitn idia embeleia record me idio name*/
				Record *tmpr;
				if ((tmpr = curNode->lookupRecord(DECLARATION, name) )== NULL) {
					(*it)->setType($1);
					curNode->insertVariable(name, *it);
				}
				else {
					Error = name+" is previously declared as record "+tmpr->getRecordName();
					delete (*it);
					$2->erase(it); 
					it--;
					yyerror(Error);
				        sem_errors++;
				        yyerrok;
				}
			}
			else {
				Error = "Variable "+name+" has been already declared as ";
				int type = ptr->getType();
				if( type == CHAR )
					Error += "Char";	
				else if (type == INT) 
					Error += "Int";
				else if (type == FLOAT)
					Error += "Float";
				
				yyerror(Error);
        			sem_errors++;
			        yyerrok;

			}
		}
		$$ = $2;				
	}

	| data_type error TK_SEMI {
		yyerror("Error in variable definition");
		num_errors++;yyerrok;
		$$ = NULL;
	}
	| data_type def_onevar_plus error TK_SEMI {
		yyerror("Error in variable definition , Missing , or ; in declarations?");
		for (int i = 0; i < $2->size(); i++)
			delete (*$2)[i];
		delete $2;
		num_errors++;
		yyerrok;
		$$ = NULL;
	}
        ;


composite_type_def : 

	composite_type def_onevar_plus TK_SEMI  {
								
		Record *Re = NULL;
		string recName = $1.Val;
		if( (Re = root->lookupRecord(DECLARATION, recName) ) != NULL)
		{
			for(int i = 0; i < $2->size(); i++)
			{
				string name = (*$2)[i]->getName();
				if( curNode->lookupRecord(DECLARATION,name) == NULL)
				{
	
					if (curNode->lookupVariable(DECLARATION, name) == NULL) {
						Record *temp = new Record(name,recName,Re->getMembers(),false,(*$2)[i]->isArray(),(*$2)[i]->getArraySize());
	                                        curNode->insertRecord(name,temp);
                	                }
                        	        else {
						Error = name+" is previously declared as a variable";
			                        yyerror(Error);
                                		sem_errors++;
 		                                yyerrok;
					}
				}
				else {
					Error = name+" has already been declared as a record";
					yyerror(Error);
                                        sem_errors++;
                                        yyerrok;
				}
			}	
		}
		else {
			Error = "Record "+recName+" has not been declared";
			yyerror(Error);
                        sem_errors++;
                        yyerrok;

		}

		delete $1.Val;
		for (int i = 0; i < $2->size(); i++)
			delete (*$2)[i];
		delete $2;
	}

	| composite_type error TK_SEMI {yyerror("Error in variable definition");num_errors++;yyerrok;delete $1.Val;}
	;


def_onevar_plus : def_one_variable 
		{
			$$ = new vector<Variable *>();
			if ($1 != NULL)
				$$->push_back($1);
		}
		| def_one_variable TK_COMMA def_onevar_plus 
		{
			$$ = $3;
			if ($1 != NULL)
				$$->push_back($1);
		}
		;


def_one_variable : TK_ID 
		{
			Variable *temp = new Variable(0,$1,0,false, false ,0); /*just create it */
			$$ = temp;
		}
		| TK_ID TK_LBRACKET expression  TK_RBRACKET
		{
			if ($3.type == INT && $3.Val != NULL) {
				if ($3.Val[0] != '-') {
					int size = atoi($3.Val);
					if (size > 0) {
						Variable *temp = new Variable(0,$1,0,false, true , size); /*just create it */
						$$ = temp;
					}
					else if (size == 0 ) {
						yyerror("Cannot declare an array with zero size");
						sem_errors++;
						yyerrok;
						$$ = NULL;
					}	
				}
				else {
					yyerror("Cannot declare an array variable with negative size");
					yyerrok; sem_errors++;
					$$ = NULL;
				}
			}
			else { 
				yyerror("Cannot declare an array with variable size");
				yyerrok;
				sem_errors++;
				$$ = NULL;
			}
		}
		| TK_ID TK_LBRACKET error TK_RBRACKET  
		{ 
			$$ = NULL;
			yyerror("Error in declaring array variable");num_errors++;yyerrok;
		}
		;


formal_params : formal_parameter { /* cjeck gia ta idia */
			$$ = new vector<Parameter *>();
			$$->push_back($1);
			
		}
		| formal_parameter TK_COMMA formal_params {
			$$ = $3;
			$$->push_back($1);
		}
		;

formal_parameter : 
	data_type TK_ID {
		Parameter *temp = new Variable($1,$2,0,false, false ,0);
		$$ = temp;
		param_names.push_back($2);
	}
	| data_type TK_AMP TK_ID {
		Parameter *temp = new Variable($1,$3,0,true, false ,0);
		$$ = temp;
		param_names.push_back($3);
	}
	| data_type TK_AMP TK_ID TK_LBRACKET TK_RBRACKET {
		Parameter *temp = new Variable($1,$3,0,true, true ,0);
		$$ = temp;
		param_names.push_back($3);
	}
        | data_type TK_ID TK_LBRACKET TK_RBRACKET{

		Parameter *temp = new Variable($1,$2,0,false, true ,0);
		$$ = temp;
		param_names.push_back($2);
	}
	| data_type TK_ID TK_LBRACKET expression TK_RBRACKET{
	
		Parameter *temp = new Variable($1,$2,0,false, true ,0);
		param_names.push_back($2);
		$$ = temp;
		yyerror("Cannot use array with fixed size as a function's parameter");
		yyerrok;
		sem_errors++;
		param_names.push_back($2);
	}
	| data_type TK_AMP TK_ID TK_LBRACKET expression TK_RBRACKET{
		Parameter *temp = new Variable($1,$3,0,false, true ,0);
		$$ = temp;
		yyerror("Cannot use array with fixed size as a function's parameter");
		yyerrok;
		sem_errors++;
		param_names.push_back($3);

	}
	| composite_type TK_ID{
		Parameter *temp = new Record($2,$1.Val,NULL,false,false,0);
		$$ = temp;
				
		if(root->lookupRecord(DECLARATION, $1.Val) == NULL){
			string rcn = $1.Val;
			Error = "Record "+rcn+" has not been defined";
			yyerror(Error);
        		sem_errors++;
      			yyerrok;
		}
		param_names.push_back($2);
	}
	| composite_type TK_AMP TK_ID{
		Parameter *temp = new Record($3,$1.Val,NULL,true,false,0);
		$$ = temp;
		if(root->lookupRecord(DECLARATION, $1.Val) == NULL){
			string rcn = $1.Val;
			Error = "Record "+rcn+" has not been defined";
			yyerror(Error);
	       		sem_errors++;
		}
		param_names.push_back($3);
	}
	| composite_type TK_AMP TK_ID TK_LBRACKET TK_RBRACKET{
		Parameter *temp = new Record($3,$1.Val,NULL,true,true,0);
		$$ = temp;
		if(root->lookupRecord(DECLARATION, $1.Val) == NULL){
			string rcn = $1.Val;
			Error = "Record "+rcn+" has not been defined";
			yyerror(Error);
        		sem_errors++;
       			yyerrok;
		}
		param_names.push_back($3);

	}
	| composite_type TK_ID TK_LBRACKET TK_RBRACKET{
		Parameter *temp = new Record($2,$1.Val,NULL,false,true,0);					
		$$ = temp;
		if(root->lookupRecord(DECLARATION, $1.Val) == NULL){
			string rcn = $1.Val;
			Error = "Record "+rcn+" has not been defined";
			yyerror(Error);
        		sem_errors++;
      			yyerrok;
		}
		param_names.push_back($2);
	}
	| composite_type TK_AMP TK_ID TK_LBRACKET expression TK_RBRACKET{
		Parameter *temp = new Record($3,$1.Val,NULL,true,true,0);
		$$ = temp;
		if(root->lookupRecord(DECLARATION, $1.Val) == NULL){
			string rcn = $1.Val;
			Error = "Record "+rcn+" has not been defined";
			yyerror(Error);
        		sem_errors++;
       			yyerrok;
		}
		param_names.push_back($3);

	}
	| composite_type TK_ID TK_LBRACKET expression TK_RBRACKET{
		Parameter *temp = new Record($2,$1.Val,NULL,false,true,0);					
		$$ = temp;
		if(root->lookupRecord(DECLARATION, $1.Val) == NULL){
			string rcn = $1.Val;
			Error = "Record "+rcn+" has not been defined";
			yyerror(Error);
        		sem_errors++;
      			yyerrok;
		}
		param_names.push_back($2);
	}    
	;

variable_definition_star : /*empty*/
                         | variable_definition_star variable_definition
                         ;

statement_star : /*empty*/
	       | statement_star statement
	       ;

statement : assignment
          | if_statement
          | while_statement
          | return_statement{
			return_types.push_back($1.type);
	  }
          | compound_statement
          | TK_SEMI /*empty statement*/
          | void_function_call
          ;


void_function_call : function_call TK_SEMI
                   ;

function_call : 
	TK_ID TK_LPAR TK_RPAR {
		/* look if function is declared*/
		Function *fun;
		string name = $1;
		$$.Val = NULL;
		bool mistakes = false;

		if( curNode->lookupVariable(USE,name) != NULL){
			Error = "called object "+name+" is not a function,is a variable";
			yyerror(Error);
			sem_errors++;
			yyerrok;
			$$.type = ERROR_TYPE;
		}
		else if( curNode->lookupRecord(USE,name) != NULL){
                        Error = "called object "+name+" is not a function,is a record ";
                        yyerror(Error);
                        sem_errors++;
                        yyerrok;
                        $$.type = ERROR_TYPE;
		}

		else if ( (fun = root->lookupFunction(name)) != NULL) {
			if (fun->getNumParams() != 0) {
				Error = "Function "+name+" does take arguments";
				string Proto = fun->TakePrototypeString();
				Error += "\n"+Proto;
				yyerror(Error);
				yyerrok;
				sem_errors++;
				$$.type = ERROR_TYPE;
			}
			else {
				$$.type = fun->getType();
				if ($$.type == RECORD) {
					string rcn = fun->getRecName();
					$$.Val = new char[rcn.length()+1];
					strcpy($$.Val, rcn.c_str() );
				}
			}
		}
		else {
			Error = "Function "+name+" has not been declared";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		
	}
	| TK_ID TK_LPAR actual_params TK_RPAR{
	
		/* look if function is declared*/
		Function *fun;
		string name = $1;
		$$.Val = NULL;
		bool mistakes = false;

		if( curNode->lookupVariable(USE,name) != NULL){
			Error = "called object "+name+" is not a function,is a variable";
			yyerror(Error);
			sem_errors++;
			yyerrok;
			$$.type = ERROR_TYPE;
			mistakes = true;
		}
		else if( curNode->lookupRecord(USE,name) != NULL){
                        Error = "called object "+name+" is not a function,is a record ";
                        yyerror(Error);
                        sem_errors++;
                        yyerrok;
                        $$.type = ERROR_TYPE;
			mistakes = true;
		}

		else if ( (fun = root->lookupFunction(name)) != NULL) {
			if (fun->getNumParams() != $3->size() ) {
				Error = "Wrong number of arguments in function "+name;
				string Proto = fun->TakePrototypeString();
				Error += "\n"+Proto;
				yyerror(Error);
				yyerrok;
				sem_errors++;
				$$.type = ERROR_TYPE;
				mistakes =true;
			}
			else {

				vector<Parameter *> *params;
				params = fun->getParams();

				for(int i =0; i < $3->size(); i++){
					int type = (*params)[i]->getType();
					bool array = (*params)[i]->isArray();
				
					if((*$3)[i]->type != (*params)[i]->getType()){
						if ((*$3)[i]->type == STRING && array != true) {
							string temp =(*params)[i]->getName() ;
							Error = "Wrong type of parameter "+temp+" for function "+name;
							yyerror(Error);
	        					sem_errors++;
	        					yyerrok;
							mistakes = true;
						}
					}
					else if ((*$3)[i]->type == (*params)[i]->getType()) {
						if((*$3)[i]->type == RECORD){
							if(curNode->lookupRecord(USE,(*$3)[i]->Val)==NULL){

								string temp = (*$3)[i]->Val;
								Error = "Record "+temp+" has not been defined";
								yyerror(Error);
	        						sem_errors++;
	    							yyerrok;
								mistakes = true;
							}	
						}
						else {
							if ((*$3)[i]->isArray != array) {
								string temp =(*params)[i]->getName() ;
								Error = "Wrong type of parameter "+temp+" for function "+name;
								yyerror(Error);
								sem_errors++;
								yyerrok;mistakes = true;
							}
						}
					}			
				}	
				if(!mistakes){
					$$.type = fun->getType(); 
					if ($$.type == RECORD) {
						string rcn = fun->getRecName();
						$$.Val = new char[rcn.length()+1];
						strcpy($$.Val, rcn.c_str() );
					}
				}
				else $$.type = ERROR_TYPE;
			
			}
		}
		else {
			Error = "Function "+name+" has not been declared";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
	
	}
	| TK_ID TK_LPAR error TK_RPAR {
		yyerror("error in function's call parameters"); 
		num_errors++; 
		yyerrok;
		$$.type = ERROR_TYPE;
	}
              ;

actual_params : 
	actual_parameter {
		$$ = new vector<struct YYSTYPE::type_and_val*>();
		$$->push_back(&$1);
	}
	| actual_parameter TK_COMMA actual_params {
		$$ = $3;
		$$->push_back(&$1);
	}
	;

actual_parameter : 
		expression {
			$$.type = $1.type;
			$$.Val = $1.Val;
			$$.isArray = $1.isArray;
		}
		| TK_STRING { 
			$$.type = STRING;	
			$$.Val = new char[strlen($1)+1];
                        strcpy($$.Val,$1);

		}
		;

if_statement : 
	TK_IF TK_LPAR b_expression TK_RPAR statement {
		if ($3 != BOOL) {
			Error = "Invalid type of if's condition expression, must be binary";
			yyerror(Error);
			yyerrok;
			sem_errors++;
		}
	}
	| TK_IF TK_LPAR b_expression TK_RPAR statement TK_ELSE statement {
		if ($3 != BOOL) {
			Error = "Invalid type of if's condition expression, must be binary";
			yyerror(Error);
			yyerrok;
			sem_errors++;
		}
	}
	| TK_IF TK_LPAR error TK_RPAR statement {yyerror("Error in if's statement binary expression"); num_errors++;yyerrok;}
	| TK_IF TK_LPAR error TK_RPAR statement TK_ELSE statement {yyerror("Error in if's statement binary expression");num_errors++;yyerrok;}
	| TK_IF TK_LPAR b_expression error statement { yyerror("Error in if statement, missing )");num_errors++; yyerrok;}
	| TK_IF TK_LPAR b_expression error statement TK_ELSE statement { yyerror("Error in if statement, missing )");num_errors++; yyerrok;}
	| TK_IF error TK_RPAR statement {yyerror("Error in if statement, missing ("); yyerrok;}
	| TK_IF error TK_RPAR statement TK_ELSE statement {yyerror("Error in if statement, missing ("); yyerrok;}
	;

while_statement : TK_WHILE TK_LPAR b_expression TK_RPAR statement {
			if ($3 != BOOL) {
				Error = "Invalid type of if's condition expression, must be binary";
				yyerror(Error);
				yyerrok;
				sem_errors++;
			}
		}
		| TK_WHILE TK_LPAR error TK_RPAR statement {yyerror("Error in while's statement binary expression"); num_errors++;yyerrok;}
		| TK_WHILE  error TK_RPAR statement {yyerror("Error in while missing ("); num_errors++;yyerrok;}
		| TK_WHILE TK_LPAR error statement {yyerror("Error in while missing )"); num_errors++;yyerrok;}
                ;

return_statement : TK_RETURN TK_SEMI{
			$$.type = VOID;
			}
                 | TK_RETURN expression TK_SEMI{
		
			$$.type = $2.type;
		 }
		 | TK_RETURN error TK_SEMI {yyerror("error in return statement"); num_errors++; yyerrok;$$.type = ERROR_TYPE;}
                 ;

assignment :  
	comp_l_value TK_MAKE_EQ expression TK_SEMI {
		if ( $1.isArray == true || $3.isArray == true) {	
			Error = "operand of assignemnt is not possible to be array's  name";
			yyerror(Error);
			sem_errors;
			yyerrok;
		}
		else if ( $1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Invalid Assignement";
			yyerror(Error);
			sem_errors++;
			yyerrok;
		}
		else if ( $1.type != $3.type && !( $1.type == FLOAT && $3.type == INT) )   /* ean einai diaforetikou tupou kai den einai float = int */
		{	/*error*/
			Error = "Cannot Assign a ";

			if( $3.type == CHAR )
				Error += "Char";
			else if ($3.type == INT)
				Error += "Int";
			else if ($3.type == FLOAT)
				Error += "Float";
			else if ($3.type == VOID )
				Error += "Void";
			else if ($3.type == RECORD) {
	                        string tmp = $3.Val;
	                        Error += "Record "+tmp;
	                }
			else if ($3.type == STRING)
				Error += "String";


			Error += " Value to a ";

			if( $1.type == CHAR )
				Error += "Char";
			else if ($1.type == INT)
				Error += "Int";
			else if ($1.type == FLOAT)
				Error += "Float";
                        else if ($1.type == VOID )
                                Error += "Void";
                        else if ($1.type == RECORD) {
                                string tmp = $1.Val;
                                Error += "Record "+tmp;
			}
			else if($1.type == STRING)
				Error += "String ";
                        

			Error += " Variable";
			yyerror(Error);
			sem_errors++;
			yyerrok;
		}
	}
	| comp_l_value TK_MAKE_EQ error TK_SEMI {yyerror("Syntax Error in assignment");num_errors++; yyerrok;}
	| comp_l_value error expression TK_SEMI {yyerror("Syntax Error in assignment missing '=' symbol"); num_errors++; yyerrok;}
        ;


comp_l_value : 
	TK_ID TK_SDOT l_value {
		$$.Val = NULL;
		string recname = $1;
		Record *temp = NULL;
		
		if ((temp = curNode->lookupRecord(USE, recname)) != NULL) {
			if (temp->isArrayRec()) {
				Error = "Record "+temp->getRecordName()+" "+recname+" is declared as an array of records";
				yyerror(Error);yyerrok;sem_errors++;
				$$.type = ERROR_TYPE;
			}
			else if ($3.name != NULL) {
				string name = $3.name;		
				Variable *var = temp->searchMember($3.name);
				if (var != NULL) {
					if ( var->isArray()) { /* an einai pinakas */
						if ( $3.indexType != INT && $3.indexType != ERROR_TYPE) {
							Error = "Index of the array must be a positive integer";
							yyerror(Error);
							yyerrok;
							sem_errors++;
							$$.type = ERROR_TYPE;
							$$.isArray = false;
						}
						else if ( $3.indexType == INT) {
							$$.isArray = false; /* its an element */
							if ($3.arrayIndex != NULL) { /*an dwthike x[3] elegxw an to 3 einai entos oriwn pinaka */
								if($3.arrayIndex[0] != '-') {
									int index = atoi($3.arrayIndex);
									if (index >= var->getArraySize()) {
										string ind = $3.arrayIndex;
									Error = "Use of the array ("+recname+"."+name+"["+ind+"]) is out of bounds ";
										yyerror(Error);
										yyerrok;
										sem_errors++;
										$$.type = ERROR_TYPE;
									}
									else $$.type = var->getType();
								}
								else {
									Error = "Cannot refer tou an array element with negative index";
									yyerror(Error);
									yyerrok;
									sem_errors++;
									$$.type = ERROR_TYPE;
								}
							}
							else $$.type = var->getType();
						}
						else {
							$$.isArray = true; /* sketo to onoma tou array */
							$$.type = var->getType();
						}
					}
					else {	/* ean den einai array */
					
						if ( $3.indexType != ERROR_TYPE && $3.arrayIndex != NULL ) {
							/*an prokeitai gia xrisi pinaka */		
							Error = recname+"."+name+" is not an array type";
							yyerror(Error);
							yyerrok;
							sem_errors++;
							$$.type = ERROR_TYPE;
						}
						else {
							$$.type = var->getType();
						}
						$$.isArray = false;
					}
				}
				else {
					Error = "Record "+temp->getRecordName()+" has no member "+name;
					yyerror(Error);
					sem_errors++;
					yyerrok;
					$$.type = ERROR_TYPE;
				}
			}
			else {
				sem_errors++;
				$$.type = ERROR_TYPE;
				$$.isArray = false;
			}
		}	
		else {
			Error = recname+" is not declared as a record type";
			yyerror(Error);
			sem_errors++;
			yyerrok;
			$$.type = ERROR_TYPE;
			$$.isArray = false;
		}
		delete $1;
	}

	| l_value {
		$$.Val = NULL;
		if ($1.name != NULL) {
			string name = $1.name;
			Variable *temp;
			if ( (temp = curNode->lookupVariable(USE, name)) != NULL) {
				if ( temp->isArray()) { /* an einai pinakas */

					if ( $1.indexType != INT && $1.indexType != ERROR_TYPE) {
						Error = "Index of the array must be a positive integer";
						yyerror(Error);
						yyerrok;
						sem_errors++;
						$$.type = ERROR_TYPE;
						$$.isArray = false;
					}
					else if ( $1.indexType == INT) {
						$$.isArray = false; /* its an element */
						if ($1.arrayIndex != NULL) { /*an dwthike x[3] elegxw an to 3 einai entos oriwn pinaka */
							if ($1.arrayIndex[0] != '-') {
								int index = atoi($1.arrayIndex);
								if (index >= temp->getArraySize()) {
									string ind = $1.arrayIndex;
									Error = "Use of the array ("+name+"["+ind+"]) is out of bounds ";
									yyerror(Error);
									yyerrok;
									sem_errors++;
									$$.type = ERROR_TYPE;
								}
								else
									$$.type = temp->getType();
							}
							else {
								Error = "Cannot refer tou an array element with negative index";
								yyerror(Error);
								yyerrok;
								sem_errors++;
								$$.type = ERROR_TYPE;
							}
						}
						else	$$.type = temp->getType();
					}
					else {
						$$.isArray = true; /* sketo to onoma tou array */
						$$.type = temp->getType();
					}
				}	
				else {	/* ean den einai array */
					
					if ( $1.indexType != ERROR_TYPE && $1.arrayIndex != NULL ) {
							/*an prokeitai gia xrisi pinaka */		
						Error = name+" is not an array type";
						yyerror(Error);
						yyerrok;
						sem_errors++;
						$$.type = ERROR_TYPE;
					}
					else {
						$$.type = temp->getType();
						
					}	
					$$.isArray = false;
				}
			}
			else {	
				Record *tmp;
				$$.isArray = false;

				if ((tmp = curNode->lookupRecord(USE, name)) != NULL) {
					$$.type = RECORD;
					string recName = tmp->getRecordName();
					$$.Val = new char[recName.length()+1];
					strcpy($$.Val,recName.c_str());

					if (tmp->isArrayRec() == true) { /* an eina iarray of record*/

					}

				}
				else {
					Error = "Use of variable "+name+" without being declared";
					yyerror(Error);
					yyerrok;
					sem_errors++;
					$$.type = ERROR_TYPE;
					$$.Val = NULL;
				}
			}     
			delete $1.name;
		}
		else {	
			num_errors++;
			$$.type = ERROR_TYPE;
		}
	}
	     
	| TK_ID TK_SDOT error TK_SEMI {
		delete $1;
		yyerror("Error in using a record variable");
		$$.type = ERROR_TYPE;
		num_errors++;
		$$.isArray = false;
		$$.Val = NULL;
	}
	;


l_value : 
	TK_ID {
		$$.name = new char [strlen($1)+1];
		strcpy($$.name, $1);
		Variable *temp;
		if ( (temp = curNode->lookupVariable(USE, $$.name)) != NULL) {
			if(temp->isArray())	$$.isArray = true;
			else 			$$.isArray = false;
		}		
		
		$$.isElement = false;
		$$.arrayIndex = NULL;
		$$.indexType = ERROR_TYPE;
		
		delete $1;
	}
	| TK_ID TK_LBRACKET expression TK_RBRACKET {
		$$.name = new char [strlen($1)+1];
		strcpy($$.name, $1);

		$$.isElement = true;
		$$.isArray = false;
		$$.arrayIndex = $3.Val;
		$$.indexType = $3.type;

		delete $1;
	}

	| TK_ID TK_LBRACKET error TK_RBRACKET {
		yyerror("Error in use of an array variable (empty/wrong index used?)");
		yyerrok;
		num_errors++;

		$$.name = NULL;
		$$.arrayIndex = NULL;
		$$.indexType = ERROR_TYPE;
		$$.isElement = false;
		$$.isArray = false;
		delete $1;
	}
	;




compound_statement : open_scope variable_definition_star statement_star close_scope 
		   | open_scope error close_scope {yyerror("Syntax Error in compound statement");num_errors++; yyerrok;}
		   ;




data_type : TK_INT_TYPE 	{ $$ = INT;  }
          | TK_FLOAT_TYPE 	{ $$ = FLOAT;}
          | TK_CHAR_TYPE 	{ $$ = CHAR; }
	  ;

b_expression : 
	b_expression TK_OR b_expression{
		if ($1 == BOOL && $3 == BOOL) {
			$$ = BOOL;
		}
		else if($1 == ERROR_TYPE || $3 == ERROR_TYPE) {
			Error = "Binary OR operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
		else {
			Error = OperationError("Binary OR operation",$1,$3,NULL,NULL);
			$$ = ERROR_TYPE;	
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| b_expression TK_AND b_expression {
		if ($1 == BOOL && $3 == BOOL) {
			$$ = BOOL;
		}
		else if($1 == ERROR_TYPE || $3 == ERROR_TYPE) {
			Error = "Binary AND operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
		else {
			Error = OperationError("Binary AND operation",$1,$3,NULL,NULL);
			$$ = ERROR_TYPE;	
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| TK_NOT b_expression {
		if ($2 == BOOL) {
			$$ = BOOL;
		}
	    	else if ($2 == ERROR_TYPE) {
			Error = "Binary NOT operation is not possible due to abnormal type of operand";
			yyerror(Error);
 			yyerrok;
 			sem_errors++;
			$$ = ERROR_TYPE;				
		}
		else {
			Error = "Binary NOT operation is not allowed for a ";
			if( $2 == CHAR )
				Error += "Char";
			else if ($2 == INT)
				Error += "Int";
			else if ($2 == FLOAT)
				Error += "Float";
			Error += " operand";
			$$ = ERROR_TYPE;	
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| expression TK_EQ expression {
		if($1.isArray || $3.isArray){
			Error = "Equal check operation between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$= ERROR_TYPE;
		}
		else if ($1.type == INT && $3.type == INT) {
			$$ = BOOL;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$ = BOOL;
		}
		else if ($1.type == CHAR && $3.type == CHAR) {
			$$ = BOOL;
		}
		else if( ($1.type == FLOAT && $3.type == INT) ||
                         ($1.type == INT && $3.type == FLOAT))
                {
                        $$ = BOOL;
                }
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Equal check operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
	
		else {
			$$ = ERROR_TYPE;
			Error = OperationError("Equal check operation",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| expression TK_DIFF expression{

		if($1.isArray || $3.isArray){
			Error = "Not Equal check operation between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$= ERROR_TYPE;
		}
		else if ($1.type == INT && $3.type == INT) {
			$$ = BOOL;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$ = BOOL;
		}
		else if ($1.type == CHAR && $3.type == CHAR) {
			$$ = BOOL;
		}
		else if( ($1.type == FLOAT && $3.type == INT) ||
                         ($1.type == INT && $3.type == FLOAT))
                {
                        $$ = BOOL;
                }

		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Not Equal check operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
		
		else {
			$$ = ERROR_TYPE;
			Error = OperationError("Not Equal check operation",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| expression TK_GR expression{

		if($1.isArray || $3.isArray){
			Error = "Greater check operation between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$= ERROR_TYPE;
		}
		else if ($1.type == INT && $3.type == INT) {
			$$ = BOOL;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$ = BOOL;
		}
		else if ($1.type == CHAR && $3.type == CHAR) {
			$$ = BOOL;
		}
		else if( ($1.type == FLOAT && $3.type == INT) ||
                         ($1.type == INT && $3.type == FLOAT))
                {
                        $$ = BOOL;
                }
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Greater check operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
		
		else {
			$$ = ERROR_TYPE;
			Error = OperationError("Greater check operation",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| expression TK_LESS expression{
		if($1.isArray || $3.isArray){
			Error = "Less check operation between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$= ERROR_TYPE;
		}
		else if ($1.type == INT && $3.type == INT) {
			$$ = BOOL;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$ = BOOL;
		}
		else if ($1.type == CHAR && $3.type == CHAR) {
			$$ = BOOL;
		}
		else if( ($1.type == FLOAT && $3.type == INT) ||
                         ($1.type == INT && $3.type == FLOAT))
                {
                        $$ = BOOL;
                }
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Less check operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
		
		else {
			$$ = ERROR_TYPE;
			Error = OperationError("Less check operation",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| expression TK_GOE expression{

		if($1.isArray || $3.isArray){
			Error = "Greater or Equal check operation between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$= ERROR_TYPE;
		}
		else if ($1.type == INT && $3.type == INT) {
			$$ = BOOL;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$ = BOOL;
		}
		else if ($1.type == CHAR && $3.type == CHAR) {
			$$ = BOOL;
		}
		else if( ($1.type == FLOAT && $3.type == INT) ||
                         ($1.type == INT && $3.type == FLOAT))
                {
                        $$ = FLOAT;
                }
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Greater or Equal check operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}
		else {
			$$ = ERROR_TYPE;
			Error = OperationError("Greater or Equal check operation",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	| expression TK_LOE expression {
		if($1.isArray || $3.isArray){
			Error = "Less or Equal check operation between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$= ERROR_TYPE;
		}
		else if ($1.type == INT && $3.type == INT) {
			$$ = BOOL;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$ = BOOL;
		}
		else if( ($1.type == FLOAT && $3.type == INT) ||
                         ($1.type == INT && $3.type == FLOAT))
                {
                        $$ = BOOL;
                }

		else if ($1.type == CHAR && $3.type == CHAR) {
			$$ = BOOL;
		}
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Less or Equal check operation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$ = ERROR_TYPE;
		}

		else {
			$$ = ERROR_TYPE;
			Error = OperationError("Less or Equal check operation",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
	}
	;



expression : 
	expression TK_ADD expression {
		
		if($1.isArray || $3.isArray){

			Error = "Add between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if($1.type == INT && $3.type == INT) {
			$$.type = INT;
		}
		else if($1.type == FLOAT && $3.type == FLOAT){
			$$.type = FLOAT;
		}
		else if( ($1.type == FLOAT && $3.type == INT) || 
			 ($1.type == INT && $3.type == FLOAT))
		{
			$$.type = FLOAT;
		}			
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Add is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else {
			$$.type = ERROR_TYPE;
			Error = OperationError("Add between a ",$1.type,$3.type,$1.Val,$3.Val);	
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
		$$.Val = NULL;
	}
	| expression TK_SUB expression {

		if($1.isArray || $3.isArray){

			Error = "Sub between array operand/s is not possible";			
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if($1.type == INT && $3.type == INT) {
			$$.type = INT;
		}
		else if($1.type == FLOAT && $3.type == FLOAT){
			$$.type = FLOAT;
		}
		else if( ($1.type == FLOAT && $3.type == INT) || 
			 ($1.type == INT && $3.type == FLOAT))
		{
			$$.type = FLOAT;
		}
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Sub is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else {
			$$.type = ERROR_TYPE;
			Error = OperationError("Sub between a ",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}

		$$.Val = NULL;
	}
	| expression TK_MULT expression {
		if($1.isArray || $3.isArray){
			Error = "Multiply between array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if($1.type == INT && $3.type == INT) {
			$$.type = INT;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			$$.type = FLOAT;
		}
		else if( ($1.type == FLOAT && $3.type == INT) || 
			 ($1.type == INT && $3.type == FLOAT))
		{
			$$.type = FLOAT;
		}
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Multiply is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		
		else {
			$$.type = ERROR_TYPE;
			Error = OperationError("Multiply of a ",$1.type,$3.type,$1.Val,$3.Val);	
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}

		$$.Val = NULL;
	}
	| expression TK_DIV expression {

		if($1.isArray || $3.isArray){

			Error = "Division between array operand/s is not possible";			
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if($1.type == INT && $3.type == INT) {
			if ($3.Val != NULL) {
				int val = atoi($3.Val);
				if ( val == 0 ) {
					$$.type = ERROR_TYPE;
					Error = "Error division by zero";
					yyerror(Error);
					yyerrok;
 					sem_errors++;
				}				
				else 
					$$.type = INT;
			}
			else
				$$.type = INT;
		}
		else if ($1.type == FLOAT && $3.type == FLOAT) {
			if ($3.Val != NULL) {
				float val = atof($3.Val);
				if (val == 0.0) {
					$$.type = ERROR_TYPE;
					Error = "Error division by zero(float 0.0)";
					yyerror(Error);
					yyerrok;
 					sem_errors++;
				}
				else $$.type = FLOAT;
			}
			else $$.type = FLOAT;
		}
		else if ($1.type == FLOAT && $3.type == INT) {
			if ($3.Val != NULL) {
				int val = atoi($3.Val);
				if ( val == 0 ) {
					$$.type = ERROR_TYPE;
					Error = "Error division by zero";
					yyerror(Error);
					yyerrok;
 					sem_errors++;
				}				
				else 
					$$.type = FLOAT;
			}
			else
				$$.type = FLOAT;
		}
		else if($1.type == INT && $3.type == FLOAT) {
			if ($3.Val != NULL) {
				float val = atof($3.Val);
				if (val == 0.0) {
					$$.type = ERROR_TYPE;
					Error = "Error division by zero(float 0.0)";
					yyerror(Error);
					yyerrok;
 					sem_errors++;
				}
				else $$.type = FLOAT;
			}
			else $$.type = FLOAT;
		}
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Division is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		
		else {
			$$.type = ERROR_TYPE;
			Error = OperationError("Division of a ",$1.type,$3.type,$1.Val,$3.Val);
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}

		$$.Val = NULL;
	}	
	| expression TK_MOD expression {
		if($1.isArray || $3.isArray){
			Error = "Mod calculation with array operand/s is not possible";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if($1.type == INT && $3.type == INT) {
			if ($3.Val != NULL) {
				int val = atoi($3.Val);
				if ( val == 0 ) {
					$$.type = ERROR_TYPE;
					Error = "Error mod calculation with zero divisor";
					yyerror(Error);
					yyerrok;
					sem_errors++;
				}
				else $$.type = INT;
			}
			else $$.type = INT;
		}
		else if($1.type == ERROR_TYPE || $3.type == ERROR_TYPE) {
			Error = "Mod calculation is not possible due to abnormal type of operand/s";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		
		else {
			$$.type = ERROR_TYPE;
			Error = OperationError("Mod calculation",$1.type,$3.type,$1.Val,$3.Val) ;
			yyerror(Error);
			yyerrok;
 			sem_errors++;
		}
		$$.Val = NULL;
	}
	| TK_ADD expression %prec TK_PLUS { 
		if($2.isArray){
			Error = "Error, array type is not allowed after operand +";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if ($2.type == CHAR) {
			$$.type = ERROR_TYPE;
			Error = "Error ,Char type is not allowed after operand +";
			yyerror(Error);
			yyerrok;
			sem_errors++;
		}
		else if ($2.type == ERROR_TYPE) {
			Error = "Error invalid following expression after operand +";
			yyerror(Error);
			yyerrok;
 			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if ($2.type == RECORD) {
			string tmp = $2.Val;
			Error = "Error, record "+tmp+" is not allowed after operand +";
			yyerror(Error);
                        yyerrok;
                        sem_errors++;
                        $$.type = ERROR_TYPE;
			delete $2.Val;
		}
		else $$.type = $2.type;
		$$.Val = $2.Val;
	}
	| TK_SUB expression %prec TK_MINUS {  

		if($2.isArray){
			Error = "Error, array type is not allowed after operand -";
			yyerror(Error);
			yyerrok;
			sem_errors++;
			$$.type = ERROR_TYPE;
		}
		else if ($2.type == CHAR) {
			$$.type = ERROR_TYPE;
			Error = "Error ,Char type is not allowed after operand -";
			yyerror(Error);
			yyerrok;
			sem_errors++;
		}
		else if ($2.type == ERROR_TYPE) {
			$$.type = ERROR_TYPE;
			Error = "Error invalid following expression after operand -";
			yyerror(Error);
			yyerrok;
 			sem_errors++;	
		}
		else if ($2.type == RECORD) {
                        string tmp = $2.Val;
                        Error = "Error, record "+tmp+" is not allowed after operand -";
                        yyerror(Error);
                        yyerrok;
                        sem_errors++;
                        $$.type = ERROR_TYPE;
                        delete $2.Val;
                }
		else $$.type = $2.type;
		if ($2.Val != NULL) {
			string subv = $2.Val;
			subv = "-"+subv;
			char *newv = new char[subv.length()+1];
			strcpy(newv, subv.c_str());
			$$.Val = newv;
		}
		else $$.Val = NULL;
	}
	| TK_INT{ 
		stringstream str;
		str << $1;
		string temp  = str.str();
		$$.Val = new char[temp.length()+1];
		strcpy($$.Val,temp.c_str());
		$$.type = INT; 
		$$.isArray = false;
	}
	| TK_CHAR {
		$$.Val = new char[1];
		$$.Val[0] = $1;
		$$.type = CHAR;
		$$.isArray = false;
	}
	| TK_FLOAT {
		stringstream str;
		str << $1;
		string temp  = str.str();
		$$.Val = new char[temp.length()+1];
		strcpy($$.Val,temp.c_str());
		$$.type = FLOAT;
		$$.isArray = false;
	}
	| comp_l_value {
		$$.isArray = $1.isArray;
		if ($1.type == ERROR_TYPE) {
			Error = "Error invalid type of left-value";
			yyerror(Error);
			yyerrok;
 			sem_errors++;
			$$.type = ERROR_TYPE;
			$$.isArray = false;
		}
		else $$.type = $1.type;

		if ($1.type == RECORD) {
			$$.Val = new char[strlen($1.Val)+1];
			strcpy($$.Val,$1.Val);
			delete $1.Val;
		}
		else
			$$.Val = NULL;
		
	}
	| function_call { 
		if ($1.type == ERROR_TYPE) {
			Error = "Error invalid return type of function";
			yyerror(Error);
			yyerrok;
 			sem_errors++;	
			$$.type = ERROR_TYPE;
		}
		else $$.type = $1.type;
		if ($1.type == RECORD) {
			$$.Val = new char[strlen($1.Val)+1];
			strcpy($$.Val, $1.Val);
			delete $1.Val;
		}
		else
			$$.Val = NULL;
	}
	;
%%


void yyerror (string s)
{
	cout<<yylineno<<":Error:"<<s<<"!"<<endl;
}


string getType(int type) {
	if( type == CHAR ) return "char";
	else if (type == INT) return "int";
	else if (type == FLOAT) return "float";
        else if (type == VOID) return "void";
        else if (type == RECORD) return "record";
	else return "Error";
}

string OperationError(string s, int type1, int type2,char *val1,char *val2)
{
	
	string Error = s+" between a ";
	if( type1 == CHAR )
		Error += "char";
	else if (type1 == INT)
		Error += "int";
	else if (type1 == FLOAT)
		Error += "float";
	else if (type1 == VOID)
		Error += "void";
	else if (type1 == RECORD) {
		string tmp = val1;
		Error += "record "+tmp;
	}
				
	Error += " And a ";
	if( type2 == CHAR )
		Error += "char";
	else if (type2 == INT)
		Error += "int";
	else if (type2 == FLOAT)
		Error += "float";
	else if (type1 == VOID)
                Error += "void";
        else if (type1 == RECORD) {
		string tmp = val2;
                Error += "record "+tmp;
	}
	Error += " operand is not allowed";
	return Error;
}
int main(int argc, char *argv[]) {
	if (argc != 1)
	{
		if (argv[1] != NULL)
			yyin = fopen( argv[1], "r" );
	}

	int ret = yyparse();
	if (ret == 0) {
		if((!num_errors) && (!sem_errors))
			cout<<"Parsing was successful!"<<endl;
		else
			cout<<"Parsing has failed, "<<num_errors<<" sytax errors, and "<< sem_errors<<" semantic errors occured"<<endl;

		
	}
	else if (ret == 1)
                cout<<"Parsing has failed,syntax error!"<<endl;
	else if (ret == 2)
                cout<<"Parsing has failed,memory exhaustion!"<<endl;

	return 0;
}
