:-lib(fd).

job(j1, [t11,t12,t13,t14]).
job(j2, [t21,t22,t23,t24]).
job(j3, [t31,t32,t33]).
job(j4, [t41]).
%job(j5, [t51]).

task(t11, m1, 3, 3).
task(t12, m2, 2, 3).
task(t13, m1, 2, 3).
task(t14, m2, 3, 1).
task(t21, m1, 2, 2).
task(t22, m2, 3, 2).
task(t23, m1, 3, 1).
task(t24, m1, 4, 2).
task(t31, m1, 3, 1).
task(t32, m2, 1, 3).
task(t33, m2, 4, 3).
task(t41, m1, 1, 1).
%task(t42, m1, 3, 2).
/*task(t43, m1, 2, 3).
task(t51, m2, 3, 2).
*/
machine(m1, 2).
machine(m2, 2).

staff(10).



writelist([]):-nl.
writelist([X|L]):- write(X),writelist(L).


make_corr(S,C,S,C).
make_corr(Sum,C1, Count,Corr):-
		Count < Sum,
		Count2 is Count +1,
		append(C1,[Count],C2),
		make_corr(Sum, C2,Count2,Corr).

create_list(0,[]).
create_list(N,[_|TAIL]):-
	N=\=0,
	N2 is N-1,
	create_list(N2,TAIL).

fill_list([],_).
fill_list([X|L], N):- X = N, fill_list(L,N).

create_machine_list([],[],Machine_list, Machine_list).
create_machine_list([M|Mlist],[N|Nlist],Init,Machine_list):-
	create_list(N,L1),
	fill_list(L1,M),
	append(Init,L1,Init2),
	create_machine_list(Mlist,Nlist,Init2,Machine_list).

find_sum([],S,S).
find_sum([X|L],S,Sum):-
	Sum1 is S+X,
	find_sum(L,Sum1,Sum).
%----------------------------------------------------------------------------------

%genika ayto to programma trexei sxedon amesws gia dedomena mexri t41. Mexri kai 
%to t41 xreiazetai 4.38s enw an valoume kai to t42 mesa xreiazetai 51.11s.



job_shop_opt(Schedule):-

	findall(X, task(_,_,X,_),Durs), 	%sygentrwnw se mia lista tis duration twn tasks
	findall(X, task(X,_,_,_), Tasks),
			
	find_sum(Durs, 0,Deadline), 		%vriskw to deadline
	
	findall(X, machine(X,_),Mlist),  	%Mlist = [m1,m2,..]
	
	findall(Z, machine(_,Z), Nlist), 	%vriskw poses einai oi 	mixanes ston arithmo 
	find_sum(Nlist,0,Sum),
	
	findall(X, task(_,X,_,_),Machine_task),  %lista pou krataei gia kathe task se poia mixani ekteleitai
	make_corr(Sum,[],0, Corr),

	findall(X, job( _, X), TasksLists),
	
	findall(X, task(_,_,_,X), Staff),
	
	
	create_machine_list(Mlist,Nlist,[],Allmachines),	%Allmachines = [m1,m1,m2,m2,..]

	length(Tasks,L),				%Tasks = [t11,t12,t13,t14,t21,..]

	length(Start, L),				%Start:lista megethous oses oi tasks, pou krataei tis stigmes enarksis kathe task
	Start::[0..Deadline],
	
	length(End, L),
	End::[0..Deadline],			%End:lista megethous oses oi tasks, pou krataei tis stigmes pou teleiwnoun oi task

	length(Machine_list, L),
	Machine_list::[0..Sum],
	
	staff(MaxStaff),

	write(Allmachines),nl,
	constraints(Start, End, Machine_list,Durs,Deadline, 
				Machine_task,Allmachines, Corr,TasksLists,Tasks,
				Staff),
										 

	labeling(Start),
	labeling(End),
	labeling(Machine_list),
	check_staff(Staff, MaxStaff, Start, End, 0, Deadline),

	%kalw to katigorima pou tha dwsei tin teliki morfi sto apotelesma
	morph(Allmachines, Start, End,Tasks,Machine_task, [], Schedule).
	
%------------------------------------------------------------------------------



morph([],_,_,_,_,F,F).
morph([M|Allmachines],Start, End,Tasks,Machine_task,Allfinal,FF):-
	
	%ftiaxnw mia lista analoga me ti mixani, m1,m2.. tis morfis 
	%execs(m1, [t(t11, 0, 3), t(t13, 5, 7), t(t21,8,9)]) klp kai tin prosthetw
	%stin teliki lista

	help_morph(M,Start, End, Machine_task,Tasks, Final, []),
	
	append(Allfinal, [execs(M, Final)],All2),

	morph(Allmachines, Start, End, Tasks, Machine_task,All2, FF).






help_morph(_,[],[],[],[],F,F).
help_morph(M,[S|Start],[E|End], [MT|Machine_task],[T|Tasks],Final, F):-
	M\==MT,
	help_morph(M,Start,End, Machine_task,Tasks,Final, F).

