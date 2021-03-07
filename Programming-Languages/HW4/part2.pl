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

% knowledge base
distance(edirne,edremit,914.67).

distance(edremit,edirne,914.67).
distance(edremit,erzincan,736.34).

distance(erzincan,edremit,736.34).

distance(istanbul,izmir,328.80).
distance(istanbul,antalya,482.75).
distance(istanbul,gaziantep,847.42).
distance(istanbul,ankara,351.50).
distance(istanbul,van,1262.37).
distance(istanbul,rize,967.79).

distance(antalya,istanbul,482.75).
distance(antalya,konya,192.28).
distance(antalya,gaziantep,592.33).

distance(burdur,isparta,24.60).

distance(isparta,burdur,24.60).
distance(isparta,izmir,308.55).

distance(izmir,isparta,308.55).
distance(izmir,istanbul,328.80).

distance(gaziantep,istanbul,847.42).
distance(gaziantep,antalya,592.33).

distance(konya,antalya,192.28).
distance(konya,ankara,227.34).

distance(rize,van,373.01).
distance(rize,istanbul,967.79).

distance(van,ankara,920.31).
distance(van,istanbul,1262.37).
distance(van,rize,373.01).

distance(ankara,konya,227.34).
distance(ankara,istanbul,351.50).
distance(ankara,van,920.31).

sroute(X, Y, N) :- 
    distance(X, Y, N).
sroute(X, Z, N) :-
    distance(X, Y, N0),
    sroute(Y, Z, N1),
    N is N0 + N1.


