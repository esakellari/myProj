#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;

int main()
{
	string line,first,second;
	size_t del;
	
	list<pair<int,int> > mList;
	map<int, map<int, int> > NMap;
	map<int, map<int, int> >::iterator it;

	while(getline(cin, line)){
		
		del = line.find("|");

		first = line.substr(0,del);
		second = line.substr(del+1, line.size()-1);
		int first_i = atoi(first.c_str());
		int sec_i = atoi(second.c_str());
		
		if((it=NMap.find(first_i))!= NMap.end()){		
			if(it->second.find(sec_i)==it->second.end())
				it->second.insert(pair<int, int>(sec_i, it->second.size()+1));
		}else{
			map<int,int> secM;
			secM[sec_i]=1;
			NMap.insert(pair<int,map<int, int> >(first_i,secM));
		}
		pair<int,int> p(first_i,sec_i);
		mList.push_back(p);
	}
	while(!mList.empty()){
		pair<int,int> ls = mList.front();
			
		it= NMap.find(ls.first);
	
		map<int,int> secM = it->second;
		map<int,int>::iterator it2;
		it2=  secM.find(ls.second);

		cout<<ls.first<<"|"<<ls.second<<"["<<it2->second<<" of "<<secM.size()<<"]"<<endl;
		mList.pop_front();
	}
	return(0);
}
