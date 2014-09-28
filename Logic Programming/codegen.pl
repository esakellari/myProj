del(X,[X|L],L).
add(X, L, [X|L]).
append([],L,L).
append([X|L1],L2,[X|L3]):-
	append(L1,L2,L3).

writelist([]):-nl.
writelist([X|L]):- write(X),writelist(L).

lengh([],0).
lengh([_|T],N):-
	length(T,N1),
	N is 1+N1.

/*otan kanw move to teleytaio stoixeio tis listas*/
legal_move(N, Lstart, Lfinish):-
	lengh(Lstart,L),
	N=:=L,
        Element \== *,
	nth2(N,Lstart,_,Element),
	change(Element, Lstart,Lfinish).

/*otan kanw move opoiodipote allo stoixeio tis listas*/
legal_move(N, Lstart, Lfinish):-
	nth2(N,Lstart,L3,Element),
	Element \== *,
	change(Element, L3,L2),
	nth5(Lstart,N,L4),
	append(L4,[Element],L8),append(L8,L2,Lfinish).

/*enallagi dyo stoixeiwn swap(1,3).*/
swap1(I,J,Lstart,Lfinish):-
	nth2(I,Lstart,L2,Element1),nth2(J,Lstart,L3,Element2),
	Element1 \== *, Element2 \== *,
	N is J-I,
	nth5(Lstart,I,L4),nth5(L2, N,L5),
	append(L4,[Element2|L5],L8),append(L8,[Element1|L3],Lfinish).

nth5(_,1,[]).
nth5([X|Rest],N,[X|Tail]):-
	N2 is N-1,
	nth5(Rest,N2,Tail).

/*vriskei an kathe stoixeio mias listas([Y|L1]) vrisketai mesa se mia alli (L)*/
find_element(_,[]).

find_element(L,[Y|L1]):-
	Y = *,
	find_element(L,L1).


find_element(L,[Y|L1]):-
	Y \== *,
	member(Y,L),
	find_element(L,L1).

/*vriskei kai epistrefei to N-osto stoixeio mias listas*/
nth2(1,[Element|Tail],Tail,Element).

nth2(N,[_|Tail],T2,Element):-
      N > 1,
      N1 is N-1,
      nth2(N1,Tail,T2,Element).

/*allazei to prwto stoixeio mias listas se Element*/
change(Element,[X|L1],[Y|L2]):-
	del(X, [X|L1],Tail),
	add(Element, Tail, [Y|L2]).


/*elegxei an dyo listes einai idies*/
are_equal([X1|L1],[X2|L2]):-
	lengh([X1|L1],N1), lengh([X2|L2],N2),
	N2 =:= N1,
	eq([X1|L1],[X2|L2]).

eq([],[]).

eq([_|L1],[X2|L2]):-
	X2 = *,
	eq(L1,L2).

eq([X1|L1],[X2|L2]):-
	X2 \== *,
	X1=X2,
	eq(L1,L2).

/*epistrefei true otan i lista listwn [X|L] den periexei ti lista L1 */
dnot_contains([],_).
dnot_contains([X|L], L1):-
	are_not_equal(X,L1),
	dnot_contains(L,L1).

/*epistrefei true otan dyo listes den einai ises*/
are_not_equal([X1|_],[X2|_]):-
	X1\=X2.

are_not_equal([X1|L1],[X2|L2]):-
	X1=X2,
	are_not_equal(L1,L2).

/*epanaliptiki emvathynsi */
id(StartList,FinalList,L3,Ret,ListOfStates,N):-
	find_element(StartList,FinalList),
	nextmove(StartList,FinalList,L3,Ret,ListOfStates,N,0),!.

id(StartList,FinalList,L3,Ret,ListOfStates,N) :-
   N1 is N+1,
   id(StartList,FinalList,L3,Ret,ListOfStates, N1).

/*elegxw an oi dyo listes pou dothikan ws orisma einai ises , an dhladh
ftasame stin teliki lista*/
nextmove(CurList,FinalList,_,[],_,N, Counter):-
       	are_equal(CurList,FinalList),Counter  =< N.

nextmove(CurList,FinalList,L,[U|L2],ListOfStates, N, Counter):-
	member(N1,L),
	legal_move(N1, CurList,NextList),         /*kanw move ena stoixeio*/
	find_element(NextList,FinalList),         /*elexgw an kapoio stoixeio stis FinalList den yparxei stin nextlist*/
	dnot_contains(ListOfStates,NextList),     /*elexgw an exw ksanaperasei apo ayti tin katastasi*/
	append(ListOfStates, [NextList],NewList),
	Counter =< N,Counter2 is Counter +1,      /*elexgw to vathos tis epanaliptikis emvathynsis*/
	nextmove(NextList,FinalList,L,L2,NewList,N,Counter2),
	U = move(N1).


nextmove(CurList,FinalList,L,[U|L2],ListOfStates, N, Counter):-
	member(N1,L),member(M,L),
	N1 \= M, N1 < M,
	swap1(N1,M,CurList,NextList),               /*kanw swap dyo stoixeia*/
	find_element(NextList,FinalList),           /*elexgw an kapoio stoixeio stis FinalList den yparxei stin nextlist*/
	dnot_contains(ListOfStates,NextList),	    /*elexgw an exw ksanaperasei apo ayti tin katastasi*/
	append(ListOfStates, [NextList],NewList),
        Counter < N,Counter2 is Counter +1,          /*elexgw to vathos tis epanaliptikis emvathynsis*/
	nextmove(NextList,FinalList,L,L2,NewList,N, Counter2),
	U = swap(N1,M).


create_list(0,_,[]).
create_list(N,C,[X|Tail]):-
	N2 is N-1,
	N2 >= 0,
	C2 is C+1,
	create_list(N2,C2,Tail),
	X = C.


codegen(StartList,FinalList,Ret):-
	lengh(StartList, N),
	create_list(N,1,L3),         /*dimiourgw mia lista analoga me to megethos tis dotheisas listas,
	                              i opoia einai tis morfis an px N= 3 L3= [1,2,3].*/
	id(StartList,FinalList,L3,Ret,[],0).


































