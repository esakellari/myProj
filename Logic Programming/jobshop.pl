del(X,[X|L],L).
add(X, L, [X|L]).


set_flag(print_depth, 1000).

create_list(0,[]).
create_list(N,[_|TAIL]):-
	N=\=0,
	N2 is N-1,
	create_list(N2,TAIL).

reverse([],[]).
reverse([First|Rest],Reversed):-
	reverse(Rest,ReversedRest),
	append(ReversedRest,[First],Reversed).

lengh([],0).
lengh([_|T],N):-
	length(T,N1),
	N is 1+N1.

sub(S,L):-
	append(L1,L2,L),
	append(S,L3,L2).

/*vriskei kai epistrefei to N-osto stoixeio mias listas*/
nth2(1,[Element|Tail],Tail,Element).

nth2(N,[_|Tail],T2,Element):-
      N > 1,
      N1 is N-1,
      nth2(N1,Tail,T2,Element).

nth5(_,1,[]).
nth5([X|Rest],N,[X|Tail]):-
	N2 is N-1,
	nth5(Rest,N2,Tail).


/*epistrefei se poies thesei se mia lista vrisketai to N stoixeio*/

find_pos([],_,_,Pos,Pos).
find_pos([X|L],N,Counter,Pos,Position):-
	var( X ),!,
	Count2 is Counter+1,
	find_pos(L,N,Count2,Pos,Position).


find_pos([Y|L],X,Counter,Pos,Position):-
	Y = X,!,
	append(Pos,[Counter],Pos2),
	Count2 is Counter+1,
	find_pos(L,X,Count2,Pos2,Position).

find_pos([X|L],N,Counter,Pos,Position):-
	X\==N,!,
	Count2 is Counter+1,
	find_pos(L,N,Count2,Pos,Position).

find_sum([],S,S).
find_sum([X|L],S,Sum):-
	Sum1 is S+X,
	find_sum(L,Sum1,Sum).

machine_list(L,L,0).
machine_list(L,Ml,N):-
	N=\=0,
	deadline(D),create_list(D,L1),
	append(L,[L1],L2),N1 is N-1,
	machine_list(L2,Ml,N1).

fill_list([],_).
fill_list([X|L], N):- X = N, fill_list(L,N).


/*vriskei se poia thesi tis listas emfanizetai gia teleytaia fora
to Element, px L=[t11,t11,t11,...]=> LastAp */

last_appear(Element, L,LastAp):-reverse(L,Rev),
	find_pos(Rev,Element,0,[],[FirstAp|_]),
	lengh(L,Length),
	LastAp is Length - FirstAp.


prev_task(Task,PrevTask):-name(Task,L),
	nth2(2,L,_,El1),nth2(3,L,_,El2),
	Y1 is El2 -1,
	name(PrevTask, [116,El1,Y1])
	.



search(_,[],_).


search(Ml,[Head|Tail],L2):- task(Head,Mach,Dur),
	name(Head,L),nth2(3,L,_,El2),          /*an  einai tis morfis tN1*/
	name(Y,[El2]),
	 Y =:= 1,

	create_list(Dur,L3),                   /*ftiaxnw ti lista tis morfis [t11,t11,..]*/
	fill_list(L3,Head),
	find_pos(L2,Mach,1,[],Positions),      /*vriskw ti thesi stin opoia vrisketai i mixani pou
	                                         thelw sti lista me tis mixanes*/
	member(N,Positions),
	nth2( N,Ml,_,Machlist),/*write(Head),nl,write(Machlist),*/
	sub(L3, Machlist),         /*topothetw ti lista [t11,t11,..]sti lista tis mixanis*/

	search(Ml,Tail,L2).


search(Ml,[Head|Tail],Machine_list):- task(Head,Mach,Dur),
/*	task(Head,Mach,Dur),
	name(Head,L),nth2(3,L,_,El2),          /*an  einai tis morfis tN1*/
	name(Y,[El2]),
	 Y =\= 1,*/

	create_list(Dur,L3),                   /*ftiaxnw ti lista tis morfis [t11,t11,..]*/
	fill_list(L3,Head),
	find_pos(Machine_list,Mach,1,[],Positions),      /*vriskw ti thesi stin opoia vrisketai i mixani pou
	                                         thelw sti lista me tis mixanes*/
	member(N,Positions),
	nth2( N,Ml,_,Machlist),
	prev_task(Head, Prev),	          /*vriskw poia ergasia prohgeitai tis twrinis*/
	task(Prev, PrevMach,_),               /*vriskw se poia mixani ekteleitai ayti*/
	find_pos(Machine_list, PrevMach, 1,[],PrevPos),

	member(N2,PrevPos),
	nth2(N2,Ml,_,PrevMachlist),


	last_appear(Prev, PrevMachlist,LastAp),
	sub(L3, Machlist),
	find_pos(Machlist, Head,1,[],[St|_]),
	St > LastAp,

	search(Ml,Tail,Machine_list).


