element(E,S) :- member(E,S).

deletex(E, [E|S], S).
deletex(E, [H|S], [H|NewSet]) :- deletex(E, S, NewSet).

p([], []).

p(PermutationX,[H|T]) :- p(T, NewList), deletex(H, PermutationX, NewList).

equivalent(F,S) :- permutation(F, S).

intersectx([], _, []).
intersectx([_|List1tail], List2, List3) :- intersectx(List1tail, List2, List3).
intersectx([H|List1tail], List2, List3) :- member(H, List2),!, List3 = [H|List3tail], intersectx(List1tail, List2, List3tail).

intersect(List,List2,List3):-  equivalent(List3,List4), intersectx(List,List2,List4), !.

unionx([], List, List).
unionx([H|List1tail], List2, [H|List3tail]) :- unionx(List1tail, List2, List3tail).
unionx([H|List1tail], List2, List3) :- member(H, List2), union(List1tail, List2, List3), !.

unionx(List1,List2,List3):- equivalent(List3,List4), unionx(List1,List2,List4), !.
