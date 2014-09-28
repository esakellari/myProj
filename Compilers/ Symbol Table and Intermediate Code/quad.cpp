#include "quad.h"

Quad ::Quad(string _op,string _arg1, string _arg2, string _result,int _label){
		
	op = _op;
	arg1 = _arg1;
	arg2 = _arg2;
	result = _result;
	label = _label;
		
}

void Quad::setResult(int label) {
	stringstream str;
	str << label;
	result = str.str();
}

void Quad::printQuad(){

	cout<<label<<":  "<<op<<", " <<arg1<<", "<<arg2<<", "<<result<<endl;
	if (op == "endu")	cout<<" "<<endl;

}

QuadFactory::QuadFactory(){ 
	LabelCount = 1 ;
	TempCount = 1 ;
}

QuadFactory::~QuadFactory() {
	for (int i = 0; i < Quads.size(); i++) {
		delete Quads[i];
	}
	Quads.clear();
}
		
int QuadFactory::emit(string op,string arg1, string arg2, string &result){
			
		if(result == "$"){ /*temp var*/
			result = newTemp();
		}
		Quad *q1 = new Quad(op,arg1,arg2,result,LabelCount);

		Quads.push_back(q1);

		return LabelCount++;					
}
		
void QuadFactory::backpatch(vector<int> *v1, int label){
	for (int i = 0; i < v1->size(); i++) {
		Quads[(*v1)[i]-1]->setResult(label);
	}
}
		
string QuadFactory::newTemp(){
	stringstream str;
	str << TempCount++;
	string temp  = "$"+str.str();
	return temp;
}

int QuadFactory::getNextQuad() {
	return LabelCount;
}

void QuadFactory::printMiddleCode() {
	cout<<"Middle Code"<<endl;
	for (int i = 0; i < Quads.size(); i++)
		Quads[i]->printQuad();
	cout<<"--------------------"<<endl;
}