create_machine_list([],[],Machine_list, Machine_list).
create_machine_list([M|Mlist],[N|Nlist],Init,Machine_list):-
	create_list(N,L1),
	fill_list(L1,M),
	append(Init,L1,Init2),
	create_machine_list(Mlist,Nlist,Init2,Machine_list).


search_with_manpower(_,_,[],_).


search_with_manpower(Ml,Sl,[Head|Tail],Machine_list):-

	task(Head,Mach,Dur,Nofw),

	name(Head,L),nth2(3,L,_,El2),          /*an  einai tis morfis tN1*/
	name(Y,[El2]),
        Y =:= 1,

	find_pos(Machine_list,Mach,1,[],Positions) ,  /*vriskw ti thesi stin opoia vrisketai i mixani pou
	                                                   thelw sti lista me tis mixanes*/
	member(N,Positions),
	nth2( N,Ml,_,Machlist),
	nth2(N,Sl,_,Stafflist),                         /*pairnw ti lista tis mixanis pou thelw gia to staff*/

	create_list(Dur,L3),                     /*ftiaxnw ti lista tis morfis [t11,t11,..]*/
	fill_list(L3,Head),

	sub(L3, Machlist),                      /*topothetw ti lista [t11,t11,..]sti lista tis mixanis*/

	find_pos(Machlist,Head,1,[],Positions2),
	make_prop(Positions2,Stafflist,Nofw),  /*ftiaxnw ti lista tis morfis [3,3,...] opou 3 = arithmos ergatwn*/

	testworkers(Sl,0,[]),                  /*elexgw an o arithmos twn ergatwn kathe mia stigmi den ksepernaei to orio*/


	search_with_manpower(Ml,Sl,Tail,Machine_list).

search_with_manpower(Ml,Sl,[Head|Tail],Machine_list):-

	task(Head,Mach,Dur,Nofw),

	create_list(Dur,L3),                            /*ftiaxnw ti lista tis morfis [t11,t11,..]*/
	fill_list(L3,Head),

	find_pos(Machine_list,Mach,1,[],Positions),      /*vriskw ti thesi stin opoia vrisketai i mixani pou
	                                                     thelw sti lista me tis mixanes*/
	member(N,Positions),
	nth2( N,Ml,_,Machlist),
	nth2(N,Sl,_,Stafflist),                           /*pairnw ti lista tis mixanis pou thelw gia to staff*/

	prev_task(Head, Prev),	                      /*vriskw poia ergasia prohgeitai tis twrinis*/
	task(Prev, PrevMach,_),                         /*vriskw se poia mixani ekteleitai ayti*/
	find_pos(Machine_list, PrevMach, 1,[],PrevPos),

	member(N2,PrevPos),
	nth2(N2,Ml,_,PrevMachlist),


	last_appear(Prev, PrevMachlist,LastAp),  /*vriskw ti stigmi stin opoia teleiwnei  i proigoumeni task*/
	sub(L3, Machlist),                      /*topothetw ti lista [t11,t11,...] sti lista tis mixanis*/
	find_pos(Machlist,Head,1,[],Positions2),

	make_prop(Positions2,Stafflist,Nofw),    /*ftiaxnw ti lista tis morfis [3,3,...] opou 3 = arithmos ergatwn*/

	find_pos(Machlist, Head,1,[],[St|_]),   /*vriskw ti xroniki stigmi apo tin opoia arxizei ti twrini task*/
	St > LastAp,
	testworkers(Sl,0,[]),                  /*elexgw an o arithmos twn ergatwn kathe mia stigmi den ksepernaei to orio*/

	search_with_manpower(Ml,Sl,Tail,Machine_list).