help_morph(M,[S|Start],[E|End], [MT|Machine_task],[T|Tasks],Final, F):-
		
		%an i task T ginetai sti mixani M, tin prosthetw sti lista
		M== MT,
		append(F, [t(T,S,E)],F2),
		help_morph(M,Start, End, Machine_task, Tasks,Final, F2).


check_staff(Staff, MaxStaff, Startlist, Endlist, Time, Deadline):-
	Time < Deadline,!,	%gia time = 0 , 1 , 2 , 3.. deadline
	
	staff_check_help(Staff, Startlist, Endlist, Time,0,SumStaff),

	SumStaff =< MaxStaff,
	
	Time2 is Time+1,
	check_staff(Staff,MaxStaff,Startlist,Endlist,Time2,Deadline).
	

check_staff(_,_,_,_,Time, Deadline):-
	Time >= Deadline.
	

staff_check_help(_,[],[],_,Sum,Sum).

staff_check_help([StH|StRest],[HS|HSREST], [HE|HEREST], Count, CurSumStaff, SumStaff):-
	
	% HS    = start ths current task
	% HE    = end   ths current task
	% StH   = staff ths current task
	% Count deixnei to time pou briskomaste
	

	Count >= HS,Count < HE ,!,

	CurSumStaff2 is CurSumStaff + StH,
	staff_check_help(StRest, HSREST, HEREST, Count, CurSumStaff2, SumStaff).

staff_check_help([StH|StRest],[HS|HSREST], [HE|HEREST], Count, CurSumStaff, SumStaff):-
	staff_check_help(StRest, HSREST, HEREST, Count, CurSumStaff, SumStaff).
	
	
	
find_corr(_,[],C,C,[]).	
find_corr(H,[H2|Allmachines], Corlist,C,[H3|Corr]):-
	H \== H2,
	find_corr(H,Allmachines, Corlist,C,Corr).

find_corr(H,[H2|Allmachines], Corlist,C,[H3|Corr]):-	
	H == H2,
	
	append(Corlist,[H3],Corlist2),
	find_corr(H,Allmachines, Corlist2, C,Corr).


%me ayto to katigorima allazw ta domain twn tasks gia tis mixanes
%analoga me ton typo tis mixanis stin opoia ektelountai
% an px i t11 ekteleitai se mixani m1,kai exoume 2 mixanes
%typou m1,  tote to domain tis ginetai[0..1], apo [0..Sum]
 
repair_mach_domains([],[],_,_).
repair_mach_domains([H|M1],[H2|M2],Allmachines, Corr):-
		find_corr(H2,Allmachines,[],Corr_to_task, Corr),
		
		list_to_dom(Corr_to_task,Dom),
		dvar_replace(H, Dom),

		repair_mach_domains(M1,M2,Allmachines, Corr).



constraints(StartList, EndList, Machine_list, Durs,Deadline,Machine_task,Allmachines,Corr,TasksLists,Tasks,Staff):-
	repair_mach_domains(Machine_list,Machine_task,Allmachines,Corr),
	main_constraint(StartList, EndList,Machine_list,Durs,Deadline),
	priority_constraint(StartList, EndList,TasksLists,Tasks).
	


%ayto to katigorima vazei ton periorismo gia tin proteraiothta,
%diladi oti i t11 prepei na ginei prin apo tin t12 k.o.k.

priority_constraint(_,_,[],_).
priority_constraint(Start,End,[TH|T1],Tasks):-
	help_priority(Start, End, TH, Tasks),
	priority_constraint(Start,End,T1,Tasks).

help_priority(_,_,[],_).
help_priority(Start,End,[J|J1],Tasks):-
		help_prior2(J, J1,Start, End,Tasks),
		help_priority(Start,End, J1,Tasks).

help_prior2(_,[],_,_,_).
help_prior2(J,[J2|Rest],Start,End,Tasks):-

	find_element(End,J,Tasks,Jend),
	find_element(Start, J2,Tasks, J2Start),

	Jend #<= J2Start.


find_element([Element|_],J,[J|_],Element).
find_element([_|Rest],J,[J1|RestTasks],Element):-
		J \== J1,
		find_element(Rest,J,RestTasks,Element).

main_constraint([],[],[],[],_).
main_constraint([H|Start],[H2|End],[H3|Mlist],[H4|Durs],Deadline):-
	
	%Start < Deadline
	H #<=Deadline-H4,
	
	%Start+Duration = End
	H + H4 #= H2,
	
	with_other_machines(H,Start,H2,End, H3, Mlist),
	main_constraint(Start,End,Mlist,Durs, Deadline).

with_other_machines(_,[],_,[],_,[]).
with_other_machines(H,[NH|Start],H2,[NH2|End],H3,[NH3|Mlist]):-

	%an mia ergasia arxizei kai teleiwnei tin idia stimgi me 
	%tin twrini, prepei opwsdhpote na ginei se alli mixani

	H #= NH #/\ H2 #= NH2 #=> H3 ## NH3,
	
	%an mia ergasia ginetai stin idia mixani me tn twrini,
	%prepei eite na teleiwnei prin arxisei i twrini,
	% i na arxizei afou teleiwsei i twrini

	H3 #= NH3 #=> NH2 #<= H #\/ NH #>= H2,
	
	with_other_machines(H,Start, H2, End, H3, Mlist).