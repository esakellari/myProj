del(X,[X|L],L).
del(X,[A|L],[A|L1]):-
	del(X,L,L1).

stores([fnac, plaisio, multirama, e_shop]).
parts([scanner, pc, display, printer]).

sublist(S,L):- append(_,L2,L),append(S,_,L2).

buy([(_,_),(_,_),(_,_),(_,_)]).

rule1(List):- member((multirama,scanner),List).
rule2(List):- sublist([(fnac,_),(_,_),(_,display)],List).
rule3(List):- member(X,[multirama,plaisio,fnac]), sublist([(_,pc),(X,_)],List).
rule4(List):- append([(_,_),(plaisio,_)],[_|_],List).

generate([(S,P)|More],SList,PList):-
	del(S,SList,SRem),
	del(P,PList,PRem),
	generate(More,SRem,PRem).

generate([],[],[]).

test(List):-
rule1(List),rule3(List),rule2(List),rule4(List).

buycomp(List):-write(*),
	stores(SList),
	parts(PList),
	buy(List),
	test(List),
	generate(List,SList, PList).

