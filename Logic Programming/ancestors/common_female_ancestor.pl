parent(eva,bob).
parent(eva,pat).
parent(bob,pam).

parent(bob,ann).
parent(pat,joe).
parent(ann,jim).
parent(pam,sam).
parent(joe,gus).
parent(joe,leo).
parent(jim,kim).
parent(jim,ben).
parent(leo,liz).

male(bob).
male(joe).
male(jim).
male(sam).
male(gus).
male(leo).
male(ben).

female(eva).
female(pat).
female(ann).
female(pam).
female(kim).

sister(X,Y):-parent(Z,X),parent(Z,Y),female(X), X\=Y.
brother(X,Y):-parent(Z,X),parent(Z,Y),male(X), X\=Y.
uncle(X,Y):-parent(Z,Y),brother(X,Z).
ancestor(X,Y):-parent(X,Y).
ancestor(X,Y):-parent(X1,Y), ancestor(X,X1).
common_female_ancestor(X,Y,Z):-ancestor(Z,X),female(Z),ancestor(Z,Y).
