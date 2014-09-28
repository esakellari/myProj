#include <naxos.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "strtok.h"

using namespace std;
using namespace naxos;

bool contains(NsDeque <NsInt> Con , NsInt Element){

	unsigned i ;
	for(i = 0; i < Con.size(); ++i){
	
		if(Con[i] == Element)
			return true;
	}
	return false;
}

	int
main (int argc, char *argv[])
{
    try  {

      //  Checking input arguments.

      if ( argc  !=  2 )   {

	cerr << "Correct syntax is:\t" << argv[0]
		<< " <flight_data_file>\n";
	return  1;
      }


      //  Opening input filename, e.g. `flight_data.pl'.

      ifstream  file(argv[1]);

      if ( ! file )   {

	cerr << "Could not open `" << argv[1] << "'\n";
	return  1;
      }
      file.exceptions( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
      
      string  input;
      getline(file, input);	// Ignore the first line.


      for (int I=1;  I <= 16;  ++I)   {

	cout << "I = " << I << "\n";


	unsigned  pairing, flight;
	NsInt                      N;
	NsDeque< NsDeque<NsInt> >  P;
	NsDeque<NsInt>             C;

	if ( I  <=  6 )    {

		string  tok;

		file >> input;	// First line is `get_flight_data(1,10,'.
		StrTokenizer  first_tokens(input, ",");
		first_tokens.next();	// First token is ignored.
		tok = first_tokens.next();
		N  =  atoi(tok.c_str());

		while ( file >> input )    {

			StrTokenizer  tokens(input, ",] ");

			while ( ! (tok = tokens.next()).empty()
					&& tok != ")." )
			{
				if ( tok[0]  ==  '[' )   {
					P.push_back( NsDeque<NsInt>() );
					tok = tok.substr(1);
					if ( tok[0]  ==  '[' )
						tok = tok.substr(1);
				}

				P.back().push_back(atoi(tok.c_str()));
			}
			if ( tok  ==  ")." )
				break;
		}
		C = P.back();
		P.pop_back();

	}  else  {

		string  filename;
		switch ( I )   {
			case  7:
				filename = "acsdata/acs197x17";
				break;
			case  8:
				filename = "acsdata/acs294x19";
				break;
			case  9:
				filename = "acsdata/acs404x19";
				break;
			case  10:
				filename = "acsdata/acs434x24";
				break;
			case  11:
				filename = "acsdata/acs467x31";
				break;
			case  12:
				filename = "acsdata/acs577x25";
				break;
			case  13:
				filename = "acsdata/acs619x23";
				break;
			case  14:
				filename = "acsdata/acs626x27";
				break;
			case  15:
				filename = "acsdata/acs711x19";
				break;
			case  16:
				filename = "acsdata/acs685x22";
				break;
			default:
				cerr << "Wrong `I'\n";
				return  1;
				break;
		}

		ifstream  fileAcs(filename.c_str());
		if ( ! fileAcs )   {
			cerr << "Could not open `" << filename << "'\n";
			return  1;
		}
		fileAcs.exceptions( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
		fileAcs >> N;
		unsigned   M;
		fileAcs >> M;
		//  Ignoring three tokens.
		fileAcs >> input;
		fileAcs >> input;
		fileAcs >> input;
		NsInt  number;
		for (pairing=0;  pairing < M;  ++pairing)   {
			fileAcs >> number;
			C.push_back(number);
			P.push_back( NsDeque<NsInt>() );

			unsigned  K;
			fileAcs >> K;
			//  Ignoring three tokens.
			fileAcs >> input;
			fileAcs >> input;
			fileAcs >> input;
			for (flight=0;  flight < K;  ++flight)   {
				fileAcs >> number;
				P.back().push_back(number);
			}
		}
		fileAcs.close();
	}

	//  Print N, P, C.
	cout << "N = " << N << "\n";
	/*cout << "P = [";
	for (pairing=0;  pairing < P.size();  ++pairing)    {

		if ( pairing > 0 )
			cout << ",\n     ";

		cout << "[";
		for (flight=0;  flight < P[pairing].size();  ++flight)   {
			if ( flight > 0 )
				cout << ", ";
			cout << P[pairing][flight];
		}
		cout << "]";

		cout << " / " << C[pairing];
	}
	cout << "]\n";*/



	/**   ADD YOUR CODE HERE   **/
	//i askisi trexei kala gia I<=6 enw gia I=7 k panw  argei

		NsProblemManager  pm;
		NsIntVarArray Comb;

		 for(unsigned j = 0; j < P.size(); ++j){
			Comb.push_back(NsIntVar(pm, 0, 1));
		}

		NsDeque<NsIntVarArray> All;
		
		for(int i = 0; i <N; ++i){

			NsIntVarArray Sum;
			
			for(unsigned j = 0; j < P.size(); ++j){
				if(contains(P[j],i+1)==true){
					Sum.push_back(Comb[j]);
				}		

			}

			All.push_back(Sum);
		}

		for(unsigned i = 0; i < All.size(); ++i){
			pm.add(NsSum(All[i]) == 1);
		}

		NsIntVarArray CombCost;

		for(unsigned i = 0; i < Comb.size(); ++i){

			CombCost.push_back(NsIntVar(pm, 0, 1));
			pm.add(CombCost[i] == Comb[i]);
		}
		

		pm.addGoal( new NsgLabeling(Comb) );
		pm.addGoal( new NsgLabeling(CombCost) );


		NsIntVarArray SumCost;

		for(unsigned i = 0; i < CombCost.size(); i++){
			SumCost.push_back(CombCost[i]*C[i]);

		}
	
		NsIntVar Cost = NsSum(SumCost);
		pm.minimize(Cost);
		
		NsDeque< NsDeque<NsInt> > result;
		NsDeque< NsDeque<NsInt> > Cost2;

		//sto apotelesma i lysi me to mikrotero kostos ektypwnetai teleytaia
		 while ( pm.nextSolution() != false) {

			NsDeque<NsInt> temp;
			NsDeque<NsInt> tempCost;
			
			
			for(unsigned i = 0; i < Comb.size(); ++i){
				temp.push_back(Comb[i].value());
			}

			result.push_back(temp);

			for(unsigned i = 0; i < CombCost.size(); ++i){
				tempCost.push_back(CombCost[i].value()*C[i]);

			}


			Cost2.push_back(tempCost);
		}

			NsInt FCost = 0;
			unsigned element   = result.size()-1;
	
			cout<<"Pairings = [ ";
			for(unsigned i = 0; i < result[element].size(); ++i){
				if(result[element][i] == 1){
					cout<<"[ ";

					for(unsigned j = 0; j < P[i].size(); ++j){
						if(j == P[i].size()-1)
							cout<<P[i][j]<<" ";
						else
							cout<<P[i][j]<<", ";
					}cout<<" ]";
					cout<< "/ " << C[i]<<endl;
					FCost  = FCost + Cost2[element][i];

				}
				


			}cout<<"]"<<endl;
			cout<<"Cost = " << FCost<< endl; 	
		}




	cout << "\n";
      

      file.close();


    }  catch (exception& exc)  {

	cerr << exc.what() << "\n";

    }  catch (...)  {

	cerr << "Unknown exception" << "\n";
    }
}
