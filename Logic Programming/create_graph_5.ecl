:- lib(fd).


create_graph(NNodes, Density, Graph) :-
   cr_gr(1, 2, NNodes, Density, [], Graph).

cr_gr(NNodes, _, NNodes, _, Graph, Graph).
cr_gr(N1, N2, NNodes, Density, SoFarGraph, Graph) :-
   N1 < NNodes,
   N2 > NNodes,
   NN1 is N1 + 1,
   NN2 is NN1 + 1,
   cr_gr(NN1, NN2, NNodes, Density, SoFarGraph, Graph).
cr_gr(N1, N2, NNodes, Density, SoFarGraph, Graph) :-
   N1 < NNodes,
   N2 =< NNodes,
   rand(1, 100, Rand),
   (Rand =< Density ->
      append(SoFarGraph, [N1 - N2], NewSoFarGraph) ;
      NewSoFarGraph = SoFarGraph),
   NN2 is N2 + 1,
   cr_gr(N1, NN2, NNodes, Density, NewSoFarGraph, Graph).

rand(N1, N2, R) :-
   random(R1),
   R is R1 mod (N2 - N1 + 1) + N1.
%------------------------------------------------------
create_list(0,[]).
create_list(N,[_|TAIL]):-
	N=\=0,
	N2 is N-1,
	create_list(N2,TAIL).

%vriskei kai epistrefei to N-osto stoixeio mias listas
nth2(1,[Element|Tail],Tail,Element).

nth2(N,[_|Tail],T2,Element):-
      N > 1,
      N1 is N-1,
      nth2(N1,Tail,T2,Element).	

writelist([]):-nl.
writelist([X|L]):- write(X),writelist(L).
%------------------------------------------------------

%ayto to katigorima vriskei tous geitonikous komvous enos komvou N

find_neighbours(_,[],Neig,Neig).

%an sto stoixeio tou grafou G den periexetai o komvos N, proxwrame parakatw
find_neighbours(N,[Head|G],Neig,Neig2):-
	Head = X-Y,
	X =\= N, Y =\= N,
	find_neighbours(N,G,Neig,Neig2).
	
%an to stoixeio tou grafou G periexei to komvo N ws X-N
find_neighbours(N,[Head|G],Neig,Neig2):-
	Head = X-Y,
	Y =:= N, append(Neig,[X],NewNeig),
	find_neighbours(N,G,NewNeig,Neig2).

%an to stoixeio tou grafou G periexei to komvo N ws N-Y
find_neighbours(N,[Head|G],Neig,Neig2):-
	Head = X-Y,
	X =:= N, append(Neig,[Y],NewNeig),
	find_neighbours(N,G,NewNeig,Neig2).

%ayto to katigorima vriskei ton elaxisto arithmo xrwmatwn me ton
%opoio exoume xrwmatisei ton grafo

find_min([],_,N,N).

find_min([Head|L],Col_list,N,C):-
	member(Head,Col_list),!,
	find_min(L,Col_list,N,C).

find_min([Head|L],Col_list,N,C):-
	N1 is N+1,
	append(Col_list,[Head],New_col),
	find_min(L,New_col,N1,C).


%----------------------------------------------------------------------------------
color_graph(N,D,List,C):-
	create_graph(N,D,G),		%dimiourgia grafou
	create_list(N,List),		%dimiourgia listas gia ta xrwmata-komvous
	List::1..N,			%xrwmata
	constrain(List,List,G,1),
	generate(List),
	find_min(List,[],0,C).

%----------------------------------------------------------------------------------


%ayto to katigorima gia kathe mia koryfi tou grafou , me ti voitheia tou constrain_help ,
%periorizei ta domain twn koryfwn etsi wste oi geitonikes na min paroune idio xrwma
constrain([],_,_,_).

constrain([Head|List],Edges,G,N):-
	find_neighbours(N,G,[],Neig),
	constrain_help(Head,Edges,Neig),
	N1 is N+1,
	constrain(List,Edges,G,N1).

%ayto to katigorima dexetai mia lista apo geitones enos komvou{[F|Neig]) kai gia kathe
%enan apo tous geitones efarmozei ton periorismo na min exoune idio xrwma me ton trexonta komvo
constrain_help(_,_,[]).

constrain_help(Head,Edges,[F|Neig]):-
	nth2(F,Edges,_,Edge),
	Head ## Edge,
	constrain_help(Head,Edges,Neig).
	
generate([]).
generate([L|List]):-
	indomain(L),
	generate(List).