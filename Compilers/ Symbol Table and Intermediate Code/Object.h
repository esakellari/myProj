#ifndef _OBJECTS_
#define _OBJECTS_

#include <iostream>
#include <string>
#include <vector>


using namespace std;

#define ERROR_TYPE -1
#define CHAR 0
#define INT 1
#define FLOAT 2
#define STRING 3
#define BOOL 4
#define VOID 5
#define RECORD 6
#define VARIABLE 7


class Parameter {
	public :
		Parameter() {};
		virtual int getType() const {} ;
		virtual string getRecordName() {};
		virtual int getKind() {};
		virtual bool isArray(){};
		virtual string getName() {};
		virtual int getArraySize(){};
		virtual bool isByref(){};
		
};

class Variable : public Parameter{
	private:
		int type;					// variable type "INT","FLOAT","CHAR","STRING"
		string name;					// variable name
		bool ArrayVar;					// if variable is array
		int size;					
		int ArraySize;					// array size
		bool byRef;
	public:
		Variable(int _type,string _name,int _size,bool _byRef,bool Array ,int _ArraySize);
		string getName();
		int getType() const;
		void setType(int newtype);
		bool isArray();
		bool isByref();
		int getArraySize();
		int getKind();
		~Variable();
};


class Function {

		private:
			vector<Parameter *> *parameters;
			int type;
			string name;
			string recName;		 
		public:
			Function(int _type,string _recname,string _name,vector<Parameter *> *param);
			string getRecName();
			string TakePrototypeString();
			string	TakeParam(int x);
			int getNumParams();
			vector<Parameter *> *getParams();
			int getType();
			
};

class Record :public Parameter{

	private:
        	vector<Variable *> *members;
             string name;
		string RecordName;
		bool isArray;
		bool byRef;
		int size;

	public:
		Record(string _name,string _recname, vector<Variable *> *memb, bool _byRef,bool _isArray, int _size);
		void AddMember( Variable * newMember);
		vector<Variable *> *getMembers();
		Variable *searchMember(string nam);
		string getName();
		int getType() const;
		string getRecordName();
		int getSize();
		int getKind();
		bool isArrayRec();
		bool isByref();
};


#endif
