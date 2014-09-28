#include <naxos.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "strtok.h"

using namespace std;
using namespace naxos;



struct  task_t  {
	NsInt  machine_type;
	NsInt  duration;
	NsInt  staff;
};

NsInt find_sum(NsDeque<NsInt> Deque){

	NsInt Sum = 0;

	for(unsigned i = 0; i < Deque.size(); ++i){

		Sum = Sum + Deque[i];

	}	
	return Sum;

}


int main (int argc, char *argv[])
{
    try  {

	//  Checking input arguments.
	if ( argc  !=  2 )   {
		cerr << "Correct syntax is:\t" << argv[0]<< " <input_filename>\n";
		return  1;
	}

	//  Opening input filename, e.g. `jobshop_opt_data.pl'.
	ifstream  file(argv[1]);
	if ( ! file )   {
		cerr << "Could not open `" << argv[1] << "'\n";
		return  1;
	}

	NsDeque< NsDeque<task_t> >  TasksOfJob;
	NsDeque<NsInt>              MachinesPerType;
	NsInt                       Staff=0;

	unsigned  job=0, task=0;
	string    input, tok;

	//  Reading input file line by line.

	while ( getline(file,input) )    {

		StrTokenizer  tokens(input, "([, ]).");

		tok  =  tokens.next();

		if ( tok  ==  "job" )    {

			TasksOfJob.push_back( NsDeque<task_t>() );
			tokens.next();	// Ignore a token.

			while ( ! (tok = tokens.next()).empty() )
				TasksOfJob.back().push_back( task_t() );

		}  else if ( tok  ==  "task" )    {

			tokens.next();	// Ignore a token.
			TasksOfJob[job][task].machine_type =
				atoi(tokens.next().substr(1).c_str()) - 1;
			TasksOfJob[job][task].duration =
				atoi(tokens.next().c_str());
			TasksOfJob[job][task].staff =
				atoi(tokens.next().c_str());

			++task;
			if ( task  ==  TasksOfJob[job].size() )   {
				task = 0;
				++job;
			}

		}  else if ( tok  ==  "machine" )    {

			tokens.next();	// Ignore a token.
			MachinesPerType.push_back( atoi(tokens.next().c_str()) );

		}  else if ( tok  ==  "staff" )    {

			Staff  =  atoi(tokens.next().c_str());

		}  else if ( tok  !=  "" )    {

			cerr << "Unknown predicate `" << tok << "'\n";
			return  1;
		}
	}

	file.close();

	cout<<TasksOfJob.size()<<endl;

	//  Print problem representation.
	for (job=0;  job < TasksOfJob.size();  ++job)   {
		for (task=0;  task < TasksOfJob[job].size();  ++task)   {
			cout << "TasksOfJob[" << job << "][" << task
				<< "].machine_type = "
				<< TasksOfJob[job][task].machine_type << "\n";
			cout << "TasksOfJob[" << job << "][" << task
				<< "].duration = "
				<< TasksOfJob[job][task].duration << "\n";
			cout << "TasksOfJob[" << job << "][" << task
				<< "].staff = "
				<< TasksOfJob[job][task].staff << "\n";
		}
		cout << "\n";
	}
	for (unsigned mpt=0;  mpt < MachinesPerType.size();  ++mpt)   {
		cout << "MachinesPerType[" << mpt << "] = "
			<< MachinesPerType[mpt] << "\n";
	}
	cout << "\nStaff = " << Staff << "\n";



	/**   ADD YOUR CODE HERE   **/
	
	//logw tis polyplokotitas tis emfanisis tou apotelesmatos stin ekfwnisi tis askisis 7,
	//se syndyasmo me tin piesi xronou, exw kanei mia pio apli ektypwsi tis morfis 
	//t11 : start: 0 end: 3 in machine 1


	NsProblemManager pm;

	NsDeque<NsDeque<NsInt> > Allmachines;
	int counter = 0;


	//oi mixanes xwrizontai analoga me ton typo toys me arithmisi i opoia ksekinaei
	//apo to 1. kathe mixani exei k ena noumero apo to 1 mexri to synoliko arithmo
	//twn mixanwn olwn twn typwn. Diladi stin periptwsi pou exoume 2 mixanes typou 0
	//aytes oi dyo mixanes exoun noumera 1 k 2 antistoixa k.o.k.
	for (unsigned mpt=0;  mpt < MachinesPerType.size();  ++mpt)   {
		
		NsDeque<NsInt> temp;
		for(int i = 0; i < MachinesPerType[mpt]; ++i){
			temp.push_back(counter);
			counter++;
		}
		cout<<endl;
		Allmachines.push_back(temp);
	}

	NsIntVarArray End, Start,Machine_list,Staff_list;
	NsDeque<NsInt> Durs,Machine_task,Staff_task;
	NsInt Machine_sum=0;

	for (job=0;  job < TasksOfJob.size();  ++job)   {
		 for (task=0;  task < TasksOfJob[job].size();  ++task)   {
			Durs.push_back( TasksOfJob[job][task].duration);
			Staff_task.push_back(TasksOfJob[job][task].staff);
		}
	}

	for (unsigned mpt=0;  mpt < MachinesPerType.size();  ++mpt)   
		Machine_sum = Machine_sum + MachinesPerType[mpt];
	
	NsInt Deadline;

	Deadline = find_sum(Durs);
	
	for (job=0;  job < TasksOfJob.size();  ++job)   {
		 for (task=0;  task < TasksOfJob[job].size();  ++task)   {
		
			Start.push_back(NsIntVar(pm, 0, Deadline));
			End.push_back(NsIntVar(pm, 0, Deadline));
			Staff_list.push_back(NsIntVar(pm, 0, Staff));	
		}
	}

	NsDeque<NsInt> temp;
	for (job=0;  job < TasksOfJob.size();  ++job)   {
		for (task=0;  task < TasksOfJob[job].size();  ++task)   {
		
			unsigned Size = Allmachines[TasksOfJob[job][task].machine_type].size();
			temp = Allmachines[TasksOfJob[job][task].machine_type];

			Machine_list.push_back(NsIntVar(pm, temp[0], temp[Size-1])); 	
		}
	}

	for(unsigned i = 0; i < Start.size(); ++i){
		
		//Start<Deadline
		pm.add( Start[i]<= Deadline-Durs[i] );
		//Start + dus = end
		pm.add( End[i] == Start[i]+Durs[i]);
	
		for(unsigned j = 0; j < Start.size(); ++j){
		
			if(j!=i){
				//an mia ergasia arxizei kai teleiwnei tin idia stimgi me 
				//tin twrini, prepei opwsdhpote na ginei se alli mixani
				pm.add(NsIfThen( Start[i] == Start[j] && End[i] == End[j] , Machine_list[j]!=Machine_list[i] ));


				//an mia ergasia ginetai stin idia mixani me tn twrini,
				//prepei eite na teleiwnei prin arxisei i twrini,
				//i na arxizei afou teleiwsei i twrini
			
				pm.add(NsIfThen(Machine_list[i] == Machine_list[j], (End[j] <= Start[i] || Start[j] >= End[i])));
			}
		} 

	}


	for (job=0;  job < TasksOfJob.size();  ++job)   {
		
		unsigned count = 0;
		for (task=0;  task < TasksOfJob[job].size();  ++task)   {
			
			if(count<Start.size()-1){
				pm.add(End[count] <= Start[count+1]);
				count++;
			}					
		}
	}

	pm.addGoal(new NsgLabeling(Start));
	pm.addGoal(new NsgLabeling(End));
	pm.addGoal(new NsgLabeling(Machine_list));
	
	pm.minimize(NsSum(End));
	pm.minimize(-NsSum(Machine_list));

	NsDeque< NsDeque<NsInt> > ResStart,Resend, ResM;
	 
	while (pm.nextSolution() != false){

		NsDeque<NsInt> tempS,tempE,tempM;
		for(unsigned i = 0; i < Start.size() ; ++i){

				tempS.push_back(Start[i].value());
				tempE.push_back(End[i].value());
				tempM.push_back(Machine_list[i].value());
		}
		ResStart.push_back(tempS);
		Resend.push_back(tempE);	
		ResM.push_back(tempM);				
	}

	unsigned element = ResStart.size()-1;
	int count =0;
	char buffer [50];

	for (job=0;  job < TasksOfJob.size();  ++job)   {
		for (task=0;  task < TasksOfJob[job].size();  ++task)   {
			sprintf (buffer, "t%d%d", job+1,task+1);
			cout<<buffer<<" : start: "<< ResStart[element][count]<<" end: " << Resend[element][count];
			cout <<" in machine " << ResM[element][count]<<endl;
			++count;
		}
	}	

    }  catch (exception& exc)  {
		cerr << exc.what() << "\n";

    }  catch (...)  {
	cerr << "Unknown exception" << "\n";
    }
}
