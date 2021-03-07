% knowledge base
flight(edirne,edremit).

flight(edremit,edirne).
flight(edremit,erzincan).

flight(erzincan,edremit).

flight(istanbul,izmir).
flight(istanbul,antalya).
flight(istanbul,gaziantep).
flight(istanbul,ankara).
flight(istanbul,van).
flight(istanbul,rize).

flight(antalya,istanbul).
flight(antalya,konya).
flight(antalya,gaziantep).

flight(burdur,isparta).

flight(isparta,burdur).
flight(isparta,izmir).

flight(izmir,isparta).
flight(izmir,istanbul).

flight(gaziantep,istanbul).
flight(gaziantep,antalya).

flight(konya,antalya).
flight(konya,ankara).

flight(rize,van).
flight(rize,istanbul).

flight(van,ankara).
flight(van,istanbul).
flight(van,rize).

flight(ankara,konya).
flight(ankara,istanbul).
flight(ankara,van).

%rules
route(A, B) :- visit(A, B, []).
visit(A, B, Visited) :- flight(A, C), not(member(C, Visited)), (B = C; visit(C, B, [A|Visited])).