/*allazei to prwto stoixeio mias listas se Element*/
change2(Element,[X|L1],[Y|L2]):-
	del(X, [X|L1],Tail),
	add(Element, Tail, [Y|L2]).


/*ayto to katigorima topothetei stin lista Stafflist tis mixanis
stis theseis opou vrisketai kapoia task, ton antistoixo arithmo
ergatwn gia ayti ti task*/

make_prop([],_,_).

make_prop([N|R],Stafflist,Dur):-
	N=:=1,
	nth2(N,Stafflist, Rest,_),
	add(Dur,Rest,Stafflist),
	make_prop(R,Stafflist,Dur).



make_prop([N|R],Stafflist,Dur):-
	N1 is N -1,
	nth2(N1,Stafflist, Rest,_),

	nth5(Stafflist, N, Rest2),

	change2(Dur, Rest,Res),
	append(Rest2,Res,Stafflist),
	make_prop(R,Stafflist,Dur).


/*ayto to katigorima pairnei stin lista listwn Sl i opoia periexei tis listes
pou deixnoun posa atoma douleyoun se kathe mixani kathe mia xroniki stigmi,
kai epistrefei false an estw mia xroniki stigmi to athroisma einai megalytero
apo ton arithmo twn ergatwn staff(N)*/
testworkers([[]|_],_,_).


testworkers([],Sum,Rem):-
	staff(N),
	Sum =<N,
	testworkers(Rem, 0,[]).


testworkers([[X|R]|L],Sum,Rem):-
	var(X) ,!,
	append(Rem,[R],Rem2),
	testworkers(L,Sum, Rem2).


testworkers([[X|R]|L],Sum,Rem):-
	append(Rem, [R],Rem2),
	Sum2 is Sum + X,
	testworkers(L,Sum2, Rem2).

make_f2(_,[],_,S,S).

make_f2(List,[X|L],Y,S1,S):-
	var(X),!,
	make_f2(List,L,Y,S1,S).

make_f2(List,[X|L],X2,S1,S):-
	X2 \== X,
	find_pos(List,X,1,[],[Y|_]),
	last_appear(X,List,LastAp),
	Y1 is Y -1,
	append(S1,[t(X,Y1,LastAp)],S3),
	make_f2(List,L,X,S3,S).

make_f2(List,[X|L],X2,S1,S):-
	X == X2,
	make_f2(List,L,X,S1,S).

/*ayto to katigorima dimiourgei tis listes gia kathe mixani stin teliki tous morfi
execs(m1, [t(t11, 0, 2), t(t41, 2, 7), t(t22, 7, 10)]) me ti voitheia tou make_f2*/

final_list([],_,[],S,S).
final_list([Head|Tail],TasksList,[H2|Ml],S1,Sol):-
	make_f2(Head,Head,_,[],S),
	append(S1,[execs(H2,S)],S2),
	final_list(Tail,TasksList,Ml,S2,Sol).




job_shop_with_manpower(S):-
	ensure_loaded('input.pl'),
	findall(X, machine(X,_),Mlist),      /*ftiaxnw ti lista me tis mixanes*/
	findall(Y, task(Y,_,_),TasksList),           /*lista me oles tis task*/
	findall(Z, machine(_,Z), Nlist),
	find_sum(Nlist,0,Sum),
	create_machine_list(Mlist, Nlist,[],Machine_list),
	machine_list([],Ml,Sum),	       /*ftiaxnw apo mia lista gia kathe ksexwristi mixani megethous deadline*/
	machine_list([],Sl,Sum),	     /*ftiaxnw apo mia lista gia kathe mixani pou tha krataei posa atoma douleyoun*/
	search_with_manpower(Ml,Sl,TasksList,Machine_list),
	final_list(Ml,TasksList,Machine_list,[],S).

job_shop(S):-
	ensure_loaded('input.pl'),
	findall(X, machine(X,_),Mlist),     /*ftiaxnw ti lista me tis mixanes*/
	findall(Y, task(Y,_,_),TasksList),           /*lista me oles tis task*/
	findall(Z, machine(_,Z), Nlist),
	find_sum(Nlist,0,Sum),
	create_machine_list(Mlist, Nlist,[],Machine_list),
	machine_list([],Ml,Sum),	      /*ftiaxnw apo mia lista gia kathe ksexwristi mixani megethous deadline*/
	search(Ml,TasksList,Machine_list),
	final_list(Ml,TasksList,Machine_list,[],S).

































































