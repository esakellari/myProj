#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


class Quad {
	private: 
		int label;
		string op;
		string arg1,arg2,result;
		vector<int> TrueList,FalseList;
		
	public:
		Quad(string _op,string _arg1, string _arg2, string _result,int _label);
		void printQuad();
		void setResult(int label);
};


class QuadFactory{
	private:
		vector<Quad *> Quads;
		int LabelCount,TempCount;

	public:
		QuadFactory();
		~QuadFactory();
		int emit(string op,string arg1, string arg2, string &result);
		string newTemp();
		int getNextQuad();
		void backpatch(vector<int> *v1, int label);
		void printMiddleCode();
};
