% splits list into 2 sections and tries possible solutions
section([X],X).
section(ItemList,Sec) :-
   split(ItemList,LeftSec,RightSec),
   section(LeftSec,LeftX),
   section(RightSec,RightX),
   sectT(LeftX,RightX,Sec).

% seperates list
seperate(ItemList,LeftX,RightX) :-
   split(ItemList,LeftB,RightB),
   section(LeftB,LeftX),
   section(RightB,RightX),
   LeftX =:= RightX.

% tries possiblities
sectT(LeftX,RightX,LeftX+RightX).
sectT(LeftX,RightX,LeftX-RightX).
sectT(LeftX,RightX,LeftX*RightX).
sectT(LeftX,RightX,LeftX/RightX) :- RightX =\= 0.

split(ItemList,List1,List2) :- List1 = [_|_], List2 = [_|_], append(List1,List2,ItemList).

%reads input.txt and invokes arithmatic function with input paramater
do :-
   open('input.txt',read,Str),
   read(Str,ItemList),
   close(Str),
   arithmatic(ItemList).

%does seperations and writes results in output.txt
arithmatic(ItemList) :-
   open('output.txt',write,Stream),
   close(Stream),
   seperate(ItemList,LeftX,RightX),
   open('output.txt',append,Out),
   write(Out, LeftX = RightX ),
   write(Out, "\n"),
   close(Out),
   fail.
arithmatic(_).


